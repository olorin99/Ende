//
// Created by cstro29 on 19/5/21.
//

#include "Ende/time/time.h"
#include <ctime>

const u32 NANO_PER_SEC = 1e9;

ende::time::Duration::Duration(u64 sec, u32 nano) noexcept
    : _time({static_cast<i64>(sec + (nano / NANO_PER_SEC)), static_cast<i32>(nano % NANO_PER_SEC)})
{}

ende::time::Duration::Duration(const ende::sys::TimeSpec &time)
    : _time(time)
{}

u64 ende::time::Duration::seconds() const {
    return _time.sec;
}

u64 ende::time::Duration::milliseconds() const {
    return _time.sec * 1e3 + _time.nano / 1e6;
}

u64 ende::time::Duration::microseconds() const {
    return _time.sec * 1e6 + _time.nano / 1e3;
}

u64 ende::time::Duration::nanoseconds() const {
    return _time.sec * 1e9 + _time.nano;
}

ende::sys::TimeSpec ende::time::Duration::spec() const {
    return _time;
}

ende::time::Duration ende::time::Duration::operator+(const Duration &rhs) const {
    return _time + rhs._time;
}

ende::time::Duration ende::time::Duration::operator-(const Duration &rhs) const {
    return _time - rhs._time;
}

bool ende::time::Duration::operator==(const Duration &rhs) const {
    return _time.sec == rhs._time.sec && _time.nano == rhs._time.nano;
}

bool ende::time::Duration::operator!=(const Duration &rhs) const {
    return _time.sec != rhs._time.sec || _time.nano != rhs._time.nano;
}

bool ende::time::Duration::operator<(const Duration &rhs) const {
    return nanoseconds() < rhs.nanoseconds();
}

bool ende::time::Duration::operator>(const Duration &rhs) const {
    return nanoseconds() > rhs.nanoseconds();
}



ende::time::Instant ende::time::Instant::now() {
    return sys::now(sys::ClockMode::MONOTONIC);
}

ende::time::Instant::Instant(u64 sec, u32 nano) noexcept
    : _time({static_cast<i64>(sec + (nano / NANO_PER_SEC)), static_cast<i32>(nano % NANO_PER_SEC)})
{}

ende::time::Duration ende::time::Instant::durationSince(const Instant &instant) const {
    return _time - instant._time;
}

ende::time::Duration ende::time::Instant::elapsed() const {
    return now().durationSince(*this);
}

ende::time::Instant::Instant(const sys::TimeSpec &time)
    : _time(time)
{}



ende::time::SystemTime::SystemTime() noexcept
    : _time{}
{}

ende::time::SystemTime::SystemTime(const sys::TimeSpec &time)
    : _time(time)
{}

std::string ende::time::SystemTime::format(const char *fmt) {
    std::time_t time = static_cast<std::time_t>(seconds());

    auto tm = localtime(&time);
    char result[20];
    strftime(result, 20, fmt, tm);
    return std::string(result);
}

ende::time::SystemTime ende::time::SystemTime::now() {
    return sys::now(sys::ClockMode::REALTIME);
}

ende::time::Duration ende::time::SystemTime::durationSince(const SystemTime &time) const {
    return _time - time._time;
}

ende::time::Duration ende::time::SystemTime::elapsed() const {
    return now().durationSince(*this);
}

u64 ende::time::SystemTime::seconds() const {
    return _time.sec;
}

u64 ende::time::SystemTime::milliseconds() const {
    return _time.sec * 1e3 + _time.nano / 1e6;
}

u64 ende::time::SystemTime::microseconds() const {
    return _time.sec * 1e6 + _time.nano / 1e3;
}

u64 ende::time::SystemTime::nanoseconds() const {
    return _time.sec * 1e9 + _time.nano;
}