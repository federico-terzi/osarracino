//
// Created by max on 01/05/19.
//

#ifndef OSARRACINO_TRANSPOSITIONTABLE_H
#define OSARRACINO_TRANSPOSITIONTABLE_H


#include <cstdint>
#include <movegenerator/MoveGenerator.h>
#include "ZobristHash.h"

struct TTEntry {
    int16_t score;
    uint64_t key;
    Move move;
    uint8_t depth;

};

const int BucketSize = 4;

struct TTBucket {
    TTEntry data[BucketSize];
};

const size_t UNITY = 1 << 20; // 1 MB
const size_t SIZE_OF_TABLE = 256 * UNITY; //256 * MB;

class TranspositionTable {
public:

    TranspositionTable();
    ~TranspositionTable();
    void store(const Board &b, const Move &move,uint8_t depth ,int score);
    TTEntry * get(const Board &b);
    void clear();

    inline uint64_t get_key(const Board &b) {
        return zobrist.hash(b);
    }

    inline TTEntry * get_first(const Board &b) {
        return buckets[this->get_key(b) & (size -1)].data;
    }

private:
    ZobristHash zobrist;
    size_t size;
    TTBucket * buckets;
};

#endif //OSARRACINO_TRANSPOSITIONTABLE_H
