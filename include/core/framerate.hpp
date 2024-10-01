#pragma once

#include <chrono>
#include <cstdint>

namespace core
{
class framerate
{
public:
    explicit framerate(const std::uint32_t seconds)
        : _framerate(seconds), _frame_period(calc_frametime(seconds)) {}

    [[nodiscard]] std::chrono::seconds seconds() const;
    [[nodiscard]] std::chrono::nanoseconds frame_period() const;

private:
    std::chrono::seconds _framerate;
    std::chrono::nanoseconds _frame_period;

    [[nodiscard]] static std::chrono::nanoseconds calc_frametime(std::uint32_t target_framerate) ;
};
} // namespace core

