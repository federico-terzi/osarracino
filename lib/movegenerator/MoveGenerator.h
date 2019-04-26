//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_MOVEGENERATOR_H
#define OSARRACINO_MOVEGENERATOR_H

#include "model/Position.h"
#include "model/Board.h"
#include <vector>

template <typename T>
class MoveGenerator {
public:
    std::vector<std::pair<Position,Position>> generate(const Board &b) const {
        return static_cast<T const&>(*this).generate(b);
    }
};


#endif //OSARRACINO_MOVEGENERATOR_H
