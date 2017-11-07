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

	UnicornHat& UnicornHat::instance()
	{
		static UnicornHat hat; 
		return hat;
	}


	UnicornHat::UnicornHat()
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
		for (int loopNum = 0; loopNum < animation.numLoops() || animation.numLoops() == 0; ++loopNum) {
			for (auto&& frame : animation.frames()) {
				showImage(frame.image);
				this_thread::sleep_for(frame.duration);
			}
		}
	}
	
	
	int UnicornHat::getPixelIndex(int x, int y) const
	{
		const static int indicies[8][8] = {
			{0,  1,  2,  3,  4,   5,  6,  7},
			{15, 14, 13, 12, 11, 10,  9,  8},
			{16, 17, 18, 19, 20, 21, 22, 23},
			{31, 30, 29, 28, 27, 26, 25, 24},
			{32, 33, 34, 35, 36, 37, 38, 39},
			{47, 46, 45, 44, 43, 42, 41, 40},
			{48, 49, 50, 51, 52, 53, 54, 55},
			{63, 62, 61, 60, 59, 58, 57, 56}
		};

		return indicies[x][y];
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
