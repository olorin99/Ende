#ifndef ENDE_CLOCK_H
#define ENDE_CLOCK_H

#include <Ende/platform.h>
#include <chrono>

namespace ende::time {

        class Clock {
	    public:

            using TimePoint = std::chrono::system_clock::time_point;
            using Duration = std::chrono::nanoseconds;
	
	        Clock() noexcept;
	
	        Clock(const TimePoint& time);
	
	        std::string format(const char* fmt);
	
	        static Clock now();
	
	        Duration durationSince(const TimePoint& time) const;
	
	        Duration elapsed() const;
	
	
	        u64 seconds() const;
	
	        u64 milliseconds() const;
	
	        u64 microseconds() const;
	
	        u64 nanoseconds() const;
	
	    private:

            TimePoint _start; 
	
	    };

}


#endif //ENDE_CLOCK_H
