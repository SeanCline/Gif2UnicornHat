#include "UnicornHatHd.h"
#include "Image.h"
#include "Animation.h"

#include <stdexcept>
#include <thread>
#include <chrono>
#include <cstdint>

// SPI headers.
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

namespace {
	namespace SpiSettings { // SPI constants for Unicorn HAT HD.
		constexpr static auto device = "/dev/spidev0.0";
		constexpr static auto mode = 0;
		constexpr static auto bits_per_word = 8;
		constexpr static auto max_speed_hz = 9000000;
		constexpr static uint8_t start_byte = 0x72;
	}
}

namespace Gif2UnicornHat {

	UnicornHatHd::UnicornHatHd()
		: orientation_(0),
		  brightness_(1),
		  fd_(-1)
	{
		fd_ = open(SpiSettings::device, O_RDWR);
		if (fd_ < 0) {
			throw std::runtime_error("Failed to open SPI device '" + std::string(SpiSettings::device) + "'\n"
				"Make sure SPI is enabled in raspi-config or /boot/config.txt.\n"
				"For more details, see: "
				"https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md#driver");
		}

		if (ioctl(fd_, SPI_IOC_WR_MODE, &SpiSettings::mode) == -1)
			throw std::runtime_error("Failed to set SPI mode.");

		if (ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &SpiSettings::bits_per_word) == -1)
			throw std::runtime_error("Failed to set bits per word");
			
		if (ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &SpiSettings::max_speed_hz) == -1)
			throw std::runtime_error("Failed to set max speed");
	}

	UnicornHatHd::~UnicornHatHd()
	{
		// Show a blank image.
		showImage(Image(width(), height()));
		close(fd_);
	}
	
	
	void UnicornHatHd::setBrightness(double brightness)
	{
		if (brightness < 0 || brightness > 1) {
			throw std::invalid_argument("Brightness must be between 0.0 and 1.0");
		}
	
		brightness_ = brightness;
	}
	
	
	void UnicornHatHd::setOrientation(int orientation)
	{
		if (orientation < 0 || orientation > 3) {
			throw std::invalid_argument("Orientation must be between 0 and 3");
		}
	
		orientation_ = orientation;
	}
	
	
	void UnicornHatHd::showImage(const Image& image)
	{
		if (image.width() > width() || image.height() > height())
			throw std::invalid_argument("Image is too big for the UnicornHatHd. Must be 16x16 pixels.");

		// If image is 8x8, then do pixel doubling up to 16x16.
		if (image.width()*2 == width() && image.height()*2 == height())
			return showImage(image.scale2x());

		if (fd_ < 0)
			throw std::runtime_error("Cannot write image data. SPI device is not open.");
		
		std::vector<uint8_t> buffer;
		buffer.resize(1 + (width()*height()*3)); //< Start byte + subpixel data.
		buffer[0] = SpiSettings::start_byte;
		for (Dimension x = 0; x < image.width(); ++x) {
			for (Dimension y = 0; y < image.height(); ++y) {
				const auto pixelIndex = getPixelIndex(x, y)*3 + 1;
				// Write each subpixel to the buffer.
				buffer[pixelIndex]   = static_cast<uint8_t>(image[x][y].r * brightness_);
				buffer[pixelIndex+1] = static_cast<uint8_t>(image[x][y].g * brightness_);
				buffer[pixelIndex+2] = static_cast<uint8_t>(image[x][y].b * brightness_);
			}
		}
		
		struct spi_ioc_transfer tr = {};
		tr.tx_buf = (uintptr_t)buffer.data();
		tr.len = buffer.size();
		tr.speed_hz = SpiSettings::max_speed_hz;
		tr.bits_per_word = SpiSettings::bits_per_word;
		
		if(ioctl(fd_, SPI_IOC_MESSAGE(1), &tr) < 1)
			throw std::runtime_error("Failed to write image data over SPI.");
	}

	
	void UnicornHatHd::playAnimation(const Animation& animation, const volatile bool* isAbortRequested)
	{
		// If this is a static image, conserve CPU by only updating the UnicornHat occasionally.
		if (animation.numLoops() == 0 && animation.numFrames() == 1) {
			// Static image.
			auto& frame = animation.frames()[0];
			while (true) {
				showImage(frame.image);
				std::this_thread::sleep_for(std::chrono::seconds(5));
				if (isAbortRequested && *isAbortRequested)
					return;
			}
		} else {
			// Animation.
			for (int loopNum = 0; loopNum < animation.numLoops() || animation.numLoops() == 0; ++loopNum) {
				for (auto&& frame : animation.frames()) {
					showImage(frame.image);
					std::this_thread::sleep_for(frame.duration);
					if (isAbortRequested && *isAbortRequested)
						return;
				}
			}
		}
	}
	
	
	UnicornHatHd::Dimension UnicornHatHd::getPixelIndex(Dimension x, Dimension y) const
	{
		// Make sure the index is in bounds.
		if (x >= width() || y >= height())
			throw std::out_of_range("getPixelIndex - Pixel coordinates out of range.");
		
		// Convert from x,y coordinates to a flat array index.
		auto index = [] (Dimension x, Dimension y) {
			return y*height() + x;
		};
		
		// Rotate based on the orientation.
		switch (orientation_) {
			case 0: return index(x, y);                      //< 0 degrees. (No rotation.)
			case 1: return index(y, height()-1-x);           //< +90 degrees.
			case 2: return index(width()-1-x, height()-1-y); //< +180 degrees.
			case 3: return index(width()-1-y, x);            //< +270 degrees.
		}
		
		throw std::runtime_error("getPixelIndex - Invalid Orientation");
	}
}
