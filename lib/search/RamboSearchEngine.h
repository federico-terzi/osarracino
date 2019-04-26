//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_RAMBOSEARCHENGINE_H
#define OSARRACINO_RAMBOSEARCHENGINE_H

#include <limits>
#include <algorithm>
#include <functional>
#include "SearchEngine.h"
#include <evaluator/Evaluator.h>

const int RAMBO_MAX_ORDERING_DEPTH = 3;

struct MoveConfiguration {
    Move move;
    Board board;
    int score;
};

class RamboSearchEngine : public SearchEngine<RamboSearchEngine> {
public:
    template<typename EvalType, typename MoveGeneratorType>
    int minimax(int depth, const Evaluator<EvalType> &eval,
                const MoveGenerator<MoveGeneratorType> &move_generator,
                bool maximizing_player,
                const Board &board, int alpha, int beta) {
        move_count++;

        if (depth == 0 || timer.is_timed_out() || board.is_black_win() || board.is_white_win()) {
            return eval.evaluate(board);
        }

        auto moves{move_generator.generate(board)};

        if (maximizing_player) {
            int value = std::numeric_limits<int>::min();

            for (const auto &move : moves) {
                auto new_board{Board::from_board(board, move.from, move.to)};
                value = std::max(value, minimax(depth - 1, eval, move_generator, false, new_board, alpha, beta));
                if (value >= beta) {
                    return value;
                }
                alpha = std::max(alpha, value);
            }

            return value;
        } else {
            int value = std::numeric_limits<int>::max();

            for (const auto &move : moves) {
                auto new_board{Board::from_board(board, move.from, move.to)};
                value = std::min(value, minimax(depth - 1, eval, move_generator, true, new_board, alpha, beta));

                if (value <= alpha) {
                    return value;
                }

                beta = std::min(beta, value);
            }

            return value;
        }
    }

    template<typename EvalType, typename MoveGeneratorType>
    int ordered_minimax(int depth, const Evaluator<EvalType> &eval,
                const MoveGenerator<MoveGeneratorType> &move_generator,
                bool maximizing_player,
                const Board &board, int alpha, int beta) {
        move_count++;

        if (depth == 0 || timer.is_timed_out() || board.is_black_win() || board.is_white_win()) {
            return eval.evaluate(board);
        }

        auto moves{move_generator.generate(board)};

        std::vector<MoveConfiguration> future_states;
        for (const auto &move : moves) {
            auto new_board{Board::from_board(board, move.from, move.to)};

            int score = eval.evaluate(new_board);

            future_states.push_back({move, new_board, score});
        }

        if (maximizing_player) {
            std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
                return s1.score > s2.score;
            });

            int value = std::numeric_limits<int>::min();

            for (const auto &state : future_states) {
                value = std::max(value, ordered_minimax(depth - 1, eval, move_generator, false, state.board, alpha, beta));
                if (value >= beta) {
                    return value;
                }
                alpha = std::max(alpha, value);
            }

            return value;
        } else {
            std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
                return s1.score < s2.score;
            });

            int value = std::numeric_limits<int>::max();

            for (const auto &state : future_states) {
                value = std::min(value, ordered_minimax(depth - 1, eval, move_generator, true, state.board, alpha, beta));

                if (value <= alpha) {
                    return value;
                }

                beta = std::min(beta, value);
            }

            return value;
        }
    }

    template<typename EvalType, typename MoveGeneratorType>
    Move __make_decision_internal(const Board &b,
                                  const Evaluator<EvalType> &eval,
                                  const MoveGenerator<MoveGeneratorType> &move_generator) {
        int best_score = std::numeric_limits<int>::min();
        Move best_move;

        auto moves{move_generator.generate(b)};

        std::vector<MoveConfiguration> future_states;
        for (const auto &move : moves) {
            auto board{Board::from_board(b, move.from, move.to)};

            int score = eval.evaluate(board);

            future_states.push_back({move, board, score});
        }

        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

        int current_depth_limit = 0;
        bool force_exit = false;
        do {
            std::cout << "Searching depth: " << current_depth_limit << std::endl;

            // TODO: ordering

            for (const auto &state: future_states) {
                int value = ordered_minimax(current_depth_limit, eval, move_generator, false, state.board,
                                    std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

                if (value > best_score) {
                    best_move = state.move;
                    best_score = value;
                }

                std::cout << value << " " << state.move << std::endl;

                if (value > 100000) {
                    std::cout << "Stopping evaluation with winning move: " << state.move << " at depth: "
                              << current_depth_limit << std::endl;
                    force_exit = true;
                    break;
                }
            }

            current_depth_limit++;
        } while (current_depth_limit <= 4 && !force_exit && !timer.is_timed_out());

        if (timer.is_timed_out()) {
            std::cout << "TIMED OUT" << std::endl;
        }
        std::cout << "Best score: " << best_score << std::endl;
        std::cout << "Reached depth: " << current_depth_limit << std::endl;

        return best_move;
    }
};


#endif //OSARRACINO_RAMBOSEARCHENGINE_H
