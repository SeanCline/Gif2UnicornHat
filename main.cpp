#include "Gif.h"
#include "UnicornHat.h"
#include "Animation.h"
#include "ProgramOptions.h"

#include <cstdlib>
#include <string>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>

using namespace std;
using namespace Gif2UnicornHat;


int main(int argc, char *argv[])
{	
	try {
		UnicornHat& hat = UnicornHat::instance();
		
		ProgramOptions opts(argc, argv);
		
		Gif gif = Gif::fromFile(opts.getGifFilename());
		hat.setBrightness(opts.getBrightness());
		hat.playAnimation(gif.getAnimation());
		
	} catch (exception& ex) {
		cerr << "Exception unwound to main. Details: " << ex.what() << endl;
		throw;
	} catch(...) {
		cerr << "Unknown exception unwound to main." << endl;
		throw;
	}
}
