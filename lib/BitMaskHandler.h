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
    static std::bitset<3> black_hor_mask[2];
    static std::bitset<2> black_ver_mask[3];
    static void init_masks();
};


#endif //OSARRACINO_BITMASKHANDLER_H
