#include "GifException.h"
#include <gif_lib.h>
#include <string>

using namespace std;

namespace Gif2UnicornHat {

	GifException::GifException(int gifErrorCode)
		: runtime_error("Gif Error: " + string(GifErrorString(gifErrorCode))),
		  code_(gifErrorCode)
	{
	}


	int GifException::code() const
	{
		return code_;
	}

}
