//
// Created by max on 11/04/19.
//

#ifndef OSARRACINO_BITMASKHANDLER_H
#define OSARRACINO_BITMASKHANDLER_H


#include <bitset>
#include <memory>

class BitMaskHandler {
public:
    std::unique_ptr<std::bitset<9>[]> masks;
    int col;
    int row;
    BitMaskHandler(int col, int row);
    void xorAndRun(std::bitset<9> set[]);
};


#endif //OSARRACINO_BITMASKHANDLER_H
