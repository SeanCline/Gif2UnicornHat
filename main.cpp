#include "Gif.h"
#include "Animation.h"
#include "ProgramOptions.h"
#include "Hat.h"

#include <string>
#include <iostream>
#include <exception>
#include <memory>

using namespace Gif2UnicornHat;

int main(int argc, char *argv[])
{
	try {
		ProgramOptions opts(argc, argv);
		Gif gif = Gif::fromFile(opts.getGifFilename());
		
		auto hatName = getConnectedHatName();
		auto hat = createImageDisplay(hatName);
		hat->setBrightness(opts.getBrightness());
		hat->setOrientation(opts.getOrientation());
		hat->playAnimation(gif.getAnimation());
		
	} catch (std::exception& ex) {
		std::cerr << "Exception unwound to main. Details: " << ex.what() << std::endl;
		throw;
	} catch(...) {
		std::cerr << "Unknown exception unwound to main." << std::endl;
		throw;
	}
}
