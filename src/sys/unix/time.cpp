//
// Created by cstro29 on 19/5/21.
//

#include "Ende/sys/time.h"
#include <ctime>

ende::sys::TimeSpec ende::sys::TimeSpec::operator+(const TimeSpec &rhs) const {
    return {sec + rhs.sec, nano + rhs.nano};
}

ende::sys::TimeSpec ende::sys::TimeSpec::operator-(const TimeSpec &rhs) const {
    return {sec - rhs.sec, nano - rhs.nano};
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