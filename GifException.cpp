#include "GifException.h"
#include <gif_lib.h>
#include <string>

using namespace std;

namespace Gif2UnicornHat {

	GifException::GifException()
		: runtime_error("Gif Error: " + GifLastError()),
		  code_(GifLastError())
	{
	}

	int GifException::code() const
	{
		return code_;
	}

}
