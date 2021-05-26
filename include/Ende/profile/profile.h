//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_PROFILE_H
#define ENDE_PROFILE_H

#include <Ende/sys/time.h>

namespace ende::profile {

    struct ProfileData {
        const char* func;
        i32 line;
        const char* file;
        sys::TimeSpec time;
    };

    void submit(ProfileData&& data);

    class Profile {
    public:

        explicit Profile(const char* func, i32 line, const char* file)
            : _func(func),
            _line(line),
            _file(file),
            _start(sys::now(sys::ClockMode::MONOTONIC))
        {}

        ~Profile() {
            submit({_func, _line, _file, sys::now(sys::ClockMode::MONOTONIC) - _start});
        }

    private:

        const char* _func;
        i32 _line;
        const char* _file;
        sys::TimeSpec _start;

    };


}

#endif //ENDE_PROFILE_H
