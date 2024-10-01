#include <core/frame_limiter.hpp>
#include <os/sleeper.hpp>
#include <chrono>

void core::frame_limiter::wait_remainder() const
{
    const auto frame_time = _frame_timer.current_frame_time();
    const auto wait_time = _framerate.frame_period() - frame_time;
    _sleeper.sleep(wait_time);
}
