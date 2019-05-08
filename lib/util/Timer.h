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

    Timer() : Timer(10) {}

    inline void reset() {
        clock_gettime(CLOCK_MONOTONIC, &begin_time);

        end_time.tv_sec = begin_time.tv_sec + __seconds;
        end_time.tv_nsec = begin_time.tv_nsec;
    }

    inline void update_start_time() {
        clock_gettime(CLOCK_MONOTONIC, &begin_time);
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

    inline timespec get_end_time() {
        return end_time;
    }

    inline timespec set_end_time(timespec __end_time) {
        end_time = __end_time;
    }
private:
    int __seconds;
    timespec begin_time;
    timespec end_time;
};


#endif //OSARRACINO_TIMER_H
