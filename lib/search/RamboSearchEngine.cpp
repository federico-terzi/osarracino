//
// Created by freddy on 26/04/19.
//

#include "RamboSearchEngine.h"
#include <evaluator/TorettoWhiteEvaluator.h>
#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ThetaMoveGenerator.h>
#include <movegenerator/ArnoldMoveGenerator.h>
#include <limits>

template<typename EvalType, typename MoveGeneratorType>
int RamboSearchEngine::minimax(int depth, const Evaluator<EvalType> &eval,
                               const MoveGenerator<MoveGeneratorType> &move_generator, bool maximizing_player,
                               const Board &board, int alpha, int beta) {
    move_count++;

    if (depth == 0 || board.is_black_win() || board.is_white_win()) {
        return eval.evaluate(board);
    }

    auto moves {move_generator.generate(board)};

    if (maximizing_player) {
        int value = std::numeric_limits<int>::min();

        for (const auto& move : moves) {
            auto new_board {Board::from_board(board, move.from, move.to)};
            value = std::max(value, minimax(depth - 1, eval, move_generator, false, new_board, alpha, beta));
            if (value >= beta) {
                return value;
            }
            alpha = std::max(alpha, value);
        }

        return value;
    }else{
        int value = std::numeric_limits<int>::max();

        for (const auto& move : moves) {
            auto new_board {Board::from_board(board, move.from, move.to)};
            value = std::min(value, minimax(depth - 1, eval, move_generator, true, new_board, alpha, beta));

            if (value <= alpha) {
                return value;
            }

            beta = std::min(beta, value);
        }

        return value;
    }
}

Move RamboSearchEngine::__make_decision_internal(const Board &b) {
    TorettoWhiteEvaluator white_eval;
    BlackEvaluator black_eval;
    ArnoldMoveGenerator move_generator;

    int best_score = std::numeric_limits<int>::min();
    Move best_move;

    auto moves {move_generator.generate(b)};

    // TODO: ordering

    int current_depth_limit = 0;
    do {
        current_depth_limit++;

        // TODO: ordering

        for (const auto& move: moves) {
            auto new_board {Board::from_board(b, move.from, move.to)};

            int value;
            if (b.is_white) {
                value = minimax(current_depth_limit, white_eval, move_generator, false, new_board,
                        std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
            }else{
                value = minimax(current_depth_limit, black_eval, move_generator, false, new_board,
                                std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
            }

            if (value > best_score) {
                best_move = move;
                best_score = value;
            }

            if (value > 100000) {
                break;
            }
        }
    }while(current_depth_limit <= 3);

    return best_move;
}