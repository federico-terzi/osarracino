//
// Created by max on 15/04/19.
//

#ifndef OSARRACINO_MINIMAX_H
#define OSARRACINO_MINIMAX_H

#include "evaluator/Evaluator.h"
#include "model/Board.h"
#include "movegenerator/MoveGenerator.h"


class Minimax {
public:
    template <typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
    static int minimax(int depth, int max_depth, const Evaluator<WhiteEvalType> &whiteEval,
                       const Evaluator<BlackEvalType> &blackEval,
                       const MoveGenerator<MoveGeneratorType> &moveGenerator, bool maximizingPlayer, Board value,
                       int alpha, int beta, bool leading_white);

    static std::string best_move(Board &b);
};


#endif //OSARRACINO_MINIMAX_H
