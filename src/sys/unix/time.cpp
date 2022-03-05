//
// Created by cstro29 on 19/5/21.
//

#include "Ende/sys/time.h"
#include <ctime>

const u32 NANO_PER_SEC = 1e9;

//static_cast<i64>(sec + (nano / NANO_PER_SEC)), static_cast<i32>(nano % NANO_PER_SEC)

ende::sys::TimeSpec ende::sys::TimeSpec::operator+(const TimeSpec &rhs) const {
    //TODO: I think this math is correct check later
    i64 nanoseconds = nano + rhs.nano;
    i64 seconds = sec + rhs.sec + (nanoseconds / NANO_PER_SEC);
    return {seconds, static_cast<i32>(nanoseconds % NANO_PER_SEC)};
}

ende::sys::TimeSpec ende::sys::TimeSpec::operator-(const TimeSpec &rhs) const {
    i64 nanoseconds = nano - rhs.nano;
    i64 seconds = sec - rhs.sec - (nanoseconds / NANO_PER_SEC);
    return {seconds, static_cast<i32>(nanoseconds % NANO_PER_SEC)};
}

ende::sys::TimeSpec &ende::sys::TimeSpec::operator=(const TimeSpec &rhs) {
    sec = rhs.sec;
    nano = rhs.nano;
    return *this;
}

ende::sys::TimeSpec ende::sys::now(ClockMode mode) {
    clockid_t m = 0;
    switch (mode) {
        case ClockMode::REALTIME:
            m = CLOCK_REALTIME;
            break;
        case ClockMode::MONOTONIC:
            m = CLOCK_MONOTONIC;
            break;
        default:
            return {};
    }
    struct timespec time;
    if (clock_gettime(m, &time) != 0)
        return {};
    return {static_cast<i64>(time.tv_sec), static_cast<i32>(time.tv_nsec)};
}