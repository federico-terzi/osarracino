//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_BITUTILS_H
#define OSARRACINO_BITUTILS_H

#include <cstdint>

const uint16_t low_mask[9] = {
        0b00000000'00000000,
        0b00000000'00000001,
        0b00000000'00000011,
        0b00000000'00000111,
        0b00000000'00001111,
        0b00000000'00011111,
        0b00000000'00111111,
        0b00000000'01111111,
        0b00000000'11111111
};

const uint16_t high_mask[9] = {
        0b11111111'11111110,
        0b11111111'11111100,
        0b11111111'11111000,
        0b11111111'11110000,
        0b11111111'11100000,
        0b11111111'11000000,
        0b11111111'10000000,
        0b11111111'00000000,
        0b11111110'00000000,
};

class BitUtils {
public:
    static int get_high_moves(uint16_t row, int index);
    static int get_low_moves(uint16_t row, int index);

    /*
     * Return 0 if not surrounded, 1 if surrounded from
     * a single side and 2 if surrounded by both sides.
     */
    static int get_surrounded(uint16_t row, int index);
};


#endif //OSARRACINO_BITUTILS_H
