//
// Created by freddy on 28/04/19.
//

#ifndef OSARRACINO_REVERSETORETTOBLACKEVALUATOR_H
#define OSARRACINO_REVERSETORETTOBLACKEVALUATOR_H

#include "Evaluator.h"

class ReverseTorettoBlackEvaluator : public Evaluator<ReverseTorettoBlackEvaluator> {
public:
    int evaluate(const Board &b) const;

    std::string get_name() const {
        return "ReverseTorettoBlackEvaluator";
    }
};


#endif //OSARRACINO_REVERSETORETTOBLACKEVALUATOR_H
