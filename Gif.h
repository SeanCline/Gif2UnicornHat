#pragma once
#include "Color.h"
#include "Animation.h"
#include <string>

class GifFileType;

namespace Gif2UnicornHat {

	class Gif {
	public: // Construction/Destruction.
		static Gif fromFile(const std::string& path, Color defaultBackground = Color());
	
	private: // Construction/Destruction.
		explicit Gif(Image::Dimension width, Image::Dimension height, Animation Animation);
	
	public: // Image Attributes.
		Image::Dimension height() const;
		Image::Dimension width() const;
		bool isAnimated() const;
		int numLoops() const;
	
	public: // Image access.
		const Animation::FrameList& frames() const;
		const Image& getStaticImage() const;
	
	private: // Data.
		Image::Dimension width_;
		Image::Dimension height_;
		Animation animation_;
		Color backgroundColor_;
	};

}
