//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_BLACKEVALUATOR_H
#define OSARRACINO_BLACKEVALUATOR_H

#include "Evaluator.h"

class BlackEvaluator : public Evaluator<BlackEvaluator>{
public:
    int evaluate(const Board &b) const;
};


#endif //OSARRACINO_BLACKEVALUATOR_H
