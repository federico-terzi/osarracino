//
// Created by max on 12/04/19.
//

#ifndef OSARRACINO_EVALUATOR_H
#define OSARRACINO_EVALUATOR_H


#include "model/Board.h"

template <typename T>
class Evaluator {
public:
    int evaluate(const Board &b) const {
        static_cast<T const&>(*this).evaluate(b);
    }
};


#endif //OSARRACINO_EVALUATOR_H
