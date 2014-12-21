#include "Gif.h"
#include "GifException.h"
#include "Color.h"
#include "Image.h"

#include <gif_lib.h>

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <utility>

using namespace Gif2UnicornHat;
using namespace std;
using namespace std::chrono;

namespace {

	int combineBytesToInt(uint8_t loByte, uint8_t hiByte)
	{
		return loByte + hiByte*0xFFu;
	}

	Color lookupColor(const ColorMapObject& colorMap, int colorIndex) noexcept
	{
		auto& entry = colorMap.Colors[colorIndex];
		return Color(entry.Red, entry.Green, entry.Blue);
	}
	
	
	void drawImageRecordToCanvas(GifFileType& file, Image& canvas, int transparentIndex = -1)
	{
		if (DGifGetImageDesc(&file) == GIF_ERROR) {
			throw GifException();
		}

		auto& img = file.Image;
		
		if (img.ColorMap == nullptr && file.SColorMap == nullptr) {
			throw runtime_error("Missing color map.");
		}
		
		// Use the local colour map if present. Otherwise, use the global one.
		auto& colorMap = (img.ColorMap != nullptr) ? *img.ColorMap : *file.SColorMap;

		if ((img.Left + img.Width) > file.SWidth || (img.Top + img.Height) > file.SHeight) {
			throw runtime_error("Image extends outside of canvas dimensions.");
		}
		
		// Place one line at a time on the canvas.
		vector<GifPixelType> imgLine(img.Width);
		auto copyLineToCanvas = [&] (int targetLine) {
			if (DGifGetLine(&file, imgLine.data(), img.Width) == GIF_ERROR) {
				throw GifException();
			}
			
			// Copy the line into the canvas.
			for (int j = 0; j < img.Width; ++j) {
				if (imgLine[j] != transparentIndex) {
					canvas[img.Left+j][img.Top+targetLine] = lookupColor(colorMap, imgLine[j]);
				} else {
					// Use the a fully transparent colour instead.
					canvas[img.Left+j][img.Top+targetLine] = Color::Transparent;
				}
			}
		};
		
		// Read each line into the canvas.
		if (file.Image.Interlace) {
			// 4-pass interlacing.
			int interlaceOffset[] = { 0, 4, 2, 1 };
			int interlaceStep[] = { 8, 8, 4, 2 };
			for (int pass = 0; pass < 4; ++pass) {
				for (int i = interlaceOffset[pass]; i < (img.Top + img.Height); i += interlaceStep[pass]) {
					copyLineToCanvas(i);
				}
			}
		} else {
			for (int i = 0; i < img.Height; ++i) {
				copyLineToCanvas(i);
			}
		}
	}
	

	struct GraphicsControlBlock {
		enum DisposalMethod {
			NotSpecified = 0, DoNotDispose, RestoreToBackground, RestoreToPrevious
		};
		
		DisposalMethod disposalMethod = NotSpecified;
		bool userInputFlag = false;
		bool transparentColorFlag = false;
		milliseconds delayTime = milliseconds(0);
		int transparentColorIndex = 0;
	};
	
	
	GraphicsControlBlock readGraphicsControlBlock(GifByteType* extData)
	{
		GraphicsControlBlock gcb;
		
		if (extData[0] != 4) {
			throw runtime_error("Unexpected size of graphics control block.");
		}
		
		gcb.disposalMethod = static_cast<GraphicsControlBlock::DisposalMethod>((extData[1] & 0x1C) >> 2);
		gcb.userInputFlag = (extData[1] & 0x2);
		gcb.transparentColorFlag = (extData[1] & 0x1);
		gcb.delayTime = milliseconds(combineBytesToInt(extData[2], extData[3])*10);
		gcb.transparentColorIndex = extData[4];
		
		return gcb;
	}
}

namespace Gif2UnicornHat {

