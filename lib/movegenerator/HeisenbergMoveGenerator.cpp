//
// Created by freddy on 27/04/19.
//

#include "util/BitUtils.h"
#include <algorithm>
#include "HeisenbergMoveGenerator.h"

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

HeisenbergMoveGenerator::HeisenbergMoveGenerator() {
    // Initialize the col_to_positions cache, by precomputing all the possible
    // configurations for a given column.
    for (uint16_t col_config = 0; col_config < 512; col_config++) {
        for (int col = 0; col < 9; col++) {
            std::vector<Position> positions;

            for (int row = 0; row < 9; row++) {
                if (col_config & (1<<row)) {
                    positions.push_back({col, row});
                }
            }

            col_to_positions[col_config][col] = std::move(positions);
        }
    }

    // Initialize the horizontal moves cache.
    for (uint16_t row_config = 0; row_config < 512; row_config++) {
        for (int col = 0; col < 9; col++) {
            for (int row = 0; row < 9; row++) {
                std::vector<Move> current_moves;

                int horizontal_high_moves = BitUtils::get_high_moves(row_config, col);
                for (int i = (col+1); i <= (col + horizontal_high_moves); i++) {
                    current_moves.push_back({Position{col, row}, Position{i, row}});
                }

                int horizontal_low_moves = BitUtils::get_low_moves(row_config, col);
                for (int i = (col-1); i >= (col- horizontal_low_moves); i--) {
                    current_moves.push_back({Position{col, row}, Position{i, row}});
                }

                row_to_horizontal_moves[row_config][col][row] = std::move(current_moves);
            }
        }
    }

    // Initialize the vertical moves cache.
    for (uint16_t col_config = 0; col_config < 512; col_config++) {
        for (int col = 0; col < 9; col++) {
            for (int row = 0; row < 9; row++) {
                std::vector<Move> current_moves;

                int vertical_high_moves = BitUtils::get_high_moves(col_config, row);
                for (int i = (row+1); i <= (row + vertical_high_moves); i++) {
                    current_moves.push_back({Position{col, row}, Position{col, i}});
                }

                int vertical_low_moves = BitUtils::get_low_moves(col_config, row);
                for (int i = (row-1); i >= (row - vertical_low_moves); i--) {
                    current_moves.push_back({Position{col, row}, Position{col, i}});
                }

                col_to_vertical_moves[col_config][col][row] = std::move(current_moves);
            }
        }
    }
}

std::vector<Move> HeisenbergMoveGenerator::generate(const Board &b) const {
    std::vector<Move> current_moves;

    if (b.is_white_win() || b.is_black_win()) {
        return current_moves;
    }

    std::vector<Position> to_be_moved;
    const uint16_t *target_col_vector = (b.is_white) ? b.white_cols : b.black_cols;

    // Calculate the moves
    for (int col = 0; col < 9; col++) {
        const std::vector<Position> &positions {col_to_positions[target_col_vector[col]][col]};
        to_be_moved.insert(to_be_moved.end(), positions.begin(), positions.end());
    }


    current_moves.reserve(30);

    for (auto &pawn : to_be_moved) {
        uint16_t target_col = b.obstacle_cols[pawn.col];
        uint16_t target_row = b.obstacle_rows[pawn.row];

        // Check if it's a black pawn inside a citadel
        // to enable passing through the citadel itself
        if (citadels[pawn.col][pawn.row]) {
            target_col &= (citadel_masks[pawn.col] | b.white_cols[pawn.col] | b.black_cols[pawn.col]);
            target_row &= (citadel_masks[pawn.row] | b.white_rows[pawn.row] | b.black_rows[pawn.row]);
        }


        auto &horizontal_moves {row_to_horizontal_moves[target_row][pawn.col][pawn.row]};
        current_moves.insert(current_moves.end(), horizontal_moves.begin(), horizontal_moves.end());

        auto &vertical_moves {col_to_vertical_moves[target_col][pawn.col][pawn.row]};
        current_moves.insert(current_moves.end(), vertical_moves.begin(), vertical_moves.end());
    }

    if (!b.is_white) { // SORT BY KING DISTANCE
        std::sort(current_moves.begin(), current_moves.end(), [&b](const auto &item1, const auto &item2) {
           return abs(b.king_pos.col - item1.to.col + b.king_pos.row - item1.to.row) <  abs(b.king_pos.col - item2.to.col + b.king_pos.row - item2.to.row);
        });
    } else {
        std::sort(current_moves.begin(), current_moves.end(), [&b](const auto &item1, const auto &item2)-> bool {
           return abs(b.king_pos.col - item1.from.col + b.king_pos.row - item1.from.row) <  abs(b.king_pos.col - item2.from.col + b.king_pos.row - item2.from.row);
        });
    }
    return current_moves;
}
