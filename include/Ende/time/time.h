//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_TIME_H
#define ENDE_TIME_H

#include <Ende/platform.h>
#include <Ende/sys/time.h>
#include <string>

namespace ende::time {

    class Duration {
    public:

        Duration(u64 sec = 0, u32 nano = 0) noexcept;

        Duration(const ende::sys::TimeSpec& time);

        Duration& operator=(const Duration& rhs);

        static Duration fromSeconds(f64 seconds);

        static Duration fromMilliseconds(f64 milli);

        static Duration fromMicroseconds(u64 micro);

        static Duration fromNanoseconds(u32 nano);


        i64 seconds() const;

        i64 milliseconds() const;

        i64 microseconds() const;

        i64 nanoseconds() const;

        sys::TimeSpec spec() const;


        Duration operator+(const Duration& rhs) const;

        Duration operator-(const Duration& rhs) const;

        Duration operator*(i32 rhs) const {
            return { _time * rhs };
        }

        Duration operator/(i32 rhs) const {
            return { _time / rhs };
        }

        Duration& operator+= (const Duration& rhs);

        Duration& operator-= (const Duration& rhs);

        bool operator==(const Duration& rhs) const;

        bool operator!=(const Duration& rhs) const;

        bool operator<(const Duration& rhs) const;

        bool operator>(const Duration& rhs) const;


    private:
        sys::TimeSpec _time;
    };


    class Instant {
    public:

        static Instant now();

        Instant(u64 sec = 0, u32 nano = 0) noexcept;

        Duration durationSince(const Instant& instant) const;

        Duration elapsed() const;

        u64 nanoseconds() const;

    private:
        Instant( const sys::TimeSpec& time);

        sys::TimeSpec _time;
    };

    constexpr const char* ddmmyyyy = "%d/%m/%Y";
    constexpr const char* mmddyyyy = "%m/%d/%Y";
    constexpr const char* ddmmyy = "%d/%m/%y";

    constexpr const char* hhmmss = "%I:%M:%S";
    constexpr const char* HHMM = "%H:%M";

    class SystemTime {
    public:

        SystemTime() noexcept;

        SystemTime(const sys::TimeSpec& time);

        std::string format(const char* fmt);

        static SystemTime now();

        Duration durationSince(const SystemTime& time) const;

        Duration elapsed() const;


        u64 seconds() const;

        u64 milliseconds() const;

        u64 microseconds() const;

        u64 nanoseconds() const;

    private:

        sys::TimeSpec _time;

    };

}

inline ende::time::Duration operator""_nano(unsigned long long s) {
    return ende::time::Duration(0, s);
}

inline ende::time::Duration operator""_micro(unsigned long long s) {
    return ende::time::Duration(0, s * 1e3);
}

inline ende::time::Duration operator""_milli(unsigned long long s) {
    return ende::time::Duration(0, s * 1e6);
}

inline ende::time::Duration operator""_sec(unsigned long long s) {
    return ende::time::Duration(s, 0);
}

inline ende::time::Duration operator""_min(unsigned long long s) {
    return ende::time::Duration(s * 60, 0);
}

inline ende::time::Duration operator""_hour(unsigned long long s) {
    return ende::time::Duration(s * 60 * 60, 0);
}

inline ende::time::Duration operator""_day(unsigned long long s) {
    return ende::time::Duration(s * 60 * 60 * 24, 0);
}

inline ende::time::Duration operator""_month(unsigned long long s) {
    return ende::time::Duration(s * 60 * 60 * 24 * 30, 0);
}

inline ende::time::Duration operator""_year(unsigned long long s) {
    return ende::time::Duration(s * 60 * 60 * 24 * 365, 0);
}

#endif //ENDE_TIME_H
