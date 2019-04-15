//
// Created by freddy on 15/04/19.
//

#include <util/BitUtils.h>
#include "WhiteEvaluator.h"

WhiteEvaluator::WhiteEvaluator() {
    // Initialize the left and right masks
    for (int i = 0; i<9; i++) {
        low_mask[i] = ((uint32_t)1 << i) - 1;
        high_mask[i] = ~(((uint32_t)1 << (i+1)) -1);
    }
}

int WhiteEvaluator::evaluate(const Board &b) const {
    // Convert the board matrix to an array of columns and rows
    uint16_t cols[9] = {56, 16, 0, 257, 403, 257, 0, 16, 56};
    uint16_t rows[9] = {56, 16, 0, 257, 403, 257, 0, 16, 56};

    // Populate the bit matrix

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            if ((b.board[x][y] & (Pawn::White | Pawn::Black)) != 0) {
                cols[x] |= 1 << y;
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

    // TODO: consider the initial situation, when the king cannot move

    return king_movement_score;
}

int WhiteEvaluator::perform_search(const uint16_t *cols, const uint16_t *rows, int depth, int king_col, int king_row,
                                   bool horizontal) const {
    if (depth == 0) {
        return 0;  // TODO: return a value based on the distance from the nearest win point
    }

    int high_score = 0;
    int low_score = 0;

    if (horizontal) {
        int high_moves = BitUtils::get_high_moves(rows[king_row], king_col);
        int low_moves = BitUtils::get_low_moves(rows[king_row], king_col);

        // Check if the king is in a win position
        if (king_col - low_moves == 0 && king_col + high_moves == 8) {  // Double side winning
            return WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER*depth * 2;
        }else if (king_col - low_moves == 0 || king_col + high_moves == 8) {  // Single side winning
            return WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER*depth;
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
            return WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER*depth * 2;
        }else if (king_row - low_moves == 0 || king_row + high_moves == 8) {  // Single side winning
            return WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER*depth;
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
