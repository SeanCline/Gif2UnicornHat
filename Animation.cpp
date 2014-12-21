#include "Animation.h"
#include "Image.h"
#include <chrono>
#include <vector>
#include <stdexcept>

using namespace std;
using namespace std::chrono;

namespace GifLoader {

	AnimationFrame::AnimationFrame(const Image& image, const milliseconds& ms)
		: image(image),
		  duration(ms)
	{ }


	const Animation::FrameList& Animation::frames() const
	{
		return frames_;
	}


	Animation::FrameList& Animation::frames()
	{
		return frames_;
	}


	void Animation::appendFrame(const AnimationFrame& frame)
	{
		frames_.push_back(frame);
	}


	bool Animation::isLooped() const
	{
		return numLoops_ >= 0;
	}


	int Animation::numLoops() const
	{
		if (!isLooped()) {
			throw runtime_error("Animation is not looped.");
		}
		
		return numLoops_;
	}


	void Animation::setNumLoops(int numLoops)
	{
		numLoops_ = numLoops;
	}


	int Animation::numFrames() const
	{
		return frames_.size();
	}

}
