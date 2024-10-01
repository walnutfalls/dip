#include <core/frame_limiter.hpp>
#include <string>

using float_seconds = std::chrono::duration<float>;

void core::frame_timer::begin_total()
{
  _begin_total = _high_res_timer.now();
}

void core::frame_timer::start()
{
  _start = _high_res_timer.now();
}

void core::frame_timer::end()
{
  _end = _high_res_timer.now();
  _delta = _end - _start;
  _smoothed_delta = _frame_average.update(_delta);
}

std::chrono::nanoseconds core::frame_timer::delta() const
{
  return _delta;
}

std::chrono::nanoseconds core::frame_timer::smoothed_delta() const
{
  return _smoothed_delta;
}



float_seconds core::frame_timer::delta_seconds() const
{
  return std::chrono::duration_cast<float_seconds>(_delta);
}

core::float_seconds core::frame_timer::smoothed_delta_seconds() const
{
  return std::chrono::duration_cast<float_seconds>(_smoothed_delta);
}



std::chrono::nanoseconds core::frame_timer::current_frame_time() const
{
  return std::chrono::high_resolution_clock::now() - _start;
}

std::string core::frame_timer::frame_info() const
{
  return "delta: " + std::to_string(delta().count()) + "ns\n" +
    "smoothed delta: " + std::to_string(smoothed_delta().count()) + "ns\n" +
    "current frame time: " + std::to_string(current_frame_time().count()) + "ns\n";
}

float core::frame_timer::smoothed_delta_seconds_f() const
{
  return std::chrono::duration_cast<float_seconds>(_smoothed_delta).count();
}

float core::frame_timer::delta_seconds_f() const
{
  return std::chrono::duration_cast<float_seconds>(_delta).count();
}

