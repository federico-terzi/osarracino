//
// Created by max on 15/04/19.
//

#include "Minimax.h"
#include "evaluator/Evaluator.h"
#include <algorithm>
#include <evaluator/WhiteEvalutator.h>
#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ThetaMoveGenerator.h>

// Returns optimal value for
// current player(Initially called
// for root and maximizer)
const int MAX = 100000;
const int MIN = -MAX;

Position from;
Position to;

template <typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
int Minimax::minimax(int depth, const Evaluator<WhiteEvalType> &whiteEval,
                   const Evaluator<BlackEvalType> &blackEval,
                   const MoveGenerator<MoveGeneratorType> &moveGenerator,
                   bool maximizingPlayer,
                   Board value, int alpha,
                   int beta)
{
    int v {blackEval.evaluate(value)};
    int w {whiteEval.evaluate(value)};

    auto moves {moveGenerator.generate(value)};

    return 0;
}

std::string Minimax::best_move(Board &b) {
    WhiteEvalutator whiteEval;
    BlackEvaluator blackEval;
    ThetaMoveGenerator moveGenerator;

    minimax(0, whiteEval, blackEval, moveGenerator, true, b, MIN, MAX);

    return std::string("{\"from\":\""+from.to_move()+"\",\"to\":\""+to.to_move()+"\",\"turn\":\"WHITE\"}");
}
