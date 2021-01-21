#pragma once

#include "ImageDisplay.h"

#include <string>
#include <memory>

namespace Gif2UnicornHat {
	
	// Reads the name of the connected HAT, "Unicorn HAT" for example from /proc.
	auto getConnectedHatName() -> std::string;

	// Create an instance of ImageDisplay based on what the hat's EEPROM says.
	auto createImageDisplay(const std::string& hatName) -> std::unique_ptr<ImageDisplay>;

}
