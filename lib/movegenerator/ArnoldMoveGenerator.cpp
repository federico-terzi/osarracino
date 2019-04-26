//
// Created by freddy on 17/04/19.
//

#include <util/BitUtils.h>
#include <utility>
#include "ArnoldMoveGenerator.h"

std::vector<std::pair<Position,Position>> ArnoldMoveGenerator::generate(const Board &b) const {
    // Convert the board matrix to an array of columns and rows
    uint16_t cols[9] = {56, 16, 0, 257, 403, 257, 0, 16, 56};
    uint16_t rows[9] = {56, 16, 0, 257, 403, 257, 0, 16, 56};

    std::vector<Position> to_be_moved;

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

    std::vector<std::pair<Position,Position>> current_moves;

    for (auto &pawn : to_be_moved) {

        int horizontal_high_moves = BitUtils::get_high_moves(rows[pawn.row], pawn.col);
        for (int i = (pawn.col+1); i <= (pawn.col + horizontal_high_moves); i++) {
            current_moves.emplace_back(pawn, Position{i, pawn.row});
        }

        int horizontal_low_moves = BitUtils::get_low_moves(rows[pawn.row], pawn.col);
        for (int i = (pawn.col-1); i >= (pawn.col- horizontal_low_moves); i--) {
            current_moves.emplace_back(pawn ,Position{i, pawn.row});
        }

        int vertical_high_moves = BitUtils::get_high_moves(cols[pawn.col], pawn.row);
        for (int i = (pawn.row+1); i <= (pawn.row + vertical_high_moves); i++) {
            current_moves.emplace_back(pawn, Position{pawn.col, i});
        }

        int vertical_low_moves = BitUtils::get_low_moves(cols[pawn.col], pawn.row);
        for (int i = (pawn.row-1); i >= (pawn.row - vertical_low_moves); i--) {
            current_moves.emplace_back(pawn, Position{pawn.col, i});
        }
    }

    return current_moves;
}
