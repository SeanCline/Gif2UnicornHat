#include "Gif.h"
#include "UnicornHat.h"
#include "Animation.h"

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
		if (argc < 2) {
			cerr << "Missing gif file argument." << endl;
		}
	
		
		Gif gif = Gif::fromFile(argv[1]);
		UnicornHat& hat = UnicornHat::instance();
		
		for (int loopNum = 0; loopNum < gif.numLoops() || gif.numLoops() == 0; ++loopNum) {
			for (auto&& frame : gif.frames()) {
				hat.showImage(frame.image);
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
