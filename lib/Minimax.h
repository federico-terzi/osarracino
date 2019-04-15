//
// Created by max on 15/04/19.
//

#ifndef OSARRACINO_MINIMAX_H
#define OSARRACINO_MINIMAX_H

#include "Board.h"


class Minimax {
public:
    static int minimax(int depth,
                       bool maximizingPlayer,
                       Board value, int alpha,
                       int beta);
    static std::string best_move(Board &b);
};


#endif //OSARRACINO_MINIMAX_H
