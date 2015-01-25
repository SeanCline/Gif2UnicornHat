#include "ProgramOptions.h"

#include <string>
#include <vector>
#include <exception>
#include <iostream>

using namespace std;

ProgramOptions::ProgramOptions(int argc, char* argv[])
	 : args_(argv, argv+argc),
	   brightness_(.2)
{
	// Process the help option.
	if (isHelpRequested()) {
		cout << getHelpString() << endl;
		exit(0);
	}
	
	// Process the gif filename.
	if (args_.size() < 2) {
		cerr << "Too few arguments." << endl;
		cout << getHelpString() << endl;
		exit(0);
	}
	
	gifFilename_ = args_[1];
	
	// Process the brightness.
	if (args_.size() > 2) {
		try {
			brightness_ = stod(args_[2]);
		} catch (exception&) {
			cerr << "Invalid brightness." << endl;
			exit(0);
		}
	}
}


ProgramOptions::~ProgramOptions()
{

}


auto ProgramOptions::getHelpString() const -> string
{
	return "Usage:\n"
	       "   Gif2UnicornHat path-to-gif [brightness]\n"
	       "   (where brightness is a value between 0.0 and 1.0. Defaults to 0.2)\n"
		   "\n"
	       "Example Usage:\n"
	       "   Gif2UnicornHat examples/star.gif";
}


auto ProgramOptions::getGifFilename() const -> string
{
	return gifFilename_;
}


auto ProgramOptions::getBrightness() const -> double
{
	return brightness_;
}


auto ProgramOptions::isHelpRequested() const -> bool
{
	for (auto& arg : args_) {
		if (arg == "-h" || arg == "--help") {
			return true;
		}
	}

	return false;
}
