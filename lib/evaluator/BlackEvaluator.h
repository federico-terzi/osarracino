//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_BLACKEVALUATOR_H
#define OSARRACINO_BLACKEVALUATOR_H

#include "Evaluator.h"
#include <vector>
#include <tuple>
#include <array>

class BlackEvaluator : public Evaluator<BlackEvaluator>{
public:
    int evaluate(const Board &b) const;
    static std::pair<std::vector<Position>, std::array<std::bitset<9>, 9>> Get_empty_and_to_move(const Board &b);
};


#endif //OSARRACINO_BLACKEVALUATOR_H
