//
// Created by freddy on 17/04/19.
//

#include <random>
#include <algorithm>
#include <util/BitUtils.h>
#include "ArnoldMoveGenerator.h"

const bool citadels[9][9] = {
        {0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0},
};

const uint16_t citadel_masks[9] = {
        0b0000000'111000111,
        0b0000000'111101111,
        0b0000000'111111111,
        0b0000000'111111111,
        0b0000000'001111100,
        0b0000000'111111111,
        0b0000000'111111111,
        0b0000000'111101111,
        0b0000000'111000111,
};

std::vector<Move> ArnoldMoveGenerator::generate(const Board &b) const {
    // Convert the board matrix to an array of columns and rows
    uint16_t obstacle_cols[9] = {56, 16, 0, 257, 403, 257, 0, 16, 56};
    uint16_t obstacle_rows[9] = {56, 16, 0, 257, 403, 257, 0, 16, 56};

    uint16_t cols[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t rows[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    std::vector<Position> to_be_moved;

    std::vector<Move> current_moves;

    if (b.is_white_win() || b.is_black_win()) {
        return current_moves;
    }

    Pawn target_pawn = b.is_white ? Pawn::KingOrWhite : Pawn::Black;

    // Populate the bit matrix

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            if ((b.board[x][y] & (Pawn::White | Pawn::Black | Pawn::King)) != 0) {
                cols[x] |= 1 << y;

                if ((b.board[x][y] & target_pawn) != 0) {
                    to_be_moved.push_back(Position{x,y});
                }
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

    current_moves.reserve(30);

    for (auto &pawn : to_be_moved) {
        uint16_t target_col = cols[pawn.col] | obstacle_cols[pawn.col];
        uint16_t target_row = rows[pawn.row] | obstacle_rows[pawn.row];

        // Check if it's a black pawn inside a citadel
        // to enable passing through the citadel itself
        if (citadels[pawn.col][pawn.row]) {
            target_col &= (citadel_masks[pawn.col] | cols[pawn.col]);
            target_row &= (citadel_masks[pawn.row] | rows[pawn.row]);
        }


        int horizontal_high_moves = BitUtils::get_high_moves(target_row, pawn.col);
        for (int i = (pawn.col+1); i <= (pawn.col + horizontal_high_moves); i++) {
            current_moves.push_back({pawn, Position{i, pawn.row}});
        }

        int horizontal_low_moves = BitUtils::get_low_moves(target_row, pawn.col);
        for (int i = (pawn.col-1); i >= (pawn.col- horizontal_low_moves); i--) {
            current_moves.push_back({pawn, Position{i, pawn.row}});
        }

        int vertical_high_moves = BitUtils::get_high_moves(target_col, pawn.row);
        for (int i = (pawn.row+1); i <= (pawn.row + vertical_high_moves); i++) {
            current_moves.push_back({pawn, Position{pawn.col, i}});
        }

        int vertical_low_moves = BitUtils::get_low_moves(target_col, pawn.row);
        for (int i = (pawn.row-1); i >= (pawn.row - vertical_low_moves); i--) {
            current_moves.push_back({pawn, Position{pawn.col, i}});
        }
    }

    return current_moves;
}