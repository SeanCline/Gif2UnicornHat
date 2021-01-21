#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <signal.h>

namespace Gif2UnicornHat {
	static volatile bool abortFlag = false;
	
	auto getAbortFlag() -> const volatile bool*
	{
		const static int signals[] = {
			SIGALRM, SIGHUP, SIGINT, SIGKILL, SIGPIPE, SIGTERM, SIGUSR1, SIGUSR2, SIGPOLL, SIGPROF, SIGVTALRM, //< Termination signals.
			SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGQUIT, SIGSEGV, SIGSYS, SIGTRAP, SIGXCPU //< Aborting signals.
		};

		for (auto i = 0u; i < sizeof(signals)/sizeof(signals[0]); ++i) {
			struct sigaction sa = {};
			sa.sa_handler = [] (int) {
				abortFlag = true;
			};
			sigaction(signals[i], &sa, nullptr);
		}
		
		return &abortFlag;
	}

}