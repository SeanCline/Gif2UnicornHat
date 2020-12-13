#pragma once

#include "ImageDisplay.h"

namespace Gif2UnicornHat {

	class Image;
	class Animation;
	
	/* Use the rpi_ws2811 library to send images to the UnicornHat. */
	class UnicornHat : public ImageDisplay {
		public: // Construction.
			UnicornHat();
			~UnicornHat();
		
		private: // Noncopyable / Nonmovable.
			UnicornHat(const UnicornHat&) = delete;
			UnicornHat& operator=(const UnicornHat&) = delete;
			UnicornHat(UnicornHat&&) = delete;
			UnicornHat& operator=(UnicornHat&&) = delete;
		
		public: // Methods.
			void setBrightness(double) override;
			void setOrientation(int) override;
			void showImage(const Image&) override;
			void playAnimation(const Animation&) override;
			
		private: // Helpers.
			int getPixelIndex(int x, int y) const;
			
		private: // Signal handling to make sure we safely shutdown.
			void registerExitHandler() const;
			static void onSignal(int);
			static bool alreadyShutdown;
			static void shutdown();
			
		private: // Data.
			int orientation_;
	};

}
