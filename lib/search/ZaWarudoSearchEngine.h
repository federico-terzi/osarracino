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
#include <model/TranspositionTable.h>

//#define ENABLE_ADVANCED_TRACING


class ZaWarudoSearchEngine : public SearchEngine<ZaWarudoSearchEngine> {
public:
    TranspositionTable table;
    std::array<MoveTrace, RAMBO_MAX_DEPTH> move_traces;

    template<typename EvalType, typename MoveGeneratorType>
    int minimax(int depth, int maxdepth,const Evaluator<EvalType> &eval,
                const MoveGenerator<MoveGeneratorType> &move_generator,
                bool maximizing_player,
                const Board &board, int alpha, int beta) {
        move_count++;


        TTEntry * entry = table.get(board);
        if (entry != NULL && entry->depth == depth) {
            switch (entry->flag) {
                case Flags::EXACT:
                    return entry->score;
                case Flags::UPPERBOUND:
                    if (entry->score > alpha) {
                        alpha = entry->score;
                    }
                    break;
                case Flags::LOWERBOUND:
                    if (entry->score < beta) {
                        beta = entry->score;
                    }
                    break;
            }
            if (alpha >= beta) {
                return entry->score;
            }
        }

        if (depth >= maxdepth || timer.is_timed_out() || board.is_black_win() || board.is_white_win()) {
            return eval.evaluate(board);
        }


        auto moves{move_generator.generate(board)};

        if (maximizing_player) {
            int value = std::numeric_limits<int>::min();  // DA DEFINIRE
            for (const auto &move: moves) {
                auto new_game_state = Board::from_board(board, move.from, move.to);
                value = std::max(value, minimax(depth + 1, maxdepth,eval, move_generator, false, new_game_state, alpha, beta));

                if (value >= beta) {
                    return value;
                }
                alpha = std::max(alpha, value);

            }

            if (value <= alpha) {
                table.store(board, depth, value, Flags::LOWERBOUND);
            } else if (value >= beta) {
                table.store(board, depth, value, Flags::UPPERBOUND);
            } else {
                table.store(board, depth, value, Flags::EXACT);
            }

            return value;

        } else {
            int value = std::numeric_limits<int>::max();
            for (const auto &move: moves){
                auto new_game_state = Board::from_board(board, move.from, move.to);
                value = std::min(value, minimax(depth+1, maxdepth ,eval, move_generator, true, new_game_state, alpha, beta));

                if (value <= alpha) { // AIMA CUTOFF
                    return value;
                }

                beta = std::min(beta, value);
            }

            if (value <= alpha) {
                table.store(board, depth, value, Flags::LOWERBOUND);
            } else if (value >= beta) {
                table.store(board, depth, value, Flags::UPPERBOUND);
            } else {
                table.store(board, depth, value, Flags::EXACT);
            }

            return value;
        }
    }

    template<typename EvalType, typename MoveGeneratorType>
    Move __make_decision_internal(const Board &b,
                                  const Evaluator<EvalType> &eval,
                                  const MoveGenerator<MoveGeneratorType> &move_generator) {

        table.clear();
        auto moves{move_generator.generate(b)};

        std::vector<MoveConfiguration> future_states;
        for (const auto &move : moves) {
            auto board{Board::from_board(b, move.from, move.to)};

            future_states.push_back({move, board, 0});
        }

        int current_depth_limit = 1;
        bool force_exit = false;
        do {
            std::cout << "Searching depth: " << current_depth_limit << ". Explored " << move_count << " moves in "
                      << timer.elapsed() << " s" << std::endl;

            std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
                return s1.score > s2.score;
            });

            for (auto &state: future_states) {
                int value = minimax(1, current_depth_limit, eval, move_generator, false, state.board,
                                    std::numeric_limits<int>::min(), std::numeric_limits<int>::max());


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

        auto best_state = future_states[0];

        std::cout << "Best score: " << best_state.score << std::endl;
        std::cout << "Reached depth: " << current_depth_limit-1 << std::endl;


        /*O si modifica l'interfaccia o tocca fare così*/
        return best_state.move;
    }
};


#endif //OSARRACINO_ZAWARUDOSEARCHENGINE_H
