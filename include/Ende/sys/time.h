//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_SYS_TIME_H
#define ENDE_SYS_TIME_H

#include <Ende/platform.h>

namespace ende::sys {

    struct TimeSpec {
        i64 sec = 0;
        i32 nano = 0;

        TimeSpec operator+(const TimeSpec& rhs) const;

        TimeSpec operator-(const TimeSpec& rhs) const;

        template <typename T>
        TimeSpec operator*(T rhs) const {
            i64 nanoseconds = nano * rhs;
            i64 seconds = sec * rhs.sec + (nanoseconds / 1000000000);
            return { seconds, static_cast<i32>(nanoseconds % 1000000000) };
        }

        template <typename T>
        TimeSpec operator/(T rhs) const {
            i64 nanoseconds = nano / rhs;
            i64 seconds = sec / rhs.sec + (nanoseconds / 1000000000);
            return { seconds, static_cast<i32>(nanoseconds % 1000000000) };
        }

        TimeSpec& operator=(const TimeSpec& rhs);

    };

    enum class ClockMode {
        REALTIME = 1,
        MONOTONIC = 2
    };

    TimeSpec now(ClockMode mode = ClockMode::REALTIME);

}

#endif //ENDE_SYS_TIME_H
