//
// Created by freddy on 30/04/19.
//

#ifndef OSARRACINO_THANOSSEARCHENGINE_H
#define OSARRACINO_THANOSSEARCHENGINE_H

#include "SearchEngine.h"
#include "RamboSearchEngine.h"
#include <algorithm>
#include <thread>

const int THANOS_MAX_DEPTH = 10;

struct ThanosMoveConfiguration {
    Move move;
    Board board;
    int score;
    int depth;
};

class ThanosSearchEngine : public SearchEngine<ThanosSearchEngine> {
public:
    int worker_count = 8; // TODO: check that is not zero

    template<typename EvalType, typename MoveGeneratorType>
    int minimax(int &worker_move_count, int depth, const Evaluator<EvalType> &eval,
                const MoveGenerator<MoveGeneratorType> &move_generator,
                bool maximizing_player,
                const Board &board, int alpha, int beta) {
        worker_move_count++;

        if (depth == 0 || timer.is_timed_out() || board.is_black_win() || board.is_white_win()) {
            return eval.evaluate(board);
        }

        auto moves{move_generator.generate(board)};

        if (maximizing_player) {
            int value = std::numeric_limits<int>::min();

            for (const auto &move : moves) {
                auto new_board{Board::from_board(board, move.from, move.to)};
                value = std::max(value, minimax(std::ref(worker_move_count), depth - 1, eval, move_generator, false, new_board, alpha, beta));

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
                value = std::min(value, minimax(std::ref(worker_move_count), depth - 1, eval, move_generator, true, new_board, alpha, beta));

                if (value <= alpha) {
                    return value;
                }

                beta = std::min(beta, value);
            }

            return value;
        }
    }

    template<typename EvalType, typename MoveGeneratorType>
    void search_worker(std::vector<ThanosMoveConfiguration> &future_states) {

    }

    template<typename EvalType, typename MoveGeneratorType>
    void search_worker_init(std::vector<ThanosMoveConfiguration> &future_states,
                            ThanosMoveConfiguration &best_move,
                            int &worker_move_count,
                            int current_depth_limit,
                            const Evaluator<EvalType> &eval,
                            const MoveGenerator<MoveGeneratorType> &move_generator) {
        for (auto &state: future_states) {
            int value = minimax(std::ref(worker_move_count), current_depth_limit, eval, move_generator, false, state.board,
                                std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

            if (timer.is_timed_out()) {
                break;
            }

            // Update future score based on the minmax search
            state.score = value;
            state.depth = current_depth_limit;

            if (value > 100000) {
                std::cout << "Stopping evaluation with winning move: " << state.move << " at depth: "
                          << current_depth_limit << std::endl;
                break;
            }
        }

        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

        best_move = future_states[0];
    }

    template<typename EvalType, typename MoveGeneratorType>
    Move __make_decision_internal(const Board &b,
                                  const Evaluator<EvalType> &eval,
                                  const MoveGenerator<MoveGeneratorType> &move_generator) {

        auto moves{move_generator.generate(b)};

        std::vector<ThanosMoveConfiguration> future_states;
        for (const auto &move : moves) {
            auto board{Board::from_board(b, move.from, move.to)};

            int score = eval.evaluate(board);

            future_states.push_back({move, board, score});
        }


        // Make the slices
        std::vector<std::vector<ThanosMoveConfiguration>> slices(worker_count);
        int worker_index = 0;
        for (auto &state : future_states) {
            slices[worker_index].push_back(state);

            worker_index++;
            if (worker_index >= worker_count) {
                worker_index= 0;
            }
        }

        int current_depth_limit = 0;
        bool force_exit = false;

        std::vector<int> worker_move_counts(worker_count);
        std::vector<ThanosMoveConfiguration> results(worker_count);

        do {
            std::cout << "Searching depth: " << current_depth_limit << ". Explored " << move_count << " moves in "
                      << timer.elapsed() << " s" << std::endl;

            std::vector<std::thread> threads;

            int current_slice = 0;
            for (auto &move_slice: slices) {
                /*std::vector<ThanosMoveConfiguration> &future_states,
                ThanosMoveConfiguration &best_move,
                int &worker_move_count,
                int current_depth_limit,
                const Evaluator<EvalType> &eval,
                const MoveGenerator<MoveGeneratorType> &move_generator*/

                /*std::thread t(&ThanosSearchEngine::search_worker<EvalType, MoveGeneratorType>,
                        this,
                        move_slice
                        );*/

                std::thread t(&ThanosSearchEngine::search_worker_init<EvalType, MoveGeneratorType>,
                        this,
                        std::ref(move_slice),
                        std::ref(results[current_slice]),
                        std::ref(worker_move_counts[current_slice]),
                        current_depth_limit,
                        std::ref(eval), std::ref(move_generator));

                current_slice++;
                threads.push_back(std::move(t));
            }

            for (auto &t: threads) {
                t.join();
            }

            current_depth_limit++;
        } while (current_depth_limit <= THANOS_MAX_DEPTH && !timer.is_timed_out());

        if (timer.is_timed_out()) {
            std::cout << "TIMED OUT" << std::endl;
        }

        std::sort(results.begin(), results.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

        for (int worker_move_count : worker_move_counts) {
            move_count += worker_move_count;
        }

        auto best_state = results[0];

        std::cout << "Best score: " << best_state.score << std::endl;
        std::cout << "Reached depth: " << current_depth_limit-1 << std::endl;


        return best_state.move;
    }
};

#endif //OSARRACINO_THANOSSEARCHENGINE_H
