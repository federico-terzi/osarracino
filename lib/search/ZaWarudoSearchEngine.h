//
// Created by max on 30/04/19.
//

#ifndef OSARRACINO_ZAWARUDOSEARCHENGINE_H
#define OSARRACINO_ZAWARUDOSEARCHENGINE_H

#include <limits>
#include <algorithm>
#include <functional>
#include <vector>
#include <string.h>
#include "SearchEngine.h"
#include <evaluator/Evaluator.h>
#include <util/MoveUtil.h>

const int ZAWARUDO_DEPTH_LIMIT = 20;

//#define ENABLE_ADVANCED_TRACING


class ZaWarudoSearchEngine : public SearchEngine<ZaWarudoSearchEngine> {
public:
    std::array<MoveTrace, RAMBO_MAX_DEPTH> move_traces;
    std::unordered_map<int, std::vector<Move>> killer_moves;

    ZaWarudoSearchEngine() {
        for (int i = 0; i < ZAWARUDO_DEPTH_LIMIT; i++) {
            killer_moves.emplace(i, std::vector<Move>());
        }
    }

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

        //Ho delle mosse a quella depth da poter ordinare, allora lo faccio!

        // MoveUtil::Order_Move(moves, killer_moves.at(depth));


        if (maximizing_player) {
            int value = std::numeric_limits<int>::min();

            for (const auto &move : moves) {
                auto new_board{Board::from_board(board, move.from, move.to)};
                value = std::max(value, minimax(depth - 1, eval, move_generator, false, new_board, alpha, beta));

                if constexpr (enable_advanced_tracing) {
                    move_traces[depth-1] = {move, maximizing_player, value, depth-1, true};
                }

                if (value >= beta) {
                    //Add killer_move
                    killer_moves[depth].push_back({move.from, move.to});
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

                if constexpr (enable_advanced_tracing) {
                    move_traces[depth-1] = {move, maximizing_player, value, depth-1, true};
                }

                if (value <= alpha) {
                    //Add killer_move
                    killer_moves[depth].push_back({move.from, move.to});
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

        auto moves{move_generator.generate(b)};

        std::vector<MoveConfiguration> future_states;
        for (const auto &move : moves) {
            auto board{Board::from_board(b, move.from, move.to)};

            int score = eval.evaluate(board);

            future_states.push_back({move, board, score});
        }

        int current_depth_limit = 0;
        bool force_exit = false;
        do {
            std::cout << "Searching depth: " << current_depth_limit << ". Explored " << move_count << " moves in "
                      << timer.elapsed() << " s" << std::endl;

            // Reorder the moves based on the score
            std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
                return s1.score > s2.score;
            });

            if constexpr (enable_advanced_tracing) {
                // Reset move traces array.
                move_traces = std::array<MoveTrace, RAMBO_MAX_DEPTH>();
            }

            for (auto &state: future_states) {
                int value = minimax(current_depth_limit, eval, move_generator, false, state.board,
                                    std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

                if constexpr (enable_advanced_tracing) {
                    move_traces[current_depth_limit] = {state.move, true, value, current_depth_limit, true};
                }

                if (timer.is_timed_out()) {
                    break;
                }

                // Update future score based on the minmax search
                state.score = value;
                state.depth = current_depth_limit;
                state.move_traces = move_traces;

                if (value > 100000) {
                    std::cout << "Stopping evaluation with winning move: " << state.move << " at depth: "
                              << current_depth_limit << std::endl;
                    force_exit = true;
                    break;
                }
            }

            current_depth_limit++;
        } while (current_depth_limit <= RAMBO_MAX_DEPTH && !force_exit && !timer.is_timed_out());

        if (timer.is_timed_out()) {
            std::cout << "TIMED OUT" << std::endl;
        }

        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

        auto best_state = future_states[0];

        std::cout << "Best score: " << best_state.score << std::endl;
        std::cout << "Reached depth: " << current_depth_limit-1 << std::endl;

        if constexpr (enable_advanced_tracing) {
            std::cout << "Possible moves: " << std::endl;
            for (auto &state: future_states) {
                std::cout << state.move << " with score: " << state.score << " at depth: " << state.depth <<std::endl;
            }

            std::cout<< "Move tracing for best move:" << std::endl;
            for (int i = 0; best_state.move_traces[i].valid; i++) {
                std::cout << best_state.move_traces[i] << std::endl;
            }
        }

        killer_moves.clear();
        return best_state.move;
    }
};


#endif //OSARRACINO_ZAWARUDOSEARCHENGINE_H
