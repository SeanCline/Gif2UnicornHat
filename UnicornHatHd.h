#pragma once

#include "ImageDisplay.h"

namespace Gif2UnicornHat {

	class Image;
	class Animation;
	
	/* Use /dev/spidev*to send images to the Unicorn HAT HD. */
	class UnicornHatHd : public ImageDisplay {
		typedef unsigned int Dimension;
		
		public: // Construction.
			UnicornHatHd();
			~UnicornHatHd();
		
		private: // Noncopyable / Nonmovable.
			UnicornHatHd(const UnicornHatHd&) = delete;
			UnicornHatHd& operator=(const UnicornHatHd&) = delete;
			UnicornHatHd(UnicornHatHd&&) = delete;
			UnicornHatHd& operator=(UnicornHatHd&&) = delete;
		
		public: // Dimensions.
			static constexpr Dimension width()  { return 16; };
			static constexpr Dimension height() { return 16; };
		
		public: // Methods.
			void setBrightness(double) override;
			void setOrientation(int) override;
			void showImage(const Image&) override;
			void playAnimation(const Animation&) override;
		
		private: // Helpers.
			Dimension getPixelIndex(Dimension x, Dimension y) const;
		
		private: // Data.
			int orientation_;
			double brightness_;
			int fd_;
	};

}
