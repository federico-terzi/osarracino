//
// Created by max on 25/04/19.
//

#include "DTranspositionTable.h"

void DTranspositionTable::clean_table() {

}

void DTranspositionTable::add_entry(const Board &b, int score, const std::pair<Position, Position> &best_move, Flag flag,
                                     int depth) {

    //Now we use always replace as replacement schema.
    auto hash_value = zobrist_hash.hash(b);
    int index = hash_value & DIMENSION;

    if (transposition_table.find(index) != transposition_table.end()) {
        transposition_table[index] = {score, best_move, flag, hash_value, depth};
    } else {
        if (transposition_table[index].depth < depth) {
            transposition_table[index] = {score, best_move, flag, hash_value, depth};
        }
    }

}

BoardEvaluation DTranspositionTable::get_entry(const Board &b) const {
    return transposition_table.at(hash & DIMENSION);
}

bool DTranspositionTable::has_entry(const Board &b) {
    auto hash_value = zobrist_hash.hash(b);
    int index = hash_value & DIMENSION;
    hash = hash_value;

    auto element = transposition_table.find(index);
    if ( element == transposition_table.end()) {
        return false;
    } else {
        return element->second.zobrist_key == hash_value;
    }
}

void DTranspositionTable::add_entry(BoardEvaluation &eval, Board &b) {
    eval.zobrist_key = zobrist_hash.hash(b);
    transposition_table[eval.zobrist_key & DIMENSION] = eval;

}
