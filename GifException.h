#pragma once
#include <stdexcept>

namespace Gif2UnicornHat {

	class GifException : public std::runtime_error {
	public:
		GifException();
		int code() const;
		
	private:
		int code_;
	};

}
