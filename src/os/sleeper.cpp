#include "sleeper.hpp"

const std::chrono::seconds  os::sleeper::DefaultTimeout{ 30 };

/* Platform specific code below */
#ifdef _WIN32
#include <stdexcept>

os::sleeper::sleeper(std::chrono::seconds timeout) : _timeout(timeout)
{    
    static constexpr std::uint8_t TARGET_RESOLUTION = 1;

    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
    {
        throw std::runtime_error("Failed to call timeGetDevCaps");
    }

    wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
    timeBeginPeriod(wTimerRes);
}

os::sleeper::~sleeper()
{
    timeEndPeriod(wTimerRes);
}

void os::sleeper::sleep(const std::chrono::nanoseconds nanoseconds) const {
    const auto timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds> (_timeout);

    HANDLE timer;
    LARGE_INTEGER time_definition;
    
    if (!(timer = CreateWaitableTimer(nullptr, TRUE, nullptr)))
    {
        throw std::runtime_error("Failed to create a timer");
    }    
    
    time_definition.QuadPart = -nanoseconds.count() / 100;

    if (!SetWaitableTimer(timer, &time_definition, 0, nullptr, nullptr, FALSE)) {
        CloseHandle(timer);
        throw std::runtime_error("Failed to set waitable timer properties");
    }
    
    WaitForSingleObject(timer, timeout_ms.count());
    
    CloseHandle(timer);
}
#endif



#ifdef __linux__
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

os::sleeper::sleeper(std::chrono::seconds timeout) : _timeout(timeout) {}
os::sleeper::~sleeper() {}

// https://www.informit.com/articles/article.aspx?p=23618&seqNum=11
void os::sleeper::sleep(std::chrono::nanoseconds nanoseconds)
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds> (nanoseconds).count();
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = ms % 1000 * 1000000;
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
}
#endif

#ifdef __APPLE__
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

os::sleeper::sleeper(std::chrono::seconds timeout) : _timeout(timeout) {}
os::sleeper::~sleeper() {}

// https://www.informit.com/articles/article.aspx?p=23618&seqNum=11
void os::sleeper::sleep(std::chrono::nanoseconds nanoseconds)
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds> (nanoseconds).count();
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = ms % 1000 * 1000000;
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
}
#endif
