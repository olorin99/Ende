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
            i64 seconds = sec * rhs;
            i64 nanoseconds = nano * rhs;
            if (nanoseconds >= 1000000000) {
                nanoseconds -= 1000000000;
                ++seconds;
            } else if (nanoseconds < 0) {
                nanoseconds = 1000000000 - nanoseconds;
                --seconds;
            }
            return { sec * rhs, nano * rhs };
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
