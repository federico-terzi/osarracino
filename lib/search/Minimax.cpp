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
const int MAX = 1000000;
const int MIN = -MAX;

Position from;
Position to;
int move_count = 0;
bool heuristic_used = false;

struct MoveConfiguration {
    Move move;
    Board board;
    int score;
};

template<typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
int Minimax::minimax(int depth, int max_depth, const Evaluator<WhiteEvalType> &whiteEval,
                     const Evaluator<BlackEvalType> &blackEval,
                     const MoveGenerator<MoveGeneratorType> &moveGenerator, bool maximizingPlayer, const Board& value,
                     int alpha, int beta, bool leading_white, int shallow_score) {
    move_count++;

    // Terminating condition. i.e
    // leaf node is reached
    // TODO: pass only the correct valuator to the minmax
    if (depth == max_depth) {
        heuristic_used = true;
        return shallow_score;
    }

    if (value.is_black_win()) {
        if (leading_white) {
            return -200000;
        }else{
            return 200000;
        }
    }else if (value.is_white_win()) {
        if (leading_white) {
            return 200000;
        }else{
            return -200000;
        }
    }

    // Generate moves and future configuration
    auto moves {moveGenerator.generate(value)};
    std::vector<MoveConfiguration> future_states;
    for (const auto& move : moves) {
        auto board {Board::from_board(value, move.from, move.to)};

        // TODO: change passing only one evaluator to minmax
        int score;
        if (leading_white) {
            score = whiteEval.evaluate(board);
        } else {
            score = blackEval.evaluate(board);
        }

        future_states.push_back({move, board, score});
    }

    /*Populate boards*/

    if (maximizingPlayer) {
        int best = MIN;

        // Order the moves
        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });


        for (auto &state : future_states) {
            // TODO: use the cached score instead of calling if needed
            int val = minimax(depth + 1, max_depth, whiteEval, blackEval, moveGenerator,
                              false, state.board, alpha, beta, leading_white, state.score);

            best = std::max(best, val);
            alpha = std::max(alpha, best);

            if (depth == 0 && best == val) {
                from = state.move.from;
                to = state.move.to;
            }

            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
        }

        return best;
    } else {
        int best = MAX;

        // Order the moves
        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score < s2.score;
        });

        for (auto &state : future_states) {
            int val = minimax(depth + 1, max_depth, whiteEval, blackEval, moveGenerator,
                              true, state.board, alpha, beta, leading_white, state.score);

            best = std::min(best, val);
            beta = std::min(beta, best);

            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
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

    heuristic_used = false;

    move_count = 0;
    // Incremental deepening

    int best_score = 0;
    int max_depth = 1;
    Position best_from;
    Position best_to;

    do {
        auto moves {moveGenerator.generate(b)};
        std::vector<MoveConfiguration> future_states;
        for (const auto& move : moves) {
            auto board {Board::from_board(b, move.from, move.to)};

            // TODO: change passing only one evaluator to minmax
            int score;
            if (b.is_white) {
                score = whiteEval.evaluate(board);
            } else {
                score = blackEval.evaluate(board);
            }

            future_states.push_back({move, board, score});
        }

        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

        for (auto &state : future_states) {
            // TODO: use the cached score instead of calling if needed
            int val = minimax(0, max_depth, whiteEval, blackEval, moveGenerator,
                              false, state.board, MIN, MAX, b.is_white, state.score);

            if (val > best_score) {
                best_from = state.move.from;
                best_to = state.move.to;
                best_score = val;
            }

            // Update score
            state.score = val;
        }

        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

        if (future_states[0].board.is_white_win() || future_states[0].board.is_black_win()) {
            break;
        }

        max_depth++;
    }while(max_depth <= 4 || heuristic_used);

    // Print statistics

    float elapsed = (clock() - begin_time) / float(CLOCKS_PER_SEC);
    float speed = float(move_count) / elapsed;
    std::cout << "Evaluation completed. Results: " << std::endl;
    std::cout << "Explored " << move_count << " moves in " << elapsed << " seconds " << std::endl;
    std::cout << "Best score: " << best_score << std::endl;
    std::cout << "Speed: " << speed << " moves/second." << std::endl;
    //std::cout << "Reached depth: " << final_depth << std::endl;

    std::string color = b.is_white ? "WHITE" : "BLACK";

    return std::string(
            "{\"from\":\"" + best_from.to_move() + "\",\"to\":\"" + best_to.to_move() + "\",\"turn\":\"" + color + "\"}");
}
