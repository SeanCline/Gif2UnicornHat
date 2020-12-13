#include "ImageDisplay.h"

#include <string>
#include <memory>

namespace Gif2UnicornHat {
	
	// Reads the name of the connected HAT, "Unicorn HAT" for example from /proc
	std::string getConnectedHatName();

	// Create an instance of ImageDisplay based on what the hat's EEPROM says.
	std::unique_ptr<ImageDisplay> createImageDisplay(const std::string& hatName);

}
