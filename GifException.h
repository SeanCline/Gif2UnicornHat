#pragma once
#include <stdexcept>

namespace GifLoader {

	class GifException : public std::runtime_error {
	public:
		GifException();
		int code() const;
		
	private:
		int code_;
	};

}
