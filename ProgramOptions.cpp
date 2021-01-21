#include "ProgramOptions.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <getopt.h>

using namespace std;

ProgramOptions::ProgramOptions(int argc, char* argv[])
	 : brightness_(.2),
	   orientation_(0),
	   deviceName_("Unicorn HAT")
{
	optind = 0;

	while (true)
	{
		static struct option long_options[] =
		{
		  {"filename",    required_argument, 0, 'f'},
		  {"brightness",  required_argument, 0, 'b'},
		  {"orientation", required_argument, 0, 'o'},
		  {"device",      required_argument, 0, 'd'},
		  {"help",        no_argument,       0, '?'},
		  {0, 0, 0, 0}
		};

		int option_index = 0;
		int c = getopt_long(argc, argv, "f:b:o:d:?h", long_options, &option_index);

		if (c == -1)
			break; //< No more options to parse.

		switch (c)
		{
			case 0: {
				break; //< Nothing to do for flag options.
			}

			case 'f': {
				gifFilename_ = optarg;
				break;
			}

			case 'b': {
				try {
					brightness_ = std::stod(optarg);
				} catch (std::exception&) {
					throw std::runtime_error("Invalid brightness.");
				}
				break;
			}

			case 'o': {
				try {
					orientation_ = std::stoi(optarg) % 4;
				} catch (std::exception&) {
					throw std::runtime_error("Invalid orientation.");
				}
				break;
			}

			case 'd': {
				deviceName_ = optarg;
				break;
			}

			case 'h': case '?': {
				std::cout << getHelpString();
				exit(0);
			}

			default: {
				throw std::runtime_error("Unexpected argument. -" + std::to_string(c));
			}
		}
	}

	// Handle the old positional way the Gif2UnicornHat used to parse args.
	for (int i = optind; i < argc; ++i) {
		int option_pos = i - optind;
		if (option_pos == 0) {
			gifFilename_ = argv[i];
		} else if (option_pos == 1) {
			try {
				brightness_ = std::stod(argv[i]);
			} catch (std::exception&) {
				throw std::runtime_error("Invalid brightness.");
			}
		} else if (option_pos == 2) {
			try {
				orientation_ = std::stoi(argv[i]) % 4;
			} catch (std::exception&) {
				throw std::runtime_error("Invalid orientation.");
			}
		} else {
			throw std::runtime_error("Too many arguments. Use --help for more details.");
		}
	}

	// Make sure we at least have the filename.
	if (gifFilename_.empty()) {
		throw std::runtime_error("No filename provided.");
	}

	//std::cout << "Gif: " << gifFilename_ << "\n";
	//std::cout << "Brightness: " << brightness_ << "\n";
	//std::cout << "Orientation:" << orientation_ << "\n";
	//std::cout << "Device: " << deviceName_ << "\n";
}


ProgramOptions::~ProgramOptions()
{

}


auto ProgramOptions::getHelpString() const -> std::string
{
	return "Usage:\n"
		   "   Gif2UnicornHat path-to-gif [-b <brightness>] [-o <orientation>] [-d device]\n"
		   "   brightness: between 0.0 and 1.0. Defaults to 0.2\n"
		   "   orientation: between 0 and 3 to rotate the display. Defaults to 0\n"
		   "   device: Either \"Unicorn HAT\" or \"Unicorn HAT HD\". Defaults to \"Unicorn HAT\"\n"
		   "\n"
		   "Example Usage:\n"
		   "   Gif2UnicornHat examples/star.gif\n";
}


auto ProgramOptions::getGifFilename() const -> std::string
{
	return gifFilename_;
}


auto ProgramOptions::getBrightness() const -> double
{
	return brightness_;
}


auto ProgramOptions::getOrientation() const -> int
{
	return orientation_;
}


auto ProgramOptions::getDevice() const -> std::string
{
	return deviceName_;
}