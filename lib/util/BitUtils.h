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

namespace BitUtils {
    inline int get_high_moves(uint16_t row, int index) {
        uint16_t select_high = row & high_mask[index];
        if (select_high != 0) {
            return (__builtin_ffs(select_high) - index - 2);
        }else{
            return 8 - index;
        }
    }

    inline int get_low_moves(uint16_t row, int index) {
        uint16_t select_low = row & low_mask[index];
        if (select_low != 0) {
            return (__builtin_clz(select_low) - 32 + index);
        }else{
            return index;
        }
    }

    /*
     * Return 0 if not surrounded, 1 if surrounded from
     * a single side and 2 if surrounded by both sides.
     */
    inline int get_surrounded(uint16_t row, int index) {
        if (index > 0) {
            return __builtin_popcount((row >> (index - 1)) & 0b0000'0101);
        }else{
            return (row & 0b0000'0010) != 0;
        }
    }

    inline int popcount(uint16_t row) {
        return __builtin_popcount(row);
    }

    /*
     * Set the bit to 0 at specified index position of the given mask
     */
    inline void unset_bit(uint16_t &mask, int index) {
        mask = mask & ~(1UL << index);
    }

    /*
     * Set the bit to 1 at specified index position of the given mask
     */
    inline void set_bit(uint16_t &mask, int index) {
        mask = mask | (1UL << index);
    }
};


#endif //OSARRACINO_BITUTILS_H
