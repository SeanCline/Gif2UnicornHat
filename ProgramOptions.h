#pragma once

#include <string>
#include <vector>

class ProgramOptions
{
public: // Construction/Destruction.
	ProgramOptions(int argc, char* argv[]);
	~ProgramOptions();

private: // Non-copyable.
	ProgramOptions(const ProgramOptions&) = delete;
	ProgramOptions& operator=(const ProgramOptions&) = delete;

public: // Methods.
	auto getHelpString() const ->std::string;
	auto getGifFilename() const -> std::string;
	auto getBrightness() const -> double;
	auto getOrientation() const -> int;
	auto getDevice() const -> std::string;

private: // Data.
	std::string gifFilename_;
	double brightness_;
	int orientation_;
	std::string deviceName_;
};
