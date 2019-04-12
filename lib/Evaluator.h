//
// Created by max on 12/04/19.
//

#ifndef OSARRACINO_EVALUATOR_H
#define OSARRACINO_EVALUATOR_H


#include "Board.h"

class Evaluator {
public:
    static int evaluate_white(Board &b);
    static int evaluate_black(Board &b);
    static int evaluate_pawn_opt(Board &b, const Action::Position &pos);
    static bool evaluate_black_win(Board &b);

};


#endif //OSARRACINO_EVALUATOR_H
