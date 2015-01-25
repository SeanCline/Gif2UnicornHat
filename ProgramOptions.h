#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/positional_options.hpp>

#include <string>

class ProgramOptions
{
public: // Construction/Destruction.
	ProgramOptions(int argc, char* argv[]);
	~ProgramOptions();
	
private: // Non-copyable.
	ProgramOptions(const ProgramOptions&) = delete;
	ProgramOptions& operator=(const ProgramOptions&) = delete;

public: // Methods.
	void printHelp() const;
	auto isHelpRequested() const -> bool;
	auto getGifFilename() const -> std::string;
	auto getBrightness() const -> double;
	
private: // Data.
	boost::program_options::variables_map varsMap_;
	boost::program_options::options_description optionsDesc_;
	boost::program_options::positional_options_description positionalDesc_;
};
