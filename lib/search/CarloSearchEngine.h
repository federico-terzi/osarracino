//
// Created by max on 08/05/19.
//

#ifndef OSARRACINO_CARLOSEARCHENGINE_H
#define OSARRACINO_CARLOSEARCHENGINE_H

#include <limits>
#include <algorithm>
#include <functional>
#include <vector>
#include <string.h>
#include "SearchEngine.h"
#include <evaluator/Evaluator.h>

//#define ENABLE_ADVANCED_TRACING

const int CARLO_MAX_DEPTH = 20;

const int QUIESCENCE_DEPTH = 2;

struct CarloMoveTrace {
    Move move;
    bool maximizing;
    int score;
    int depth;
    bool valid = false;

    friend std::ostream &operator<<(std::ostream &s, const CarloMoveTrace &trace) {
        s << "TRACE: maximizing: " << trace.maximizing << " with score: " << trace.score
          << " at depth " << trace.depth << " for move: " << trace.move;
        return s;
    };
};

struct CarloMoveConfiguration {
    Move move;
    Board board;
    int score;
    int depth;
    std::array<CarloMoveTrace, CARLO_MAX_DEPTH> move_traces;
};

class CarloSearchEngine : public SearchEngine<CarloSearchEngine> {
public:
    std::string get_name() const {
        return "CarloSearchEngine";
    }

    std::array<CarloMoveTrace, CARLO_MAX_DEPTH> move_traces;

    template<typename EvalType, typename MoveGeneratorType>
    int quiescence_search(const Board &game_state, const MoveGenerator<MoveGeneratorType> &move_generator,
                          const Evaluator<EvalType> &eval,
                          bool maximizing_player, int alpha, int beta, int depth) {
        quiet_count++;

        if (maximizing_player) { //Maximizing player
            int stand_pat = eval.evaluate(game_state);
            if (depth == 0 || game_state.is_white_win() || game_state.is_white_win() || timer.is_timed_out()) {
                return stand_pat;
            }
            if (stand_pat >= beta) {
                return beta;
            }
            if (alpha < stand_pat) {
                alpha = stand_pat;
            }

            auto moves = move_generator.generate(game_state);
            int value = std::numeric_limits<int>::min();

            for (const auto &move : moves) {
                auto new_board{Board::from_board(game_state, move.from, move.to)};
                value = std::max(value, quiescence_search(new_board,move_generator, eval ,false, alpha, beta, depth-1));


                if (value >= beta) {
                    return value;
                }
                alpha = std::max(alpha, value);
            }

            return value;

        } else { //Minimizing player

            int stand_pat = eval.evaluate(game_state);
            if(depth == 0 || game_state.is_white_win() || game_state.is_black_win() || timer.is_timed_out()) {
                return stand_pat;
            }
            if(stand_pat <= alpha) {
                return alpha;
            }
            if(beta > stand_pat) {
                beta = stand_pat;
            }
            auto moves = move_generator.generate(game_state);

            int value = std::numeric_limits<int>::max();

            for (const auto &move : moves) {
                auto new_board{Board::from_board(game_state, move.from, move.to)};
                value = std::min(value, quiescence_search(new_board, move_generator, eval, true, alpha, beta, depth-1));

                if (value <= alpha) {
                    return value;
                }

                beta = std::min(beta, value);
            }

            return value;

        }

    }

    template<typename EvalType, typename MoveGeneratorType>
    int minimax(int depth, const Evaluator<EvalType> &eval,
                const MoveGenerator<MoveGeneratorType> &move_generator,
                bool maximizing_player,
                const Board &board, int alpha, int beta, int quiet_search) {
        move_count++;

        if (depth == 0 || timer.is_timed_out() || board.is_black_win() || board.is_white_win()) {
            if (!board.is_quiet && !timer.is_timed_out() && !board.is_black_win() && !board.is_white_win()) {
                return quiescence_search(board, move_generator, eval,maximizing_player,alpha, beta, quiet_search);
            } else {
                return eval.evaluate(board);
            }

        }

        auto moves{move_generator.generate(board)};

        if (maximizing_player) {
            int value = std::numeric_limits<int>::min();

            for (const auto &move : moves) {
                auto new_board{Board::from_board(board, move.from, move.to)};
                value = std::max(value, minimax(depth - 1, eval, move_generator, false, new_board, alpha, beta, QUIESCENCE_DEPTH));


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
                value = std::min(value, minimax(depth - 1, eval, move_generator, true, new_board, alpha, beta, QUIESCENCE_DEPTH));


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

        std::vector<CarloMoveConfiguration> future_states;
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

            std::cout << "Searched " << quiet_count << " with quiescence" << std::endl;
            quiet_count = 0;
            // Reorder the moves based on the score
            std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
                return s1.score > s2.score;
            });


            for (auto &state: future_states) {
                int value = minimax(current_depth_limit, eval, move_generator, false, state.board,
                                    std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 2);


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
        } while (current_depth_limit <= CARLO_MAX_DEPTH && !force_exit && !timer.is_timed_out());

        if (timer.is_timed_out()) {
            std::cout << "TIMED OUT" << std::endl;
        }

        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

        auto best_state = future_states[0];

        std::cout << "Best score: " << best_state.score << std::endl;
        std::cout << "Reached depth: " << current_depth_limit-1 << std::endl;


        return best_state.move;
    }
};

#endif //OSARRACINO_CARLOSEARCHENGINE_H
