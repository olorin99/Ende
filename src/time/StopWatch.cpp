//
// Created by cstro29 on 21/6/21.
//

#include "Ende/time/StopWatch.h"

ende::time::StopWatch::StopWatch()
        : _running(false),
          _started(0),
          _passed(0)
{}


void ende::time::StopWatch::start() {
    if (!_running) {
        _running = true;
        _started = Instant::now();
    }
}

void ende::time::StopWatch::stop() {
    if (_running) {
        _running = false;
        _passed = _passed + _started.elapsed();
    }
}



ende::time::Duration ende::time::StopWatch::reset() {
    stop();
    start();
    Duration passed = _passed;
    _passed = 0;
    return passed;
}


bool ende::time::StopWatch::running() const {
    return _running;
}