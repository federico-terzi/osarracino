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
    static std::vector<Action::Position> xorAndRun(Board &b, int col, int row);
    static std::bitset<9> white_mask[9];
    static void init_white_mask();
};


#endif //OSARRACINO_BITMASKHANDLER_H
