#pragma once

#include <csignal>

namespace Gif2UnicornHat {

	class Image;
	class Animation;
	
	/* Inherited by class UnicornHat and UnicornHatHd */
	struct ImageDisplay {
		virtual ~ImageDisplay() {}
		virtual void setBrightness(double) = 0;
		virtual void setOrientation(int) = 0;
		virtual void showImage(const Image&) = 0;
		virtual void playAnimation(const Animation&,
			const volatile std::sig_atomic_t* isAbortRequested = nullptr) = 0;
	};

}
