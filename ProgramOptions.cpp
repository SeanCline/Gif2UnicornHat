#include "ProgramOptions.h"

#include <string>
#include <chrono>
#include <boost/program_options.hpp>

using namespace std;
using namespace std::chrono;
using namespace boost::program_options;


ProgramOptions::ProgramOptions(int argc, char* argv[])
	 : optionsDesc_("Main options"),
	   positionalDesc_()
{
	optionsDesc_.add_options()
		("help,h", "Print this help message.")
		("gif-file", value<string>()->required(), "Path to gif file to play on the hat.")
		("brightness", value<double>()->default_value(0.2), "Unicorn Hat brightness (0.0 to 1.0)")
	;
	
	positionalDesc_.add("gif-file", 1);
	
	auto parsedOpts = command_line_parser(argc, argv).options(optionsDesc_).positional(positionalDesc_).run();
	store(parsedOpts, varsMap_);

	// Process the help option.
	if (isHelpRequested()) 
	{ 
		printHelp();
		exit(0);
	}
	
	notify(varsMap_);
}


ProgramOptions::~ProgramOptions()
{

}


void ProgramOptions::printHelp() const
{
	cout << optionsDesc_ << endl;
}


auto ProgramOptions::isHelpRequested() const -> bool
{
	return (varsMap_.count("help") > 0);
}


auto ProgramOptions::getGifFilename() const -> string
{
	return varsMap_["gif-file"].as<string>();
}


auto ProgramOptions::getBrightness() const -> double
{
	return varsMap_["brightness"].as<double>();
}
