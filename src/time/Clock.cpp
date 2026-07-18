module;

#include <Ende/platform.h>
#include <chrono>

export module ende.time.Clock;

import ende.util;

namespace ende::time {

export class Clock {
  public:
    using TimePoint = std::chrono::system_clock::time_point;
    using Duration = std::chrono::nanoseconds;

    Clock() noexcept;

    Clock(const TimePoint &time);

    std::string format(const char *fmt);

    static Clock now();

    Duration durationSince(const TimePoint &time) const;

    Duration elapsed() const;

    u64 seconds() const;

    u64 milliseconds() const;

    u64 microseconds() const;

    u64 nanoseconds() const;

  private:
    TimePoint _start;
};

} // namespace ende::time

ende::time::Clock::Clock() noexcept
    : _start{} {}

ende::time::Clock::Clock(const TimePoint &time)
    : _start(time) {}

std::string ende::time::Clock::format(const char *fmt) {
    std::time_t time = static_cast<std::time_t>(seconds());

    auto tm = localtime(&time);
    char result[20];
    strftime(result, 20, fmt, tm);
    return std::string(result);
}

ende::time::Clock ende::time::Clock::now() {
    return std::chrono::system_clock::now();
}

ende::time::Clock::Duration ende::time::Clock::durationSince(const TimePoint &time) const {
    return std::chrono::duration_cast<std::chrono::microseconds>(_start - time);
}

ende::time::Clock::Duration ende::time::Clock::elapsed() const {
    return now().durationSince(_start);
}

u64 ende::time::Clock::seconds() const {
    return std::chrono::duration_cast<std::chrono::seconds>(elapsed()).count();
}

u64 ende::time::Clock::milliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed()).count();
}

u64 ende::time::Clock::microseconds() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed()).count();
}

u64 ende::time::Clock::nanoseconds() const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed()).count();
}
