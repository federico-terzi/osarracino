//
// Created by max on 30/04/19.
//

#ifndef OSARRACINO_MOVEUTIL_H
#define OSARRACINO_MOVEUTIL_H

#include <vector>
#include <movegenerator/MoveGenerator.h>


class MoveUtil {
public:
    static std::vector<Move> Order_Move(std::vector<Move> &moves, std::vector<Move> &place_it_first);
};


#endif //OSARRACINO_MOVEUTIL_H
