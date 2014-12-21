#include "UnicornHat.h"
#include "Image.h"

extern "C" {
#include <ws2812-RPi.h>
}

#include <unistd.h>
#include <signal.h>

#include <stdexcept>
#include <iostream>

using namespace std;

namespace Gif2UnicornHat {

	bool UnicornHat::alreadyShutdown = false;

	UnicornHat& UnicornHat::instance()
	{
		static UnicornHat hat = UnicornHat();
		return hat;
	}


	UnicornHat::UnicornHat()
	{
		numLEDs = 64;
		initHardware();
		clearLEDBuffer();
		setBrightness(DEFAULT_BRIGHTNESS);
		registerExitHandler();
	}

	UnicornHat::~UnicornHat()
	{
		shutdown();
	}
	

	void UnicornHat::showImage(const Image& img)
	{
		if (img.width() > 8 || img.height() > 8) {
			throw runtime_error("Image is too big for the UnicornHat. An image must be 8x8 pixels to be sent to the hat.");
		}
		
		for (Image::Dimension x = 0; x < img.width(); ++x) {
			for (Image::Dimension y = 0; y < img.height(); ++y) {
				setPixelColor(getPixelIndex(x, y), img[x][y].r, img[x][y].g, img[x][y].b);
			}
		}
		show();
	}

	
	int UnicornHat::getPixelIndex(int x, int y)
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
	
	
	void UnicornHat::registerExitHandler()
	{
		auto onSignal = [](int) {
			shutdown();
		};

		for (int i = 0; i < 64; ++i) {
			struct sigaction sa;
			memset(&sa, 0, sizeof(struct sigaction));
			sa.sa_handler = onSignal;
			sigaction(i, &sa, nullptr);
		}
	}
	
	
	void UnicornHat::shutdown()
	{
		// Don't run termination logic more than once.
		if (alreadyShutdown) {
			return;
		}
		alreadyShutdown = true;
		
		cout << "Exiting..." << endl;
		for (int i = 0; i < 64; ++i) {
			setPixelColor(i, 0, 0, 0);
		}
		show();
		terminate(0);
	}
	
}