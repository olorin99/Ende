//
// Created by cstro29 on 21/6/21.
//

#ifndef ANINO_STOPWATCH_H
#define ANINO_STOPWATCH_H

#include <Ende/time/time.h>

namespace ende::time {

    class StopWatch {
    public:

        StopWatch();

        void start();

        void stop();


        Duration reset();


        bool running() const;


    private:

        bool _running;
        Instant _started;
        Duration _passed;


    };

}

#endif //ANINO_STOPWATCH_H
