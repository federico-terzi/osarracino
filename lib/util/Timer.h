//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_TIMER_H
#define OSARRACINO_TIMER_H

#include <ctime>

class Timer {
public:
    Timer(int seconds) : __seconds(seconds) {
        reset();
    }

    inline void reset() {
        begin_time = clock();
        end_time = begin_time + CLOCKS_PER_SEC * __seconds;
    }

    inline float elapsed() {
        return (clock() - begin_time) / float(CLOCKS_PER_SEC);
    }

    inline bool is_timed_out() {
        return end_time < clock();
    }
private:
    int __seconds;
    clock_t begin_time;
    clock_t end_time;
};


#endif //OSARRACINO_TIMER_H
