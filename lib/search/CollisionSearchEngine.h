//
// Created by freddy on 29/04/19.
//

#ifndef OSARRACINO_COLLISIONSEARCHENGINE_H
#define OSARRACINO_COLLISIONSEARCHENGINE_H


#include "SearchEngine.h"
#include "evaluator/Evaluator.h"
#include <algorithm>
#include <evaluator/TorettoWhiteEvaluator.h>
#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ThetaMoveGenerator.h>
#include <movegenerator/ArnoldMoveGenerator.h>
#include <tuple>
#include <model/TranspositionTable.h>

// Returns optimal value for
// current player(Initially called
// for root and maximizer)
const int MAX = 10000000;
const int MIN = -MAX;

const int MAX_DEPTH = 5;

class CollisionSearchEngine : public SearchEngine<CollisionSearchEngine> {
public:
    int hits;
    TranspositionTable table;

    template<typename EvalType, typename MoveGeneratorType>
    Move __make_decision_internal(const Board &b,
                                  const Evaluator<EvalType> &eval,
                                  const MoveGenerator<MoveGeneratorType> &move_generator) {
        hits = 0;

        auto best_score = __make_decision(0, eval, move_generator, b);

        std::cout << "Best score: " << std::get<0>(best_score) << std::endl;
        std::cout << "Table hits:" << hits << std::endl;
        table.clear();

        return Move{std::get<1>(best_score), std::get<2>(best_score)};
    }

    template<typename EvalType, typename MoveGeneratorType>
    std::tuple<int, Position, Position> __make_decision(int depth, const Evaluator<EvalType> &eval,
                                                        const MoveGenerator<MoveGeneratorType> &moveGenerator,
                                                        Board game_state) {
        //Init the result
        std::tuple<int, Position, Position> result;
        std::get<0>(result) = -MAX;

        //Generate moves
        auto all_possible_moves = moveGenerator.generate(game_state);

        int max_depth = 1;
        int state_evaluation;

        do {
            //std::vector<int> cache_moves;
            //For each move

            for (const auto &move : all_possible_moves) {
                auto new_game_state = Board::from_board(game_state, move.from, move.to);
                state_evaluation = minimax(depth + 1, max_depth, eval, moveGenerator, false,
                                           new_game_state,
                                           -MAX, MAX,
                                           game_state.is_white);

                // cache_moves.push_back(state_evaluation);
                //Value is better so update it
                if (state_evaluation > std::get<0>(result)) {
                    std::get<0>(result) = state_evaluation;
                    std::get<1>(result) = move.from;
                    std::get<2>(result) = move.to;
                }
            }

            // We have a winning move[move_index]
            if (std::get<0>(result) > 9999) {
                break;
            }
            //indexes = sort_moves(cache_moves);
            max_depth++;

        } while (max_depth <= MAX_DEPTH);

        return result;
    }

    template<typename EvalType, typename MoveGeneratorType>
    int minimax(int depth, int max_depth,
                const Evaluator<EvalType> &eval,
                const MoveGenerator<MoveGeneratorType> &moveGenerator,
                bool maximizingPlayer,
                Board game_state, int alpha, int beta, bool leading_white) {
        move_count++;

        int alpha_orig = alpha;

        TTEntry * entry = table.get(game_state);
        if (entry != NULL && entry->depth >= depth) {
            hits++;
            switch (entry->flag) {
                case EXACT:
                    return entry->score;
                    break;
                case UPPERBOUND:
                    if (alpha < entry->score) {
                        alpha = entry->score;
                    }
                    break;
                case LOWERBOUND:
                    if (entry->score < beta) {
                        beta = entry->score;
                    }
                    break;
            }
        }


        if (depth == 0 || depth >= max_depth || game_state.is_black_win() || game_state.is_white_win()) {
            if (game_state.is_white)
                return eval.evaluate(game_state);
        }

        int evaluation;

        if (maximizingPlayer) { // Stiamo massimizzando

            //Generiamo tutte le mosse possibili
            auto all_possible_moves = moveGenerator.generate(game_state);
            //Evaluation = - infinito
            evaluation = -MAX;


            for (const auto &move : all_possible_moves) {
                auto new_game_state = Board::from_board(game_state, move.from,
                                                        move.to);

                evaluation = std::max(evaluation, minimax(depth + 1, max_depth, eval,
                                                          moveGenerator, false,
                                                          new_game_state, // new State
                                                          alpha, beta, leading_white));

                if (evaluation >= beta) {
                    return evaluation;
                }


                if (alpha < evaluation) {
                    alpha = evaluation;
                }

                if (evaluation <= alpha_orig) {
                    table.store(game_state, move, depth, evaluation,Flags::UPPERBOUND);
                } else {
                    table.store(game_state, move, depth, evaluation,Flags::EXACT);
                }
            }
            return evaluation;

        } else { // Stiamo minimizzando
            auto all_possible_moves = moveGenerator.generate(game_state);

            evaluation = MAX;


            for (auto const &move : all_possible_moves) {
                auto new_game_state = Board::from_board(game_state, move.from,
                                                        move.to);

                evaluation = std::min(evaluation, minimax(depth + 1, max_depth, eval,
                                                          moveGenerator, true,
                                                          new_game_state, // new State
                                                          alpha, beta, leading_white));

                if (evaluation <= alpha) {
                    return evaluation;
                }

                if (beta > evaluation) {
                    beta = evaluation;
                }

                if (evaluation >= beta) {
                    table.store(game_state, move, depth, evaluation,Flags::LOWERBOUND);
                } else {
                    table.store(game_state, move, depth, evaluation,Flags::EXACT);
                }
            }
            return evaluation;

        } //ENDIF MAXIMIZING PLAYER



    }
};

#endif //OSARRACINO_COLLISIONSEARCHENGINE_H
