#include "Hat.h"
#include "UnicornHat.h"
#include "UnicornHatHd.h"

#include <string>
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <memory>

#include "trim.h"

namespace {
	std::string readFileContents(const std::string& filename)
	{
		std::ifstream in(filename);
		return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
	}
}

namespace Gif2UnicornHat {
	
	// Reads the name of the connected HAT, "Unicorn HAT" for example from /proc
	auto getConnectedHatName() -> std::string
	{
		return trim(readFileContents("/proc/device-tree/hat/product"));
	}

	// Create an instance of ImageDisplay based on what the hat's EEPROM says.
	auto createImageDisplay(const std::string& hatName) -> std::unique_ptr<ImageDisplay>
	{
		if (hatName == "Unicorn HAT") {
			return std::unique_ptr<UnicornHat>(new UnicornHat);
		} else if (hatName == "Unicorn HAT HD") {
			return std::unique_ptr<UnicornHatHd>(new UnicornHatHd);
		} else if (hatName.empty()) {
			throw std::runtime_error("No hat connected.");
		} else {
			throw std::runtime_error("Unrecognized hat: <" + hatName + ">");
		}
	}

}
