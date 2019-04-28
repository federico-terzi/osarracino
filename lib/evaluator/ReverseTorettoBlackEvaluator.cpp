//
// Created by freddy on 28/04/19.
//

#include "ReverseTorettoBlackEvaluator.h"
#include "TorettoWhiteEvaluator.h"

int ReverseTorettoBlackEvaluator::evaluate(const Board &b) const {
    TorettoWhiteEvaluator evaluator;
    return -evaluator.evaluate(b);
}
