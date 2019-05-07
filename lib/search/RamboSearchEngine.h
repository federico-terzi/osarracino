//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_RAMBOSEARCHENGINE_H
#define OSARRACINO_RAMBOSEARCHENGINE_H

#include <limits>
#include <algorithm>
#include <functional>
#include <vector>
#include <string.h>
#include "SearchEngine.h"
#include <evaluator/Evaluator.h>

const int RAMBO_MAX_DEPTH = 20;

//#define ENABLE_ADVANCED_TRACING

struct MoveTrace {
    Move move;
    bool maximizing;
    int score;
    int depth;
    bool valid = false;

    friend std::ostream &operator<<(std::ostream &s, const MoveTrace &trace) {
        s << "TRACE: maximizing: " << trace.maximizing << " with score: " << trace.score
        << " at depth " << trace.depth << " for move: " << trace.move;
        return s;
    };
};

struct MoveConfiguration {
    Move move;
    Board board;
    int score;
    int depth;
    std::array<MoveTrace, RAMBO_MAX_DEPTH> move_traces;
};

class RamboSearchEngine : public SearchEngine<RamboSearchEngine> {
public:
    std::array<MoveTrace, RAMBO_MAX_DEPTH> move_traces;

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

#ifdef ENABLE_ADVANCED_TRACING
                move_traces[depth-1] = {move, maximizing_player, value, depth-1, true};
#endif

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

#ifdef ENABLE_ADVANCED_TRACING
                move_traces[depth-1] = {move, maximizing_player, value, depth-1, true};
#endif

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

#ifdef ENABLE_ADVANCED_TRACING
            // Reset move traces array.
            move_traces = std::array<MoveTrace, RAMBO_MAX_DEPTH>();
#endif

            for (auto &state: future_states) {
                int value = minimax(current_depth_limit, eval, move_generator, false, state.board,
                                    std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

#ifdef ENABLE_ADVANCED_TRACING
                move_traces[current_depth_limit] = {state.move, true, value, current_depth_limit, true};
#endif

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

#ifdef ENABLE_ADVANCED_TRACING
            std::cout << "Possible moves: " << std::endl;
            for (auto &state: future_states) {
                std::cout << state.move << " with score: " << state.score << " at depth: " << state.depth <<std::endl;
            }

            std::cout<< "Move tracing for best move:" << std::endl;
            for (int i = 0; best_state.move_traces[i].valid; i++) {
                std::cout << best_state.move_traces[i] << std::endl;
            }
#endif


        return best_state.move;
    }
};


#endif //OSARRACINO_RAMBOSEARCHENGINE_H
