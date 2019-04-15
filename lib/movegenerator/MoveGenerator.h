//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_MOVEGENERATOR_H
#define OSARRACINO_MOVEGENERATOR_H

#include "model/Position.h"
#include "model/Board.h"
#include <unordered_map>
#include <vector>

template <typename T>
class MoveGenerator {
public:
    std::unordered_map<Position, std::vector<Position>, pos_hash> generate(const Board &b) const {
        static_cast<T const&>(*this).generate(b);
    }
};


#endif //OSARRACINO_MOVEGENERATOR_H
