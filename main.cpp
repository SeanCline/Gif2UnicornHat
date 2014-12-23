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

		UnicornHat& hat = UnicornHat::instance();
		
		Gif gif = Gif::fromFile(argv[1]);
		hat.playAnimation(gif.getAnimation());
		
	} catch (exception& ex) {
		cerr << "Exception unwound to main. Details: " << ex.what() << endl;
		throw;
	} catch(...) {
		cerr << "Unknown exception unwound to main." << endl;
		throw;
	}
}
