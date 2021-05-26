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

    };

    enum class ClockMode {
        REALTIME = 1,
        MONOTONIC = 2
    };

    TimeSpec now(ClockMode mode = ClockMode::REALTIME);

}

#endif //ENDE_SYS_TIME_H
