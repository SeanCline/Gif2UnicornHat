#include "Color.h"
#include <cstdint>
#include <limits>

namespace GifLoader {

	const Color Color::Transparent = {0, 0, 0, 0};
	const Color Color::White = {255, 255, 255};
	const Color Color::Black = {0, 0, 0};
	const Color Color::Red = {255, 0, 0};
	const Color Color::Green = {0, 255, 0};
	const Color Color::Blue = {0, 0, 255};

	Color::Color()
		: r(0), g(0), b(0), a(0)
	{
	}

	Color::Color(SubpixelType red, SubpixelType green, SubpixelType blue, SubpixelType alpha)
		: r(red), g(green), b(blue), a(alpha)
	{
	}

	bool Color::operator==(Color other)
	{
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}

}
