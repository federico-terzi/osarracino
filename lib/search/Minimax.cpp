//
// Created by max on 15/04/19.
//

#include "Minimax.h"
#include "evaluator/Evaluator.h"
#include <algorithm>
#include <evaluator/TorettoWhiteEvaluator.h>
#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ThetaMoveGenerator.h>
#include <movegenerator/ArnoldMoveGenerator.h>

// Returns optimal value for
// current player(Initially called
// for root and maximizer)
const int MAX = 100000;
const int MIN = -MAX;

Position from;
Position to;
int moves = 0;

template<typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
int Minimax::minimax(int depth, int max_depth, const Evaluator<WhiteEvalType> &whiteEval,
                     const Evaluator<BlackEvalType> &blackEval,
                     const MoveGenerator<MoveGeneratorType> &moveGenerator, bool maximizingPlayer, Board value,
                     int alpha, int beta, bool leading_white, Line &pline) {
    moves++;

    Line line;
    // Terminating condition. i.e
    // leaf node is reached
    // TODO: valutate the sign of the evaluation based on the turn
    if (depth == max_depth) {
        if (leading_white) {
            return whiteEval.evaluate(value);
        } else {
            return blackEval.evaluate(value);
        }
    }

    /*Populate boards*/

    if (maximizingPlayer) {
        int best = MIN;

        auto moves{moveGenerator.generate(value)};

        for (auto &pawnMoves : moves) {
            for (auto &dest : pawnMoves.second) {
                auto board{Board::from_board(value, pawnMoves.first, dest)};

                int val = minimax(depth + 1, max_depth, whiteEval, blackEval, moveGenerator,
                                  false, board, alpha, beta, leading_white, line);

                best = std::max(best, val);
                alpha = std::max(best, alpha);
                if (val == best) {
                    pline.counter = 0;
                    pline.move[pline.counter++] = {pawnMoves.first, dest};
                    for (int i = 0; i < line.counter; i++) {
                        pline.move[pline.counter++] = line.move[i];
                    }
                }
                if (depth == 0 && best == val) {
                    from = pawnMoves.first;
                    to = dest;
                }

                // Alpha Beta Pruning
                if (beta <= alpha)
                    break;
            }
        }

        return best;
    } else {
        int best = MAX;

        for (auto &pawnMoves : moveGenerator.generate(value)) {
            for (auto &dest : pawnMoves.second) {
                auto board{Board::from_board(value, pawnMoves.first, dest)};

                int val = minimax(depth + 1, max_depth, whiteEval, blackEval, moveGenerator,
                                  true, board, alpha, beta, leading_white, line);

                best = std::min(best, val);
                beta = std::min(beta, best);
                if (val == best) {
                    pline.counter = 0;
                    pline.move[pline.counter++] = {pawnMoves.first, dest};
                    for (int i = 0; i < line.counter; i++) {
                        pline.move[pline.counter++] = line.move[i];
                    }
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
    TorettoWhiteEvaluator whiteEval;
    BlackEvaluator blackEval;
    ArnoldMoveGenerator moveGenerator;
    //ThetaMoveGenerator moveGenerator;

    const clock_t begin_time = clock();

    moves = 0;
    // Incremental deepening


    int best_score = 0;
    int final_depth = 0;
    for (int depth = 1; depth <= 5; depth++) {
        Line main_line;
        best_score = minimax(0, depth, whiteEval, blackEval, moveGenerator, true, b, MIN, MAX, b.is_white, main_line);
        final_depth = depth;
        std::cout << "Iteration:" << depth << std::endl;
        for (int i = 0; i < main_line.counter; i++) {
            std::cout << "From:" << main_line.move[i].first << " To:" << main_line.move[i].second << std::endl;
        }
        if (best_score > 100000) {
            break;
        }
    }

    // Print statistics

    float elapsed = (clock() - begin_time) / float(CLOCKS_PER_SEC);
    float speed = float(moves) / elapsed;
    std::cout << "Evaluation completed. Results: " << std::endl;
    std::cout << "Explored " << moves << " moves in " << elapsed << " seconds " << std::endl;
    std::cout << "Best score: " << best_score << std::endl;
    std::cout << "Speed: " << speed << " moves/second." << std::endl;
    std::cout << "Reached depth: " << final_depth << std::endl;


    std::string color = b.is_white ? "WHITE" : "BLACK";

    return std::string(
            "{\"from\":\"" + from.to_move() + "\",\"to\":\"" + to.to_move() + "\",\"turn\":\"" + color + "\"}");
}
