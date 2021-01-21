#pragma once

#include "ImageDisplay.h"

namespace Gif2UnicornHat {

	class Image;
	class Animation;
	
	/* Use the rpi_ws2811 library to send images to the UnicornHat. */
	class UnicornHat : public ImageDisplay {
		typedef unsigned int Dimension;
		
		public: // Construction.
			UnicornHat();
			~UnicornHat();
		
		private: // Noncopyable / Nonmovable.
			UnicornHat(const UnicornHat&) = delete;
			UnicornHat& operator=(const UnicornHat&) = delete;
			UnicornHat(UnicornHat&&) = delete;
			UnicornHat& operator=(UnicornHat&&) = delete;
		
		public: // Dimensions.
			static constexpr Dimension width()  { return 8; };
			static constexpr Dimension height() { return 8; };
		
		public: // Methods.
			void setBrightness(double) override;
			void setOrientation(int) override;
			void showImage(const Image&) override;
			void playAnimation(const Animation&, const volatile bool* isAbortRequested = nullptr) override;
			
		private: // Helpers.
			Dimension getPixelIndex(Dimension x, Dimension y) const;
			
		private: // Data.
			int orientation_;
	};

}
