//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_WHITEEVALUTATOR_H
#define OSARRACINO_WHITEEVALUTATOR_H

#include "Evaluator.h"

const int WHITE_EVALUATOR_MAX_DEPTH = 3;
const int WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER = 1000;
const int WHITE_EVALUATOR_FREE_WINPOINT_MULTIPLIER = 50;
const int WHITE_EVALUATOR_WHITE_PAWN_MULTIPLIER = 30;
const int WHITE_EVALUATOR_BLACK_PAWN_MULTIPLIER = -20;

class WhiteEvaluator : public Evaluator<WhiteEvaluator> {
public:
    int evaluate(const Board &b) const;

    int perform_search(const uint16_t *cols, const uint16_t *rows, int depth, int king_col, int king_row,
                       bool horizontal) const;
};


#endif //OSARRACINO_WHITEEVALUTATOR_H
