//
// Created by freddy on 15/04/19.
//

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

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            if ((b.board[x][y] & (Pawn::White | Pawn::Black | Pawn::King)) != 0) {
                cols[x] |= 1 << y;
            }
        }
    }

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if ((b.board[x][y] & (Pawn::White | Pawn::Black | Pawn::King)) != 0) {
                rows[y] |= 1 << x;
            }
        }
    }


    return 0;
}
