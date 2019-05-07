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
    int worker_count = 6; // TODO: check that is not zero

    template<typename EvalType, typename MoveGeneratorType>
    int minimax(long &worker_move_count, int depth, const Evaluator<EvalType> &eval,
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
    void search_worker_init(std::vector<ThanosMoveConfiguration> &future_states,
                            ThanosMoveConfiguration &best_move,
                            long &worker_move_count,
                            int &early_exit,
                            float &elapsed,
                            int current_depth_limit,
                            const Evaluator<EvalType> &eval,
                            const MoveGenerator<MoveGeneratorType> &move_generator) {
        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

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
                early_exit = true;
                break;
            }
        }

        std::sort(future_states.begin(), future_states.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

        elapsed = timer.elapsed();

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
        bool force_exit;
        std::vector<long> worker_move_counts(worker_count);
        std::vector<ThanosMoveConfiguration> results(worker_count);

        do {
            std::vector<std::thread> threads;
            std::vector<int> worker_early_exit(worker_count);
            std::vector<float> worker_elapsed(worker_count);

            int current_slice = 0;
            for (auto &move_slice: slices) {
                worker_early_exit[current_slice] = false;

                std::thread t(&ThanosSearchEngine::search_worker_init<EvalType, MoveGeneratorType>,
                        this,
                        std::ref(move_slice),
                        std::ref(results[current_slice]),
                        std::ref(worker_move_counts[current_slice]),
                        std::ref(worker_early_exit[current_slice]),
                        std::ref(worker_elapsed[current_slice]),
                        current_depth_limit,
                        std::ref(eval), std::ref(move_generator));

                current_slice++;
                threads.push_back(std::move(t));
            }

            for (auto &t: threads) {
                t.join();
            }


            // Check if any of the threads exited early
            force_exit = false;
            int current_worker = 0;
            for (auto worker_exit : worker_early_exit) {
                if (worker_exit) {
                    std::cout << "Worker "<<current_worker<<" requested early exit" << std::endl;
                    force_exit = true;
                    break;
                }
                current_worker++;
            }

            // Calculate complessive move count
            move_count = 0;
            for (int worker_move_count : worker_move_counts) {
                move_count += worker_move_count;
            }

            // Print per/thread statistics
            for (int i = 0; i<worker_count; ++i) {
                std::cout << "Worker: " << i << " explored " << worker_move_counts[i] << " moves in "
                          << worker_elapsed[i] << " seconds with score: " << results[i].score << std::endl;
            }

            std::cout << "Searched depth: " << (current_depth_limit+1) << ". Explored a total of " << move_count << " moves in "
                      << timer.elapsed() << " s" << std::endl;



            current_depth_limit++;
        } while (current_depth_limit <= THANOS_MAX_DEPTH && !force_exit && !timer.is_timed_out());

        if (timer.is_timed_out()) {
            std::cout << "TIMED OUT" << std::endl;
        }

        std::sort(results.begin(), results.end(), [](const auto &s1, const auto &s2) {
            return s1.score > s2.score;
        });

        auto best_state = results[0];

        std::cout << "Best score: " << best_state.score << std::endl;
        std::cout << "Reached depth: " << current_depth_limit-1 << std::endl;


        return best_state.move;
    }
};

#endif //OSARRACINO_THANOSSEARCHENGINE_H
