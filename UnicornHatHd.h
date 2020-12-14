#pragma once

#include "ImageDisplay.h"

namespace Gif2UnicornHat {

	class Image;
	class Animation;
	
	/* Use /dev/spidev*to send images to the Unicorn HAT HD. */
	class UnicornHatHd : public ImageDisplay {
		public: // Construction.
			UnicornHatHd();
			~UnicornHatHd();
		
		private: // Noncopyable / Nonmovable.
			UnicornHatHd(const UnicornHatHd&) = delete;
			UnicornHatHd& operator=(const UnicornHatHd&) = delete;
			UnicornHatHd(UnicornHatHd&&) = delete;
			UnicornHatHd& operator=(UnicornHatHd&&) = delete;
		
		public: // Methods.
			void setBrightness(double) override;
			void setOrientation(int) override;
			void showImage(const Image&) override;
			void playAnimation(const Animation&) override;
		
		private: // Helpers.
			int getPixelIndex(int x, int y) const;
		
		/*
		private: // Signal handling to make sure we safely shutdown.
			void registerExitHandler() const;
			static void onSignal(int);
			static bool alreadyShutdown;
			static void shutdown();
		*/
		
		private: // Data.
			int orientation_;
			double brightness_;
			int fd_;
	};

}
