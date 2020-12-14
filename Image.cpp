#include "Image.h"
#include "Color.h"
#include <vector>
#include <algorithm>

namespace Gif2UnicornHat {

	Image::Image(Dimension width, Dimension height, Color background /*=Color()*/)
		: width_(width),
		  height_(height),
		  canvas_(width*height, background)
	{
	}

		
	Image::Dimension Image::width() const
	{
		return width_;
	}


	Image::Dimension Image::height() const
	{
		return height_;
	}


	Image scale2x() const
	{
		Image img(width_*2, height_*2);
		for (Dimension x = 0; x < width_; ++x) {
			for (Dimension y = 0; y < height_; ++y) {
				// Each pixel in the source turns into 4 in the target.
				const Color px = (*this)[x][y];
				img[2*x][2*y] = px;
				img[2*x][2*y+1] = px;
				img[2*x+1][2*y] = px;
				img[2*x+1][2*y+1] = px;
			}
		}
	}
	

	void Image::fill(const Color& c)
	{
		std::fill(begin(canvas_), end(canvas_), c);
	}


	Image::AccessorProxy::AccessorProxy(Image* image, Dimension x)
		: image_(image),
		  x_(x)
	{
	}


	Color& Image::AccessorProxy::operator[] (Dimension y)
	{
		return image_->canvas_[y*image_->width()+x_];
	}


	const Color& Image::AccessorProxy::operator[] (Dimension y) const
	{
		return image_->canvas_[y*image_->width()+x_];
	}


	Image::AccessorProxy Image::operator[] (Dimension x)
	{
		return AccessorProxy(this, x);
	}


	const Image::AccessorProxy Image::operator[] (Dimension x) const
	{
		return AccessorProxy(const_cast<Image*>(this), x);
	}

}
