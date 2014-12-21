#include "Gif.h"
#include "Animation.h"

extern "C" {
#include <ws2812-RPi.h>
}

#include <unistd.h>
#include <signal.h>

#include <cstdlib>
#include <string>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>

using namespace std;
using namespace GifLoader;

void registerExitHandler()
{
	auto onExit = [](int status) {
		cout << "Exiting..." << endl;
		for (int i = 0; i < 64; ++i) {
			setPixelColor(i, 0, 0, 0);
		}
		show();
		terminate(status);
	};

	for (int i = 0; i < 64; ++i) {
		struct sigaction sa;
		memset(&sa, 0, sizeof(struct sigaction));
		sa.sa_handler = onExit;
		sigaction(i, &sa, nullptr);
	}
}


int getPixelIndex(int x, int y)
{
	int indicies[8][8] = {
		{7 ,6 ,5 ,4 ,3 ,2 ,1 ,0 },
		{8 ,9 ,10,11,12,13,14,15},
		{23,22,21,20,19,18,17,16},
		{24,25,26,27,28,29,30,31},
		{39,38,37,36,35,34,33,32},
		{40,41,42,43,44,45,46,47},
		{55,54,53,52,51,50,49,48},
		{56,57,58,59,60,61,62,63}
	};
	
	return indicies[7-x][y];
}


void showImage(const Image& img) {
	for (Image::Dimension x = 0; x < img.width(); ++x) {
		for (Image::Dimension y = 0; y < img.height(); ++y) {
			setPixelColor(getPixelIndex(x, y), img[x][y].r, img[x][y].g, img[x][y].b);
		}
	}
	show();
}


int main(int argc, char *argv[])
{	
	try {
		if (argc < 2) {
			cerr << "Missing gif file argument." << endl;
		}
	
		Gif gif = Gif::fromFile(argv[1]);

		numLEDs = 64;
		initHardware();
		clearLEDBuffer();
		setBrightness(DEFAULT_BRIGHTNESS);
		registerExitHandler();
		
		for (int loopNum = 0; loopNum < gif.numLoops() || gif.numLoops() == 0; ++loopNum) {
			for (auto&& frame : gif.frames()) {
				showImage(frame.image);
				this_thread::sleep_for(frame.duration);
			}
		}
	} catch (exception& ex) {
		cerr << "Exception unwound to main. Details: " << ex.what() << endl;
		throw;
	} catch(...) {
		cerr << "Unknown exception unwound to main." << endl;
		throw;
	}
}
