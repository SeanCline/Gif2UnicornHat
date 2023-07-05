#pragma once

#include <csignal>

namespace Gif2UnicornHat {
	
	auto getAbortFlag() -> const volatile std::sig_atomic_t*;
	
}
