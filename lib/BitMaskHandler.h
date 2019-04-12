//
// Created by max on 11/04/19.
//

#ifndef OSARRACINO_BITMASKHANDLER_H
#define OSARRACINO_BITMASKHANDLER_H


#include <bitset>
#include <memory>
#include "Board.h"

class BitMaskHandler {
public:
    static std::vector<Action::Position> xorAndRun(std::bitset<9> set[], int col, int row);
};


#endif //OSARRACINO_BITMASKHANDLER_H
