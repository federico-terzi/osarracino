//
// Created by freddy on 15/04/19.
//

#include <util/BitUtils.h>
#include "WhiteEvaluator.h"

const uint16_t winpoints[9] = {
        0b00000000'11000110,
        0b00000001'00000001,
        0b00000001'00000001,
        0b00000000'00000000,
        0b00000000'00000000,
        0b00000000'00000000,
        0b00000001'00000001,
        0b00000001'00000001,
        0b00000000'11000110
};

int WhiteEvaluator::evaluate(const Board &b) const {
    // Convert the board matrix to an array of columns and rows
    uint16_t cols[9] = {56, 16, 0, 257, 403, 257, 0, 16, 56};
    uint16_t rows[9] = {56, 16, 0, 257, 403, 257, 0, 16, 56};

    int white_count = 0;
    int black_count = 0;
    int free_winpoint = 0;

    // Populate the bit matrix

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            if ((b.board[x][y] & (Pawn::White | Pawn::Black)) != 0) {
                cols[x] |= 1 << y;

                if ((b.board[x][y] & Pawn::White) != 0) {
                    white_count++;
                }else{
                    black_count++;
                }
            }else if ((b.board[x][y] & Pawn::WinPoint) != 0) {
                free_winpoint++;
            }
        }
    }

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if ((b.board[x][y] & (Pawn::White | Pawn::Black)) != 0) {
                rows[y] |= 1 << x;
            }
        }
    }

    // Calculate the best next move
    int horizontal_score = perform_search(cols, rows, WHITE_EVALUATOR_MAX_DEPTH, b.king_pos.col, b.king_pos.row, true);
    int vertical_score = perform_search(cols, rows, WHITE_EVALUATOR_MAX_DEPTH, b.king_pos.col, b.king_pos.row, false);

    int king_movement_score = std::max(horizontal_score, vertical_score);

    if (king_movement_score != 0) {  // King can move
        return king_movement_score;

    }else{   // King cannot move, use other euristics.
        // return a score based on the number of free win points, the number
        // of white pawns and the number of black pawns.

        return free_winpoint * WHITE_EVALUATOR_FREE_WINPOINT_MULTIPLIER +
               white_count * WHITE_EVALUATOR_WHITE_PAWN_MULTIPLIER +
               black_count * WHITE_EVALUATOR_BLACK_PAWN_MULTIPLIER;
    }
}

int WhiteEvaluator::perform_search(const uint16_t *cols, const uint16_t *rows, int depth, int king_col, int king_row,
                                   bool horizontal) const {
    if (depth == 0) {
        return 0;  // TODO: return a value based on the distance from the nearest win point
    }

    // TODO: optimize
    // TODO: test
    if ((winpoints[king_col] & (1u << king_row)) != 0) {
        return WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER;
    }

    int high_score = 0;
    int low_score = 0;

    if (horizontal) {
        int high_moves = BitUtils::get_high_moves(rows[king_row], king_col);
        int low_moves = BitUtils::get_low_moves(rows[king_row], king_col);

        // Check if the king is in a win position
        if (king_col - low_moves == 0 && king_col + high_moves == 8) {  // Double side winning
            return WHITE_EVALUATOR_SEARCH_WIN_POTENTIAL_MULTIPLIER*depth * 2;
        }else if (king_col - low_moves == 0 || king_col + high_moves == 8) {  // Single side winning
            return WHITE_EVALUATOR_SEARCH_WIN_POTENTIAL_MULTIPLIER*depth;
        }

        for (int i = (king_col+1); i <= (king_col + high_moves); i++) {
            high_score = std::max(perform_search(cols, rows, depth - 1, i, king_row, false), high_score);
        }
        for (int i = (king_col-1); i >= (king_col - low_moves); i--) {
            low_score = std::max(perform_search(cols, rows, depth - 1, i, king_row, false), low_score);
        }
    }else{
        int high_moves = BitUtils::get_high_moves(cols[king_col], king_row);
        int low_moves = BitUtils::get_low_moves(cols[king_col], king_row);

        // Check if the king is in a win position
        if (king_row - low_moves == 0 && king_row + high_moves == 8) {  // Double side winning
            return WHITE_EVALUATOR_SEARCH_WIN_POTENTIAL_MULTIPLIER*depth * 2;
        }else if (king_row - low_moves == 0 || king_row + high_moves == 8) {  // Single side winning
            return WHITE_EVALUATOR_SEARCH_WIN_POTENTIAL_MULTIPLIER*depth;
        }

        for (int i = (king_row+1); i <= (king_row + high_moves); i++) {
            high_score = std::max(perform_search(cols, rows, depth - 1, king_col, i, true), high_score);
        }
        for (int i = (king_row-1); i >= (king_row - low_moves); i--) {
            low_score = std::max(perform_search(cols, rows, depth - 1, king_col, i, true), low_score);
        }
    }

    return std::max(low_score, high_score);
}
