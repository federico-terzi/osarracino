//
// Created by max on 12/04/19.
//

#ifndef OSARRACINO_EVALUATOR_H
#define OSARRACINO_EVALUATOR_H


#include "Board.h"

// Consts that represents the weight of the matrix to make the pawns have some kind of formation.
const int PURPLE_WG = 3;
const int CYAN_WG = 2;
const int BLANK_WG = 0;

// Matrix of values. 1 for every cell.
static int matrix[9][9] = {
    {BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG},
    {BLANK_WG, BLANK_WG, PURPLE_WG, BLANK_WG, CYAN_WG, BLANK_WG, PURPLE_WG},
    {BLANK_WG, PURPLE_WG, BLANK_WG, CYAN_WG, BLANK_WG, CYAN_WG, BLANK_WG, PURPLE_WG, BLANK_WG },
    {BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG},
    {BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG},
    {BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG},
    {BLANK_WG, PURPLE_WG, BLANK_WG, CYAN_WG, BLANK_WG, CYAN_WG, BLANK_WG, PURPLE_WG, BLANK_WG},
    {BLANK_WG, BLANK_WG, PURPLE_WG, BLANK_WG, CYAN_WG, BLANK_WG, PURPLE_WG},
    {BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG},

};

class Evaluator {
public:
    static int evaluate_white(Board &b);
    static int evaluate_black(Board &b);
    
    static int eval_formation(Board &b);
    static int evaluate_pawn_opt(Board &b, const Action::Position &pos);
    static bool evaluate_black_win(Board &b);

};


#endif //OSARRACINO_EVALUATOR_H
