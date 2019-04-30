//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_TIMER_H
#define OSARRACINO_TIMER_H

#include <ctime>
#include "time.h"

class Timer {
public:
    Timer(int seconds) : __seconds(seconds) {
        reset();
    }

    inline void reset() {
        clock_gettime(CLOCK_MONOTONIC, &begin_time);

        end_time.tv_sec = begin_time.tv_sec + __seconds;
        end_time.tv_nsec = begin_time.tv_nsec;
    }

    inline float elapsed() {
        float elapsed;

        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);

        elapsed = (now.tv_sec - begin_time.tv_sec);
        elapsed += (now.tv_nsec - begin_time.tv_nsec) / 1000000000.0;

        return elapsed;
    }

    inline bool is_timed_out() {
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return end_time.tv_sec < now.tv_sec;
    }
private:
    int __seconds;
    timespec begin_time;
    timespec end_time;
};


#endif //OSARRACINO_TIMER_H
