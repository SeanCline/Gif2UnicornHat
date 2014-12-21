#pragma once
#include <cstdint>
#include <limits>

namespace Gif2UnicornHat {

	struct Color {
		typedef uint8_t SubpixelType;
		SubpixelType r, g, b, a;
		
		Color();
		Color(SubpixelType red, SubpixelType green, SubpixelType blue, SubpixelType alpha = 255);
		
		bool operator==(Color other);
		
		static const Color Transparent, White, Black, Red, Green, Blue;
	};

}
