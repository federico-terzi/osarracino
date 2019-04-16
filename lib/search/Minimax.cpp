//
// Created by max on 15/04/19.
//

#include "Minimax.h"
#include "evaluator/Evaluator.h"
#include <algorithm>
#include <evaluator/WhiteEvaluator.h>
#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ThetaMoveGenerator.h>

// Returns optimal value for
// current player(Initially called
// for root and maximizer)
const int MAX = 100000;
const int MIN = -MAX;

Position from;
Position to;
int moves = 0;

template <typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
int Minimax::minimax(int depth, const Evaluator<WhiteEvalType> &whiteEval, const Evaluator<BlackEvalType> &blackEval,
                     const MoveGenerator<MoveGeneratorType> &moveGenerator, bool maximizingPlayer, Board value,
                     int alpha,
                     int beta, bool leading_white)
{
    moves++;

    // Terminating condition. i.e
    // leaf node is reached
    // TODO: valutate the sign of the evaluation based on the turn
    if (depth == 5)
        if (leading_white) {
            return value.is_white ? whiteEval.evaluate(value): -blackEval.evaluate(value);
        }else{
            return value.is_white ? -whiteEval.evaluate(value): blackEval.evaluate(value);
        }

    /*Populate boards*/

    if (maximizingPlayer)
    {
        int best = MIN;

        auto moves { moveGenerator.generate(value)};

        for (auto &pawnMoves : moves) {
            for (auto &dest : pawnMoves.second) {
                auto board {Board::from_board(value, pawnMoves.first, dest)};

                int val = minimax(depth + 1, whiteEval, blackEval, moveGenerator,
                                  false, board, alpha, beta, leading_white);

                best = std::max(best, val);
                alpha = std::max(alpha, best);

                if(best == val && depth == 0) {
                    from = pawnMoves.first;
                    to = dest;
                }

                // Alpha Beta Pruning
                if (beta <= alpha)
                    break;
            }
        }

        return best;
    }
    else
    {
        int best = MAX;

        for (auto &pawnMoves : moveGenerator.generate(value)) {
            for (auto &dest : pawnMoves.second) {
                auto board {Board::from_board(value, pawnMoves.first, dest)};

                int val = minimax(depth + 1, whiteEval, blackEval, moveGenerator,
                                  true, board, alpha, beta, leading_white);

                best = std::min(best, val);
                beta = std::min(alpha, best);

                // Alpha Beta Pruning
                if (beta <= alpha)
                    break;
            }
        }

        return best;
    }
}

std::string Minimax::best_move(Board &b) {
    WhiteEvaluator whiteEval;
    BlackEvaluator blackEval;
    ThetaMoveGenerator moveGenerator;

    const clock_t begin_time = clock();

    moves = 0;
    minimax(0, whiteEval, blackEval, moveGenerator, true, b, MIN, MAX, b.is_white);



    std::cout << "Explored "<<moves << " moves in " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;
    std::string color = b.is_white ? "WHITE" : "BLACK";

    return std::string("{\"from\":\""+from.to_move()+"\",\"to\":\""+to.to_move()+"\",\"turn\":\""+color+"\"}");
}
