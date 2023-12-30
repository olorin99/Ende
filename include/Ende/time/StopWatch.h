#ifndef ENDE_STOPWATCH_H
#define ENDE_STOPWATCH_H

#include <chrono>

namespace ende::time {

    class StopWatch {
    public:

        StopWatch() = default;

        void start();

        void stop();

        auto reset() -> std::chrono::high_resolution_clock::duration;

        auto running() const -> bool { return _running; }

    private:

        bool _running = false;
        std::chrono::time_point<std::chrono::high_resolution_clock> _started = {};
        std::chrono::high_resolution_clock::duration _passed = {};

    };

}

#endif //ENDE_STOPWATCH_H
