#include <Ende/time/StopWatch.h>

void ende::time::StopWatch::start() {
    if (!_running) {
        _running = true;
        _started = std::chrono::high_resolution_clock::now();
    }
}

void ende::time::StopWatch::stop() {
    if (_running) {
        _running = false;
        _passed = std::chrono::high_resolution_clock::now() - _started;
    }
}

auto ende::time::StopWatch::reset() -> std::chrono::high_resolution_clock::duration {
    stop();
    start();
    auto passed = _passed;
    _passed = {};
    return passed;
}