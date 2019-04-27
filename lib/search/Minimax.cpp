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
#include <tuple>
#include <numeric>

// Returns optimal value for
// current player(Initially called
// for root and maximizer)
const int MAX = 10000000;
const int MIN = -MAX;

const int MAX_DEPTH = 5;

int moves = 0;
Line main_line;

template<typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
int Minimax::minimax(int depth, int max_depth,
                     const Evaluator<WhiteEvalType> &whiteEval,
                     const Evaluator<BlackEvalType> &blackEval,
                     const MoveGenerator<MoveGeneratorType> &moveGenerator,
                     bool maximizingPlayer,
                     Board &game_state, int alpha, int beta, bool leading_white,
                     Line &pline) {
    moves++;

    // Se siamo alla radice
    // oppure siamo arrivati alle foglie
    // oppure siamo in una board che indica la terminazione del gioco
    // allora ritorna.
    if (depth == 0 || depth >= max_depth || game_state.is_black_win() || game_state.is_white_win()) {
        pline.counter = 0;
        return leading_white ? whiteEval.evaluate(game_state) : blackEval.evaluate(game_state);
    }
    Line line;

    int evaluation;

    if (maximizingPlayer) { // Stiamo massimizzando

        //Generiamo tutte le mosse possibili
        auto all_possible_moves = moveGenerator.generate(game_state);
        //Evaluation = - infinito
        evaluation = -MAX;

        if (main_line.counter > depth) {
            auto it = std::find(all_possible_moves.begin(), all_possible_moves.end(), main_line.move[depth]);
            if (it != all_possible_moves.end()) {
                auto temp = all_possible_moves.at(0);
                all_possible_moves[0] = main_line.move[depth];
                (*it) = temp;
            }
        }



        for (const auto &move : all_possible_moves) {
            auto new_game_state = Board::from_board(game_state, move.first,
                                                    move.second);
            evaluation = std::max(evaluation, minimax(depth + 1, max_depth, whiteEval, blackEval,
                                                      moveGenerator, false,
                                                      new_game_state, // new State
                                                      alpha, beta, leading_white, line));

            if (evaluation >= beta) {
                return evaluation;
            }

            alpha = std::max(alpha, evaluation);
            if (alpha == evaluation) {
                pline.move[0] = move;
                pline.counter = 1;
                for (int i = 0; i < line.counter; i++) {
                    pline.move[pline.counter] = line.move[i];
                    pline.counter++;
                }
            }
        }
        return evaluation;

    } else { // Stiamo minimizzando
        auto all_possible_moves = moveGenerator.generate(game_state);

        evaluation = MAX;

        if (main_line.counter > depth) {
            auto it = std::find(all_possible_moves.begin(), all_possible_moves.end(), main_line.move[depth]);
            if (it != all_possible_moves.end()) {
                auto temp = all_possible_moves.at(0);
                all_possible_moves[0] = main_line.move[depth];
                (*it) = temp;
            }
        }

        for (auto const &move : all_possible_moves) {
            auto new_game_state = Board::from_board(game_state, move.first,
                                                    move.second);
            evaluation = std::min(evaluation, minimax(depth + 1, max_depth, whiteEval, blackEval,
                                                      moveGenerator, true,
                                                      new_game_state, // new State
                                                      alpha, beta, leading_white, line));
            if (evaluation <= alpha) {
                return evaluation;
            }

            beta = std::min(beta, evaluation);
            if (beta == evaluation) {
                pline.move[0] = move;
                pline.counter = 1;
                for (int i = 0; i < line.counter; i++) {
                    pline.move[pline.counter] = line.move[i];
                    pline.counter++;
                }
            }
        }
        return evaluation;

    } //ENDIF MAXIMIZING PLAYER



}

std::string Minimax::best_move(Board &b) {
    TorettoWhiteEvaluator whiteEval;
    BlackEvaluator blackEval;
    ArnoldMoveGenerator moveGenerator;
    //ThetaMoveGenerator moveGenerator;

    const clock_t begin_time = clock();

    moves = 0;
    // Incremental deepening


    std::tuple<int, Position, Position> best_score;
    Line actual_line;
    best_score = make_decision(0, whiteEval, blackEval, moveGenerator, b, actual_line);

    // Print statistics

    float elapsed = (clock() - begin_time) / float(CLOCKS_PER_SEC);
    float speed = float(moves) / elapsed;
    std::cout << "Evaluation completed. Results: " << std::endl;
    std::cout << "Explored " << moves << " moves in " << elapsed << " seconds " << std::endl;
    std::cout << "Best score: " << std::get<0>(best_score) << std::endl;
    std::cout << "Speed: " << speed << " moves/second." << std::endl;

    std::string color = b.is_white ? "WHITE" : "BLACK";

    return std::string(
            "{\"from\":\"" + std::get<1>(best_score).to_move() + "\",\"to\":\"" + std::get<2>(best_score).to_move() + "\",\"turn\":\"" + color + "\"}");
}

template<typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
std::tuple<int, Position, Position> Minimax::make_decision(int depth, const Evaluator<WhiteEvalType> &whiteEval,
                                                           const Evaluator<BlackEvalType> &blackEval,
                                                           const MoveGenerator<MoveGeneratorType> &moveGenerator,
                                                           Board &game_state, Line &pline) {

    Line line;
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
        if (main_line.counter > depth) {
            auto it = std::find(all_possible_moves.begin(), all_possible_moves.end(), main_line.move[depth]);
            if (it != all_possible_moves.end()) {
                auto temp = all_possible_moves.at(0);
                all_possible_moves[0] = main_line.move[depth];
                (*it) = temp;
            }
        }

        for (const auto &move : all_possible_moves) {
            auto new_game_state = Board::from_board(game_state, move.first, move.second);
            state_evaluation = minimax(depth + 1, max_depth, whiteEval, blackEval, moveGenerator, false,
                                           new_game_state,
                                           -MAX, MAX,
                                           game_state.is_white, line);

            // cache_moves.push_back(state_evaluation);
            //Value is better so update it
            if (state_evaluation > std::get<0>(result)) {
                std::get<0>(result) = state_evaluation;
                std::get<1>(result) = move.first;
                std::get<2>(result) = move.second;
                pline.move[0] = move;
                pline.counter = 1;
            }
        }
        for (int i = 0; i < line.counter; i++) {
            pline.move[pline.counter] = line.move[i];
            pline.counter++;
        }

        // We have a winning move[move_index]
        if (std::get<0>(result) > 9999) {
            break;
        }
        //indexes = sort_moves(cache_moves);

        std::cout << "Indexes for iteration " << max_depth << std::endl;
        max_depth++;

        main_line = pline;

        for (int i = 0; i < main_line.counter && i < 5; i++) {
            std::cout << "FROM: " << main_line.move[i].first.to_move() << " TO: " << main_line.move[i].second.to_move() << std::endl;
        }

    }while(max_depth <= MAX_DEPTH);

    return result;
}


