#pragma once

namespace Gif2UnicornHat {

	class Image;

	class UnicornHat {
		public: // Singleton.
			static UnicornHat& instance();
		
		private: // Construction.
			UnicornHat();
			~UnicornHat();
		
		public: // Methods.
			void showImage(const Image&);
			
		private: // Helpers.
			int getPixelIndex(int x, int y);
			void registerExitHandler();
			static void onSignal(int);
			static bool alreadyShutdown;
			static void shutdown();
			
	};

}