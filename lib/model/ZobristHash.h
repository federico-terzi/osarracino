//
// Created by max on 24/04/19.
//

#ifndef OSARRACINO_ZOBRISTHASH_H
#define OSARRACINO_ZOBRISTHASH_H


#include <cstdint>
#include "Board.h"

class ZobristHash {
public:
    /*INDEXES*/

    const int white_pawn = 0;
    const int black_pawn = 1;
    const int king = 2;

    uint64_t table[81][3];

    ZobristHash();
    uint64_t hash(const Board &b) const;


};


#endif //OSARRACINO_ZOBRISTHASH_H
