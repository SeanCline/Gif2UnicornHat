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

private: // Helpers.
	auto isHelpRequested() const -> bool;
	
private: // Data.
	std::vector<std::string> args_;
	std::string gifFilename_;
	double brightness_;
	int orientation_;
};
