module;

#include <chrono>

export module ende.time.StopWatch;

namespace ende::time {

export class StopWatch {
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

} // namespace ende::time

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
