//
// Created by freddy on 15/04/19.
//

#include "BitUtils.h"

int BitUtils::get_high_moves(uint16_t row, int index) {
    uint16_t select_high = row & high_mask[index];
    if (select_high != 0) {
        return (__builtin_ffs(select_high) - index - 2);
    }else{
        return 8 - index;
    }
}

int BitUtils::get_low_moves(uint16_t row, int index) {
    uint16_t select_low = row & low_mask[index];
    if (select_low != 0) {
        return (__builtin_clz(select_low) - 32 + index);
    }else{
        return index;
    }
}
