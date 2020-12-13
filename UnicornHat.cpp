#include "UnicornHat.h"
#include "Image.h"
#include "Animation.h"
#include <cstring>

extern "C" {
#include <ws2811.h>
}

#include <unistd.h>
#include <signal.h>

#include <stdexcept>
#include <thread>
#include <chrono>

using namespace std;

namespace Gif2UnicornHat {

	bool UnicornHat::alreadyShutdown = false;

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
		
		registerExitHandler();
	}

	UnicornHat::~UnicornHat()
	{
		shutdown();
	}
	
	
	void UnicornHat::setBrightness(double brightness)
	{
		if (brightness < 0 || brightness > 1) {
			throw invalid_argument("Brightness must be between 0.0 and 1.0");
		}
	
		ledstring.channel[0].brightness = (int8_t)(brightness*255);
	}
	
	
	void UnicornHat::setOrientation(int orientation)
	{
		if (orientation < 0 || orientation > 3) {
			throw invalid_argument("Orientation must be between 0 and 3");
		}
	
		orientation_ = orientation;
	}
	
	
	void UnicornHat::showImage(const Image& image)
	{
		if (image.width() > 8 || image.height() > 8) {
			throw invalid_argument("Image is too big for the UnicornHat. An image must be 8x8 pixels to be sent to the hat.");
		}
		
		for (Image::Dimension x = 0; x < image.width(); ++x) {
			for (Image::Dimension y = 0; y < image.height(); ++y) {
				ledstring.channel[0].leds[getPixelIndex(x, y)] = (image[x][y].r << 16) | (image[x][y].g << 8) | image[x][y].b;
			}
		}
		::ws2811_render(&ledstring);
	}

	
	void UnicornHat::playAnimation(const Animation& animation)
	{
		// If this is a static image, conserve CPU by only updating the UnicornHat occasionally.
		if (animation.numLoops() == 0 && animation.numFrames() == 1) {
			// Static image.
			auto& frame = animation.frames()[0];
			while (true) {
				showImage(frame.image);
				this_thread::sleep_for(std::chrono::seconds(5));
			}
		} else {
			// Animation.
			for (int loopNum = 0; loopNum < animation.numLoops() || animation.numLoops() == 0; ++loopNum) {
				for (auto&& frame : animation.frames()) {
					showImage(frame.image);
					this_thread::sleep_for(frame.duration);
				}
			}
		}
	}
	
	
	int UnicornHat::getPixelIndex(int x, int y) const
	{
		const static int indicies_0[8][8] = {
			{0,  1,  2,  3,  4,   5,  6,  7},
			{15, 14, 13, 12, 11, 10,  9,  8},
			{16, 17, 18, 19, 20, 21, 22, 23},
			{31, 30, 29, 28, 27, 26, 25, 24},
			{32, 33, 34, 35, 36, 37, 38, 39},
			{47, 46, 45, 44, 43, 42, 41, 40},
			{48, 49, 50, 51, 52, 53, 54, 55},
			{63, 62, 61, 60, 59, 58, 57, 56}
		};
		
		const static int indicies_90[8][8] = {
			{63, 48, 47, 32, 31, 16, 15, 0, },
			{62, 49, 46, 33, 30, 17, 14, 1, },
			{61, 50, 45, 34, 29, 18, 13, 2, },
			{60, 51, 44, 35, 28, 19, 12, 3, },
			{59, 52, 43, 36, 27, 20, 11, 4, },
			{58, 53, 42, 37, 26, 21, 10, 5, },
			{57, 54, 41, 38, 25, 22, 9,  6, },
			{56, 55, 40, 39, 24, 23, 8,  7, },
		};
		
		const static int indicies_180[8][8] = {
			{56, 57, 58, 59, 60, 61, 62, 63, },
			{55, 54, 53, 52, 51, 50, 49, 48, },
			{40, 41, 42, 43, 44, 45, 46, 47, },
			{39, 38, 37, 36, 35, 34, 33, 32, },
			{24, 25, 26, 27, 28, 29, 30, 31, },
			{23, 22, 21, 20, 19, 18, 17, 16, },
			{8,  9,  10, 11, 12, 13, 14, 15, },
			{7,  6,  5,  4,  3,  2,  1,  0,  }
		};

		const static int indicies_270[8][8] = {
			{7, 8, 23, 24, 39, 40, 55, 56, },
			{6, 9, 22, 25, 38, 41, 54, 57, },
			{5, 10, 21, 26, 37, 42, 53, 58, },
			{4, 11, 20, 27, 36, 43, 52, 59, },
			{3, 12, 19, 28, 35, 44, 51, 60, },
			{2, 13, 18, 29, 34, 45, 50, 61, },
			{1, 14, 17, 30, 33, 46, 49, 62, },
			{0, 15, 16, 31, 32, 47, 48, 63, },
		};

		switch (orientation_) {
			case 0: return indicies_0[x][y];   break;
			case 1: return indicies_90[x][y];  break;
			case 2: return indicies_180[x][y]; break;
			case 3: return indicies_270[x][y]; break;
		}
		
		throw runtime_error("getPixelIndex - Invalid Orientation");
	}
	
	
	void UnicornHat::onSignal(int)
	{
		shutdown();
		exit(0);
	}
	
	
	void UnicornHat::registerExitHandler() const
	{
		const static int signals[] = {
			SIGALRM, SIGHUP, SIGINT, SIGKILL, SIGPIPE, SIGTERM, SIGUSR1, SIGUSR2, SIGPOLL, SIGPROF, SIGVTALRM, //< Termination signals.
			SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGQUIT, SIGSEGV, SIGSYS, SIGTRAP, SIGXCPU //< Aborting signals.
		};
	
		for (auto i = 0u; i < sizeof(signals)/sizeof(signals[0]); ++i) {
			struct sigaction sa;
			memset(&sa, 0, sizeof(struct sigaction));
			sa.sa_handler = onSignal;
			sigaction(signals[i], &sa, nullptr);
		}
	}
	
	
	void UnicornHat::shutdown()
	{
		// Don't run termination logic more than once.
		if (alreadyShutdown) {
			return;
		}
		alreadyShutdown = true;
		
		for (int i = 0; i < 64; ++i) {
			ledstring.channel[0].leds[i] = 0;
		}
		::ws2811_render(&ledstring);
		::ws2811_fini(&ledstring);
	}
	
}
