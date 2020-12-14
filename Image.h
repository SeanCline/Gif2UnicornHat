#pragma once
#include "Color.h"
#include <vector>
#include <cstddef>

namespace Gif2UnicornHat {

	class Image {
	public: // Construction.
		typedef size_t Dimension;
		Image(Dimension width, Dimension height, Color background = Color());
		
	public: // Dimensions.
		Dimension width() const;
		Dimension height() const;
		Image scale2x() const;
		
	public: // Drawing.
		void fill(const Color&);
		
	public: // Pixel Access.
		// A proxy object used to index into an Image twice for x and y.
		// https://en.wikibooks.org/wiki/More_C++_Idioms/Temporary_Proxy
		class AccessorProxy {
		public: // Construction.
			AccessorProxy(Image* image, Dimension x);
			
		public: // Default construct, copy construct, copy assign, and destruct.
			AccessorProxy() = default;
			AccessorProxy(const AccessorProxy&) = default;
			AccessorProxy& operator=(const AccessorProxy&) = default;
			~AccessorProxy() = default;
			
		public: // Pixel Access.
			Color& operator[] (Dimension y);
			const Color& operator[] (Dimension y) const;
			
		private: // Data.
			Image* image_;
			Dimension x_;
		};
		
		AccessorProxy operator[] (Dimension x);
		const AccessorProxy operator[] (Dimension x) const;
		
	private: // Data.
		Dimension width_;
		Dimension height_;
		std::vector<Color> canvas_;
	};

}
