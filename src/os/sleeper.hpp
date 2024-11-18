#ifndef __SLEEP_H_
#define __SLEEP_H_

#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#endif


namespace os
{
	class sleeper
	{
		static const std::chrono::seconds DefaultTimeout;

	public:
		explicit sleeper(std::chrono::seconds timeout = DefaultTimeout);
		~sleeper();

		void sleep(std::chrono::nanoseconds nanoseconds) const;

	private:
		std::chrono::seconds _timeout;

#ifdef _WIN32
		TIMECAPS tc {0};
		UINT wTimerRes {0};
#endif
	};

} // namespace os

#endif
