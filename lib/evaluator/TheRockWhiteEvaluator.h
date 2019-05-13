//
// Created by freddy on 13/05/19.
//

#ifndef OSARRACINO_THEROCKWHITEEVALUATOR_H
#define OSARRACINO_THEROCKWHITEEVALUATOR_H

#include "Evaluator.h"

const int THEROCK_EVALUATOR_MAX_DEPTH = 2;
const int THEROCK_EVALUATOR_SEARCH_WIN_POTENTIAL_MULTIPLIER = 1000;
const int THEROCK_EVALUATOR_SEARCH_WIN_MULTIPLIER = 200000;
const int THEROCK_EVALUATOR_SEARCH_LOSE_MULTIPLIER = -200000;
const int THEROCK_EVALUATOR_FREE_WINPOINT_MULTIPLIER = 300;
const int THEROCK_EVALUATOR_WHITE_WINPOINT_MULTIPLIER = 500;
const int THEROCK_EVALUATOR_BLACK_WINPOINT_MULTIPLIER = -500;
const int THEROCK_EVALUATOR_WHITE_PAWN_MULTIPLIER = 30;
const int THEROCK_EVALUATOR_BLACK_PAWN_MULTIPLIER = -50;
const int THEROCK_EVALUATOR_BLACK_SURROUNDED_MULTIPLIER = -1000;

class TheRockWhiteEvaluator : public Evaluator<TheRockWhiteEvaluator> {
public:
    int evaluate(const Board &b) const;

    std::string get_name() const {
        return "TheRockWhiteEvaluator";
    }

    int perform_search(const uint16_t *cols, const uint16_t *rows, int depth, int king_col, int king_row,
                       bool horizontal) const;

    int calculate_surrounded_penality(const uint16_t *cols, const uint16_t *rows,
                                      int king_col, int king_row) const;

    int calculate_white_winpoints(const Board &b) const;
    int calculate_black_winpoints(const Board &b) const;
};



#endif //OSARRACINO_THEROCKWHITEEVALUATOR_H
