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
int hit = 0;

template<typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
int Minimax::minimax(int depth, int max_depth, const Evaluator<WhiteEvalType> &whiteEval,
                     const Evaluator<BlackEvalType> &blackEval,
                     const MoveGenerator<MoveGeneratorType> &moveGenerator, bool maximizingPlayer, Board value,
                     int alpha, int beta, bool leading_white, DTranspositionTable &table) {
    moves++;

    int old_alpha = alpha;
    // Terminating condition. i.e
    // leaf node is reached
    // TODO: valutate the sign of the evaluation based on the turn

    //Setting alpha and beta according to the flag
    if (table.has_entry(value) && table.get_entry(value).depth >= depth) {
        auto myEntry = table.get_entry(value);
        hit++;
        switch (myEntry.flag) {
            case Flag::HASH_EXACT:
                return myEntry.score;
            case Flag::LOWERBOUND:
                alpha = std::max(alpha, myEntry.score);
                break;
            case Flag::UPPERBOUND:
                beta = std::min(beta, myEntry.score);
                break;
        }

        if (alpha >= beta) {
            return myEntry.score;
        }

    }
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
                              true, board, alpha, beta, leading_white, table);

                best = std::max(best, val);
                alpha = std::max(alpha, best);


                BoardEvaluation entry;
                entry.score = val;
                if (val <= old_alpha) {
                    entry.flag = Flag::UPPERBOUND;
                } else if (val >= beta) {
                    entry.flag = Flag::LOWERBOUND;
                } else {
                    entry.flag = Flag::HASH_EXACT;
                }
                entry.depth = depth;
                table.add_entry(entry, value);


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
                              true, board, alpha, beta, leading_white, table);

                best = std::min(best, val);
                beta = std::min(beta, best);

                BoardEvaluation entry;
                entry.score = val;
                if (val <= old_alpha) {
                    entry.flag = Flag::UPPERBOUND;
                } else if (val >= beta) {
                    entry.flag = Flag::LOWERBOUND;
                } else {
                    entry.flag = Flag::HASH_EXACT;
                }
                entry.depth = depth;
                table.add_entry(entry, value);

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
    DTranspositionTable table;
    //ThetaMoveGenerator moveGenerator;

    const clock_t begin_time = clock();

    hit = 0;
    moves = 0;
    // Incremental deepening

    int best_score = 0;
    int final_depth = 0;
    for (int depth = 1; depth <= 5; depth++) {
        best_score = minimax(0, depth, whiteEval, blackEval, moveGenerator, true, b, MIN, MAX, b.is_white, table);
        final_depth = depth;
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
    std::cout << "Hits: " << hit << std::endl;
    std::cout << "Reached depth: " << final_depth << std::endl;

    std::string color = b.is_white ? "WHITE" : "BLACK";

    return std::string(
            "{\"from\":\"" + from.to_move() + "\",\"to\":\"" + to.to_move() + "\",\"turn\":\"" + color + "\"}");
}