	Gif Gif::fromFile(const std::string& path, Color defaultBackground /*= Color()*/)
	{
		// Open the file.
		auto deletor = [] (GifFileType* f) { if (f) DGifCloseFile(f); };
		auto file = unique_ptr<GifFileType, decltype(deletor)>(DGifOpenFileName(path.c_str()), deletor);
		
		if (file == nullptr) {
			throw runtime_error("Failed to open gif file: " + path);
		}
		
		// Figure out what background colour to use.
		Color backgroundColor = defaultBackground;
		if (file->SColorMap != nullptr && file->SBackGroundColor > 0) {
			backgroundColor = lookupColor(*file->SColorMap, file->SBackGroundColor);
		}
		
		// A place to store the last frame and the next control block.
		Image lastCanvas(file->SWidth, file->SHeight, backgroundColor);
		GraphicsControlBlock gcb;
		Animation animation;
		
		while (true) {
			GifRecordType recordType;
			if (DGifGetRecordType(file.get(), &recordType) == GIF_ERROR) {
				throw GifException();
			}
			
			if (recordType == IMAGE_DESC_RECORD_TYPE) {
				Image canvas(file->SWidth, file->SHeight, backgroundColor);
				
				if (gcb.disposalMethod == GraphicsControlBlock::DoNotDispose) {
					// Copy in the last canvas and use it as the starting point.
					canvas = lastCanvas;
				}
				
				drawImageRecordToCanvas(*file, canvas, (gcb.transparentColorFlag ? gcb.transparentColorIndex : -1));
				
				if (gcb.delayTime == milliseconds(0) && !animation.frames().empty()) {
					// Replace the last frame when no delay is specified.
					animation.frames().back().image = canvas;
				} else {
					// Append a new frame.
					animation.appendFrame({canvas, gcb.delayTime});
				}
				
				if (gcb.disposalMethod != GraphicsControlBlock::RestoreToPrevious) {
					// Store the canvas for the next frame, when RestoreToPrevious isn't set.
					lastCanvas = move(canvas);
				}
				
				// Clear the GCB so it doesn't apply to the next frame.
				gcb = GraphicsControlBlock();
			} else if (recordType == EXTENSION_RECORD_TYPE) {
				int extCode;
				GifByteType* extData;
				if (DGifGetExtension(file.get(), &extCode, &extData) == GIF_ERROR) {
					throw GifException();
				}
				
				if (extCode == APPLICATION_EXT_FUNC_CODE) //< Stores loop count animated images.
				{
					auto extString = string(extData+1, extData+12);
					if (extString == "NETSCAPE2.0") { //< Is this the animation extension record?
						// The number of loops is in the next extension record, but don't fail if it isn't there.
						if (DGifGetExtensionNext(file.get(), &extData) != GIF_ERROR) {
							if (extData[0] == 3) {
								animation.setNumLoops(combineBytesToInt(extData[2], extData[3]));
							}
						}
					}
				} else if (extCode == GRAPHICS_EXT_FUNC_CODE) { //< Stores frame delay for each image.
					gcb = readGraphicsControlBlock(extData);
				}
				
				// Skip any other extension blocks.
				while (extData != nullptr) {
					if (DGifGetExtensionNext(file.get(), &extData) == GIF_ERROR) {
						throw GifException();
					}
				}
			} else if (recordType == TERMINATE_RECORD_TYPE) {
				break;
			}
		}
		
		return Gif(file->SWidth, file->SHeight, animation);
	}

	Gif::Gif(Image::Dimension width, Image::Dimension height, Animation animation)
		: width_(width),
		  height_(height),
		  animation_(animation)
	{
	}


	Image::Dimension Gif::height() const
	{
		return height_;
	}


	Image::Dimension Gif::width() const
	{
		return width_;
	}


	bool Gif::isAnimated() const
	{
		return animation_.numFrames() > 0;
	}

	
	int Gif::numLoops() const
	{
		return animation_.numLoops();
	}
	

	const Animation::FrameList& Gif::frames() const
	{
		return animation_.frames();
	}


	const Image& Gif::getStaticImage() const
	{
		if (animation_.numFrames() == 0) {
			throw runtime_error("Gif contains no image records.");
		}
		
		return animation_.frames().front().image;
	}

}
