//
// Created by max on 01/05/19.
//

#include "TranspositionTable.h"

#include <cstring>

TranspositionTable::TranspositionTable() {
    size_t newsize = 1024;
    while (2ULL* newsize * sizeof(TTBucket) <= SIZE_OF_TABLE ) {
        newsize *= 2;
    }
    size = newsize;

    buckets = new TTBucket[size];
}

TranspositionTable::~TranspositionTable() {
    delete [] buckets;
}

void TranspositionTable::clear() {
    memset(buckets, 0, size);
}

void TranspositionTable::store(const Board &b, uint8_t depth, int score, Flags flag) {
    uint32_t key = this->get_key(b) >> 32;
    TTEntry * first = this->get_first(b);
    TTEntry * to_replace = first;

    for (int i = 0; i < BucketSize; i++, first++){ //For every element in the bucket
        //If the key is not set or the key is the same
        //Place the entry
        if (!(first->key) || first->key == key) {
            //Storing the entry
            first->key = key;
           // first->move = move;
            first->depth = depth;
            first->flag = flag;
            first->score = score;
            return; //Job done, we return
        }

        //Check which one we have to replace
        if (first->depth < to_replace->depth) {
            to_replace = first;
        }
    }
    // None of the buckets had the same hash, so we replace the one at highest depth
    to_replace->key = key;
  //  to_replace->move = move;
    to_replace->depth = depth;
    to_replace->flag = flag;
    to_replace->score = score;
}

TTEntry *TranspositionTable::get(const Board &b) {
    TTEntry * first = get_first(b);

    for (int i = 0; i < BucketSize; i++, first++) {
        if (first->key == (this->get_key(b) & 0xFFFF)) {
            return first;
        }
    }
    return NULL;
}

