#include "UnicornHatHd.h"
#include "Image.h"
#include "Animation.h"

#include <unistd.h>
#include <signal.h>

#include <stdexcept>
#include <thread>
#include <chrono>
#include <cstdint>

// SPI headers.
#include <fcntl.h>
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

using namespace std;

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
		
		//registerExitHandler();
	}

	UnicornHatHd::~UnicornHatHd()
	{
		// Show a blank image.
		showImage(Image(16, 16));
		
		close(fd_);
		
		//shutdown();
	}
	
	
	void UnicornHatHd::setBrightness(double brightness)
	{
		if (brightness < 0 || brightness > 1) {
			throw invalid_argument("Brightness must be between 0.0 and 1.0");
		}
	
		brightness_ = brightness;
	}
	
	
	void UnicornHatHd::setOrientation(int orientation)
	{
		if (orientation < 0 || orientation > 3) {
			throw invalid_argument("Orientation must be between 0 and 3");
		}
	
		orientation_ = orientation;
	}
	
	
	void UnicornHatHd::showImage(const Image& image)
	{
		if (image.width() > 16 || image.height() > 16) {
			throw std::invalid_argument("Image is too big for the UnicornHat. An image must be 8x8 pixels to be sent to the hat.");
		}
		
		if (fd_ < 0)
			throw std::runtime_error("Cannot write image data. SPI device is not open.");
		
		// TODO: If image is 8x8, then do pixel doubling up to 16x16.
		// TODO: If orientation_ is non-zero, transform the image.
		
		std::vector<uint8_t> buffer;
		buffer.resize(1 + (16*16*3)); //< Start byte + subpixel data.
		buffer[0] = SpiSettings::start_byte;
		for (Image::Dimension x = 0; x < image.width(); ++x) {
			for (Image::Dimension y = 0; y < image.height(); ++y) {
				const auto pixelIndex = getPixelIndex(x, y) + 1;
				// Write each subpixel to the buffer.
				buffer[pixelIndex]   = static_cast<uint8_t>(image[x][y].r * brightness_);
				buffer[pixelIndex+1] = static_cast<uint8_t>(image[x][y].g * brightness_);
				buffer[pixelIndex+2] = static_cast<uint8_t>(image[x][y].b * brightness_);
			}
		}
		
		struct spi_ioc_transfer tr = {0};
		tr.tx_buf = (uintptr_t)buffer.data();
		tr.len = buffer.size();
		tr.speed_hz = SpiSettings::max_speed_hz;
		tr.bits_per_word = SpiSettings::bits_per_word;
		
		if(ioctl(fd_, SPI_IOC_MESSAGE(1), &tr) < 1)
			throw std::runtime_error("Failed to write image data over SPI.");
	}

	
	void UnicornHatHd::playAnimation(const Animation& animation)
	{
		// If this is a static image, conserve CPU by only updating the UnicornHat occasionally.
		if (animation.numLoops() == 0 && animation.numFrames() == 1) {
			// Static image.
			auto& frame = animation.frames()[0];
			while (true) {
				showImage(frame.image);
				this_thread::sleep_for(std::chrono::seconds(5));
			}
		} else {
			// Animation.
			for (int loopNum = 0; loopNum < animation.numLoops() || animation.numLoops() == 0; ++loopNum) {
				for (auto&& frame : animation.frames()) {
					showImage(frame.image);
					this_thread::sleep_for(frame.duration);
				}
			}
		}
	}
	
	
	int UnicornHatHd::getPixelIndex(int x, int y) const
	{
		if (x > 15 || y > 15)
			throw std::out_of_range("UnicornHatHd::getPixelIndex - Pixel coordinates out of range.");
			
		// TODO: Support orientation.
		return (y*16+x)*3;
	}
	
	/*
	void UnicornHatHd::onSignal(int)
	{
		shutdown();
		exit(0);
	}
	
	
	void UnicornHatHd::registerExitHandler() const
	{
		const static int signals[] = {
			SIGALRM, SIGHUP, SIGINT, SIGKILL, SIGPIPE, SIGTERM, SIGUSR1, SIGUSR2, SIGPOLL, SIGPROF, SIGVTALRM, //< Termination signals.
			SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGQUIT, SIGSEGV, SIGSYS, SIGTRAP, SIGXCPU //< Aborting signals.
		};
	
		for (auto i = 0u; i < sizeof(signals)/sizeof(signals[0]); ++i) {
			struct sigaction sa;
			memset(&sa, 0, sizeof(struct sigaction));
			sa.sa_handler = onSignal;
			sigaction(signals[i], &sa, nullptr);
		}
	}
	
	
	void UnicornHatHd::shutdown()
	{
		// Don't run termination logic more than once.
		if (fd_ < 0)
			return;
		
		// Show a blank image.
		showImage(Image(16, 16));
		
		close(fd_)
		fd_ = -1;
	}
	*/
}
