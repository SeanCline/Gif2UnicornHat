#pragma once

namespace Gif2UnicornHat {

	class Image;
	class Animation;
	
	class UnicornHat {
		public: // Singleton.
			static UnicornHat& instance();
		
		private: // Construction.
			UnicornHat();
			~UnicornHat();
		
		private: // Noncopyable / Nonmovable.
			UnicornHat(const UnicornHat&) = delete;
			UnicornHat& operator=(const UnicornHat&) = delete;
			UnicornHat(UnicornHat&&) = delete;
			UnicornHat& operator=(UnicornHat&&) = delete;
		
		public: // Methods.
			void setBrightness(double);
			void showImage(const Image&);
			void playAnimation(const Animation&);
			
		private: // Helpers.
			int getPixelIndex(int x, int y) const;
			
		private: // Signal handling to make sure we safely shutdown.
			void registerExitHandler() const;
			static void onSignal(int);
			static bool alreadyShutdown;
			static void shutdown();
	};

}
