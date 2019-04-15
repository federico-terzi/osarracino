//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_WHITEEVALUTATOR_H
#define OSARRACINO_WHITEEVALUTATOR_H

#include "Evaluator.h"

class WhiteEvaluator : public Evaluator<WhiteEvaluator>{
public:
    WhiteEvaluator();

    int evaluate(const Board &b) const;



private:
    uint16_t low_mask[9];
    uint16_t high_mask[9];
};


#endif //OSARRACINO_WHITEEVALUTATOR_H
