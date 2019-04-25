//
// Created by max on 25/04/19.
//

#ifndef OSARRACINO_DTRANSPOSITIONTABLE_H
#define OSARRACINO_DTRANSPOSITIONTABLE_H

#define DIMENSION 1000

#include <unordered_map>
#include "ZobristHash.h"

enum Flag {
    HASH_EXACT = 1, //Evaluation between alpha and beta
    HASH_BETA = 2, // Evaluation > beta
    HASH_ALFA = 3 // Evaluation < alpha
};

// Value of the transposition table
struct BoardEvaluation {
    int score;
    std::pair<Position, Position> best_move;
    Flag flag;
    uint16_t zobrist_key;
    int depth;
};


class DTranspositionTable {
    ZobristHash zobrist_hash;
    std::unordered_map<int, BoardEvaluation> transposition_table;
public:
    void add_entry(const Board &b, int score,const std::pair<Position, Position> &best_move, Flag flag, int depth);
    void clean_table();
    bool has_entry(const Board &b);
    BoardEvaluation get_entry(const Board &b) const;
};


#endif //OSARRACINO_DTRANSPOSITIONTABLE_H
