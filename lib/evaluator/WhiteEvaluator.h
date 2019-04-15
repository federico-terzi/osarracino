//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_WHITEEVALUTATOR_H
#define OSARRACINO_WHITEEVALUTATOR_H

#include "Evaluator.h"

const int WHITE_EVALUATOR_MAX_DEPTH = 3;
const int WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER = 1000;

class WhiteEvaluator : public Evaluator<WhiteEvaluator> {
public:
    WhiteEvaluator();

    int evaluate(const Board &b) const;

    int perform_search(const uint16_t *cols, const uint16_t *rows, int depth, int king_col, int king_row,
                       bool horizontal) const;

private:
    uint16_t low_mask[9];
    uint16_t high_mask[9];
};


#endif //OSARRACINO_WHITEEVALUTATOR_H
