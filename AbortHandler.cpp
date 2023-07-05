#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <csignal>

namespace Gif2UnicornHat {
	
	auto getAbortFlag() -> const volatile std::sig_atomic_t*
	{
		static volatile std::sig_atomic_t abortFlag = 0;
		static bool isInitialized = false;
		
		const static int signals[] = {
			SIGALRM, SIGHUP, SIGINT, SIGKILL, SIGPIPE, SIGTERM, SIGUSR1, SIGUSR2, SIGPOLL, SIGPROF, SIGVTALRM, //< Termination signals.
			SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGQUIT, SIGSEGV, SIGSYS, SIGTRAP, SIGXCPU //< Aborting signals.
		};

		if (!isInitialized) { //< Only initialize the signal handlers once.
			for (auto i = 0u; i < sizeof(signals)/sizeof(signals[0]); ++i) {
				struct sigaction sa = {};
				sa.sa_handler = [] (int) {
					abortFlag = 1;
				};
				sigaction(signals[i], &sa, nullptr);
			}
			isInitialized = true;
		}
		
		return &abortFlag;
	}

}