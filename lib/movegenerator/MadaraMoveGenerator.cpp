//
// Created by freddy on 12/05/19.
//

#include "util/BitUtils.h"
#include <algorithm>
#include "MadaraMoveGenerator.h"

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

MadaraMoveGenerator::MadaraMoveGenerator() {
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
                for (int king_col = 0; king_col < 9; king_col++) {
                    for (int king_row = 0; king_row < 9; king_row++) {
                        int from_penality = abs(king_col - col) + abs(king_row - row);

                        std::vector<Move> white_current_moves;
                        std::vector<Move> black_current_moves;

                        int horizontal_high_moves = BitUtils::get_high_moves(row_config, col);
                        for (int i = (col+1); i <= (col + horizontal_high_moves); i++) {
                            int to_penality = abs(king_col - i) + abs(king_row - row);
                            white_current_moves.push_back({Position{col, row}, Position{i, row}, from_penality});
                            black_current_moves.push_back({Position{col, row}, Position{i, row}, to_penality});
                        }

                        int horizontal_low_moves = BitUtils::get_low_moves(row_config, col);
                        for (int i = (col-1); i >= (col- horizontal_low_moves); i--) {
                            int to_penality = abs(king_col - i) + abs(king_row - row);
                            white_current_moves.push_back({Position{col, row}, Position{i, row}, from_penality});
                            black_current_moves.push_back({Position{col, row}, Position{i, row}, to_penality});
                        }

                        std::sort(white_current_moves.begin(), white_current_moves.end(), [](const auto &item1, const auto &item2) {
                            return item1.penality < item2.penality;
                        });
                        std::sort(black_current_moves.begin(), black_current_moves.end(), [](const auto &item1, const auto &item2) {
                            return item1.penality < item2.penality;
                        });

                        row_to_horizontal_moves[row_config][col][row][king_col][king_row][0] = std::move(black_current_moves);
                        row_to_horizontal_moves[row_config][col][row][king_col][king_row][1] = std::move(white_current_moves);
                    }
                }
            }
        }
    }

    // Initialize the vertical moves cache.
    for (uint16_t col_config = 0; col_config < 512; col_config++) {
        for (int col = 0; col < 9; col++) {
            for (int row = 0; row < 9; row++) {
                for (int king_col = 0; king_col < 9; king_col++) {
                    for (int king_row = 0; king_row < 9; king_row++) {
                        int from_penality = abs(king_col - col) + abs(king_row - row);

                        std::vector<Move> white_current_moves;
                        std::vector<Move> black_current_moves;

                        int vertical_high_moves = BitUtils::get_high_moves(col_config, row);
                        for (int i = (row+1); i <= (row + vertical_high_moves); i++) {
                            int to_penality = abs(king_col - col) + abs(king_row - i);
                            white_current_moves.push_back({Position{col, row}, Position{col, i}, from_penality});
                            black_current_moves.push_back({Position{col, row}, Position{col, i}, to_penality});
                        }

                        int vertical_low_moves = BitUtils::get_low_moves(col_config, row);
                        for (int i = (row-1); i >= (row - vertical_low_moves); i--) {
                            int to_penality = abs(king_col - col) + abs(king_row - i);
                            white_current_moves.push_back({Position{col, row}, Position{col, i}, from_penality});
                            black_current_moves.push_back({Position{col, row}, Position{col, i}, to_penality});
                        }

                        std::sort(white_current_moves.begin(), white_current_moves.end(), [](const auto &item1, const auto &item2) {
                            return item1.penality < item2.penality;
                        });
                        std::sort(black_current_moves.begin(), black_current_moves.end(), [](const auto &item1, const auto &item2) {
                            return item1.penality < item2.penality;
                        });

                        col_to_vertical_moves[col_config][col][row][king_col][king_row][0] = std::move(black_current_moves);
                        col_to_vertical_moves[col_config][col][row][king_col][king_row][1] = std::move(white_current_moves);
                    }
                }
            }
        }
    }
}

std::vector<Move> MadaraMoveGenerator::generate(const Board &b) const {
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


        auto &horizontal_moves {row_to_horizontal_moves[target_row][pawn.col][pawn.row][b.king_pos.col][b.king_pos.row][b.is_white]};
        current_moves.insert(current_moves.end(), horizontal_moves.begin(), horizontal_moves.end());

        auto &vertical_moves {col_to_vertical_moves[target_col][pawn.col][pawn.row][b.king_pos.col][b.king_pos.row][b.is_white]};
        current_moves.insert(current_moves.end(), vertical_moves.begin(), vertical_moves.end());
    }

    std::sort(current_moves.begin(), current_moves.end(), [](const auto &item1, const auto &item2) {
        return item1.penality < item2.penality;
    });

    return current_moves;
}
