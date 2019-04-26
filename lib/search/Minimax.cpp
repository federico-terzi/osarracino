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

// Returns optimal value for
// current player(Initially called
// for root and maximizer)
const int MAX = 10000000;
const int MIN = -MAX;

const int MAX_DEPTH = 4;

int depth = 0;
int moves = 0;

template<typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
int Minimax::minimax(int depth, int max_depth,
                     const Evaluator<WhiteEvalType> &whiteEval,
                     const Evaluator<BlackEvalType> &blackEval,
                     const MoveGenerator<MoveGeneratorType> &moveGenerator,
                     bool maximizingPlayer,
                     Board game_state, int alpha, int beta, bool leading_white,
                     Line &pline) {
    moves++;

    // Se siamo alla radice
    // oppure siamo arrivati alle foglie
    // oppure siamo in una board che indica la terminazione del gioco
    // allora ritorna.
    if (depth == 0 || depth >= max_depth || game_state.is_black_win() || game_state.is_white_win()) {
        return leading_white ? whiteEval.evaluate(game_state) : blackEval.evaluate(game_state);
    }

    int evaluation;
    if (maximizingPlayer) { // Stiamo massimizzando

        //Generiamo tutte le mosse possibili
        auto all_possible_moves = moveGenerator.generate(game_state);

        //Evaluation = - infinito
        evaluation = -MAX;
        for (auto const &move : all_possible_moves) {
            evaluation = std::max(evaluation, minimax(depth + 1, max_depth, whiteEval, blackEval,
                                                      moveGenerator, false,
                                                      Board::from_board(game_state, move.first,
                                                                        move.second), // new State
                                                      alpha, beta, leading_white, pline));
            if (evaluation >= beta) {
                return evaluation;
            }
            alpha = std::max(alpha, evaluation);
        }
        return evaluation;

    } else { // Stiamo minimizzando
        auto all_possible_moves = moveGenerator.generate(game_state);
        evaluation = MAX;
        for (auto const &move : all_possible_moves) {
            evaluation = std::min(evaluation, minimax(depth + 1, max_depth, whiteEval, blackEval,
                                                      moveGenerator, true,
                                                      Board::from_board(game_state, move.first,
                                                                        move.second), // new State
                                                      alpha, beta, leading_white, pline));

            if (evaluation <= alpha) {
                return evaluation;
            }
            beta = std::min(beta, evaluation);
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
    Line main_line;
    best_score = make_decision(0, whiteEval, blackEval, moveGenerator, b, main_line);

    // Print statistics

    float elapsed = (clock() - begin_time) / float(CLOCKS_PER_SEC);
    float speed = float(moves) / elapsed;
    std::cout << "Evaluation completed. Results: " << std::endl;
    std::cout << "Explored " << moves << " moves in " << elapsed << " seconds " << std::endl;
    std::cout << "Best score: " << std::get<0>(best_score) << std::endl;
    std::cout << "Speed: " << speed << " moves/second." << std::endl;
    std::cout << "Reached depth: " << depth << std::endl;


    std::string color = b.is_white ? "WHITE" : "BLACK";

    return std::string(
            "{\"from\":\"" + std::get<1>(best_score).to_move() + "\",\"to\":\"" + std::get<2>(best_score).to_move() + "\",\"turn\":\"" + color + "\"}");
}

template<typename WhiteEvalType, typename BlackEvalType, typename MoveGeneratorType>
std::tuple<int, Position, Position> Minimax::make_decision(int depth, const Evaluator<WhiteEvalType> &whiteEval,
                                                           const Evaluator<BlackEvalType> &blackEval, const MoveGenerator<MoveGeneratorType> &moveGenerator,
                                                           Board game_state, Line &pline) {

    Line line;
    //Init the result
    std::tuple<int, Position, Position> result;
    std::get<0>(result) = -MAX;

    //Generate moves
    auto all_possible_moves = moveGenerator.generate(game_state);
    std::vector<std::tuple<int, Position, Position>> cache_moves;

    int max_depth = 1;
    do {
        //For each move
        for (const auto &move : all_possible_moves) {
            int state_evaluation = minimax(depth + 1, max_depth, whiteEval, blackEval, moveGenerator, false,
                                           Board::from_board(game_state, move.first, move.second),
                                           -MAX, MAX,

                                           game_state.is_white, line);
            cache_moves.emplace_back(state_evaluation, move.first, move.second);
            //Value is better so update it
            if (state_evaluation > std::get<0>(result)) {
                std::get<0>(result) = state_evaluation;
                std::get<1>(result) = move.first;
                std::get<2>(result) = move.second;
            }
        }
        max_depth++;
        /*
        std::sort(cache_moves.begin(), cache_moves.end(), [](const auto &e1, const auto &e2) {
            return std::get<0>(e1) > std::get<0>(e2);
        });
        all_possible_moves.clear();
        for (const auto &element : cache_moves) {
            all_possible_moves.emplace_back(std::get<1>(element), std::get<2>(element));

        }*/

    }while(max_depth <= MAX_DEPTH);

    return result;
}

