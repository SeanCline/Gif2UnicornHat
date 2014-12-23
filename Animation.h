#pragma once
#include "Image.h"
#include <chrono>
#include <vector>

namespace Gif2UnicornHat {

	struct AnimationFrame {
		Image image;
		std::chrono::milliseconds duration;
		
		AnimationFrame(const Image& image, const std::chrono::milliseconds& ms);
	};

	class Animation {
	public: // Construction.
		Animation();
	
	public: // Frame getters and setters.
		typedef std::vector<AnimationFrame> FrameList;
		const FrameList& frames() const;
		FrameList& frames();
		void appendFrame(const AnimationFrame&);
		
	public: // Other properties.
		bool isLooped() const;
		int numLoops() const;
		void setNumLoops(int);
		int numFrames() const;
		
	private: //< Data.
		int numLoops_;
		FrameList frames_;
	};

}
