//
// Created by max on 15/04/19.
//

#ifndef OSARRACINO_MINIMAX_H
#define OSARRACINO_MINIMAX_H

#include "evaluator/Evaluator.h"
#include "model/Board.h"
#include "movegenerator/MoveGenerator.h"
#include <vector>

struct Line {
    std::array<std::pair<Position,Position>, 5> move;
    int counter = 0;
};


class Minimax {
public:
    template <typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
    static std::tuple<int, Position, Position> make_decision(int depth,
                                                             const Evaluator<WhiteEvalType> &whiteEval,
                                                             const Evaluator<BlackEvalType> &blackEval,
                                                             const MoveGenerator<MoveGeneratorType> &moveGenerator,
                                                             Board &game_state,
                                                             Line &pline);

    template <typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
    static int minimax(int depth, int max_depth,
                       const Evaluator<WhiteEvalType> &whiteEval,
                       const Evaluator<BlackEvalType> &blackEval,
                       const MoveGenerator<MoveGeneratorType> &moveGenerator,
                       bool maximizingPlayer,
                       Board &game_state, int alpha, int beta, bool leading_white,
                       Line &pline);

    static std::string best_move(Board &b);
};


#endif //OSARRACINO_MINIMAX_H
