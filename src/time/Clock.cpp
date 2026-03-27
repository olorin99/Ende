#include <Ende/time/Clock.h>
#include <chrono>


ende::time::Clock::Clock() noexcept
	    : _start{}
	{}
	
ende::time::Clock::Clock(const TimePoint &time)
    : _start(time)
{}

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