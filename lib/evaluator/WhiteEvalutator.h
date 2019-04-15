//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_WHITEEVALUTATOR_H
#define OSARRACINO_WHITEEVALUTATOR_H

#include "Evaluator.h"

class WhiteEvalutator : public Evaluator<WhiteEvalutator>{
public:
    int evaluate(const Board &b) const;
};


#endif //OSARRACINO_WHITEEVALUTATOR_H
