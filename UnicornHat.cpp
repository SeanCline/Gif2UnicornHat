#include "UnicornHat.h"
#include "Image.h"
#include "Animation.h"

extern "C" {
#include <ws2811.h>
}

#include <stdexcept>
#include <thread>
#include <chrono>

namespace Gif2UnicornHat {

	ws2811_t ledstring;

	UnicornHat::UnicornHat()
		: orientation_(0)
	{
		ledstring.freq = WS2811_TARGET_FREQ;
		ledstring.dmanum = 5;
		ledstring.channel[0].gpionum = 18;
		ledstring.channel[0].count = 64;
		ledstring.channel[0].invert = 0;
		ledstring.channel[0].brightness = 55;
		ledstring.channel[0].strip_type = WS2811_STRIP_GRB;
		
		::ws2811_init(&ledstring);
	}

	UnicornHat::~UnicornHat()
	{
		for (int i = 0; i < 64; ++i) {
			ledstring.channel[0].leds[i] = 0;
		}
		::ws2811_render(&ledstring);
		::ws2811_fini(&ledstring);
	}
	
	
	void UnicornHat::setBrightness(double brightness)
	{
		if (brightness < 0 || brightness > 1) {
			throw std::invalid_argument("Brightness must be between 0.0 and 1.0");
		}
	
		ledstring.channel[0].brightness = (int8_t)(brightness*255);
	}
	
	
	void UnicornHat::setOrientation(int orientation)
	{
		if (orientation < 0 || orientation > 3) {
			throw std::invalid_argument("Orientation must be between 0 and 3");
		}
	
		orientation_ = orientation;
	}
	
	
	void UnicornHat::showImage(const Image& image)
	{
		if (image.width() > width() || image.height() > height())
			throw std::invalid_argument("Image is too big for the UnicornHat. Must be 8x8 pixels.");
		
		for (Dimension x = 0; x < image.width(); ++x) {
			for (Dimension y = 0; y < image.height(); ++y) {
				ledstring.channel[0].leds[getPixelIndex(x, y)] = (image[x][y].r << 16) | (image[x][y].g << 8) | image[x][y].b;
			}
		}
		::ws2811_render(&ledstring);
	}

	
	void UnicornHat::playAnimation(const Animation& animation, const volatile bool* isAbortRequested)
	{
		// If this is a static image, conserve CPU by only updating the UnicornHat occasionally.
		if (animation.numLoops() == 0 && animation.numFrames() == 1) {
			// Static image.
			auto& frame = animation.frames()[0];
			while (true) {
				showImage(frame.image);
				std::this_thread::sleep_for(std::chrono::seconds(5));
				if (isAbortRequested && *isAbortRequested)
					return;
			}
		} else {
			// Animation.
			for (int loopNum = 0; loopNum < animation.numLoops() || animation.numLoops() == 0; ++loopNum) {
				for (auto&& frame : animation.frames()) {
					showImage(frame.image);
					std::this_thread::sleep_for(frame.duration);
					if (isAbortRequested && *isAbortRequested)
						return;
				}
			}
		}
	}
	
	
	UnicornHat::Dimension UnicornHat::getPixelIndex(Dimension x, Dimension y) const
	{
		if (x >= width() || y >= height())
			throw std::out_of_range("getPixelIndex - Pixel coordinates out of range.");
			
		// The Unicorn HAT simplifies its PCB design by zig-zagging each row.
		const static Dimension indicies[width()][height()] = {
			{0,  1,  2,  3,  4,   5,  6,  7},
			{15, 14, 13, 12, 11, 10,  9,  8},
			{16, 17, 18, 19, 20, 21, 22, 23},
			{31, 30, 29, 28, 27, 26, 25, 24},
			{32, 33, 34, 35, 36, 37, 38, 39},
			{47, 46, 45, 44, 43, 42, 41, 40},
			{48, 49, 50, 51, 52, 53, 54, 55},
			{63, 62, 61, 60, 59, 58, 57, 56}
		};

		switch (orientation_) {
			case 0: return indicies[x][y];                      //< 0 degrees. (No rotation.)
			case 1: return indicies[y][height()-1-x];           //< +90 degrees.
			case 2: return indicies[width()-1-x][height()-1-y]; //< +180 degrees.
			case 3: return indicies[width()-1-y][x];            //< +270 degrees.
		}
		
		throw std::runtime_error("getPixelIndex - Invalid Orientation");
	}

}
