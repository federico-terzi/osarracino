//
// Created by max on 15/04/19.
//

#include "Minimax.h"
#include "Evaluator.h"
#include "MoveCalculator.h"
#include <algorithm>

// Returns optimal value for
// current player(Initially called
// for root and maximizer)
const int MAX = 100000;
const int MIN = -MAX;

Action::Position from;
Action::Position to;

int Minimax::minimax(int depth,
                     bool maximizingPlayer,
                     Board value, int alpha,
                     int beta)
{

    // Terminating condition. i.e
    // leaf node is reached
    if (depth == 3)
        value.is_white ? Evaluator::evaluate_white(value): Evaluator::evaluate_black(value);

    /*Populate boards*/


    //TODO: INIT MOVE CALCULATOR MASKS!!!!!
    if (maximizingPlayer)
    {
        int best = MIN;

        // Recur for left and
        // right children
        for (auto &move : value.to_be_moved) {
            for (auto &dest : MoveCalculator::Get_All_Moves(value, move)) {
                auto board  {Board::from_board(value, move, dest)};

                int val = minimax(depth + 1,
                                  false, board, alpha, beta);

                best = std::max(best, val);
                alpha = std::max(alpha, best);

                if(best == val) {
                    from = move;
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

        for (auto &move : value.to_be_moved) {
            for (auto &dest : MoveCalculator::Get_All_Moves(value, move)) {
                auto board  {Board::from_board(value, move, dest)};

                int val = minimax(depth + 1,
                                  true, board, alpha, beta);

                best = std::min(best, val);
                alpha = std::min(alpha, best);

                if(best == val) {
                    from = move;
                    to = dest;
                }

                // Alpha Beta Pruning
                if (beta <= alpha)
                    break;
            }
        }
        return best;
    }
}

std::string Minimax::best_move(Board &b) {
    MoveCalculator::init_masks();
    minimax(0, true, b, MIN, MAX);

    return std::string("{\"from\":\""+from.to_move()+"\",\"to\":\""+to.to_move()+"\",\"turn\":\"WHITE\"}");
}
