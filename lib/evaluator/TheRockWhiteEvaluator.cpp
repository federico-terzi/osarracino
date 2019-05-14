//
// Created by freddy on 13/05/19.
//


#include <util/BitUtils.h>
#include "TheRockWhiteEvaluator.h"

// Bonus or penality for the given king position
const int position_weight[9][9] = {
        {   0, 1000, 1000,   0,    0,   0, 1000, 1000,    0},
        {1000,  300,  200, 100,    0, 100,  200,  300, 1000},
        {1000,  200,  200, 200,  200, 200,  200,  200, 1000},
        {   0,  100,  200,   0,    0,   0,  200,  100,    0},
        {   0,    0,  200,   0,-9000,   0,  200,    0,    0},
        {   0,  100,  200,   0,    0,   0,  200,  100,    0},
        {1000,  300,  200, 200,  200, 200,  200,  300, 1000},
        {1000,  100,  200, 100,    0, 100,  200,  100, 1000},
        {   0, 1000, 1000,   0,    0,   0, 1000, 1000,    0},
};

const uint16_t black_risk_mask[9] = {
        0b0000000'011000110,
        0b0000000'111000111,
        0b0000000'110000011,
        0b0000000'000000000,
        0b0000000'000000000,
        0b0000000'000000000,
        0b0000000'110000011,
        0b0000000'111000111,
        0b0000000'011000110,
};

const uint16_t black_high_risk_mask[9] = {
        0b0000000'011000110,
        0b0000000'100000001,
        0b0000000'100000001,
        0b0000000'000000000,
        0b0000000'000000000,
        0b0000000'000000000,
        0b0000000'100000001,
        0b0000000'100000001,
        0b0000000'011000110,
};

int TheRockWhiteEvaluator::evaluate(const Board &b) const {
    // TODO: check if obstacle rows/cols must not include king

    // Check if the board is a winning board
    if (b.is_white_win()) {
        return THEROCK_EVALUATOR_SEARCH_WIN_MULTIPLIER;
    }else if (b.is_black_win()) {
        return THEROCK_EVALUATOR_SEARCH_LOSE_MULTIPLIER;
    }

    // Calculate the best next move
    int horizontal_score = perform_search(b.obstacle_cols, b.obstacle_rows, THEROCK_EVALUATOR_MAX_DEPTH, b.king_pos.col, b.king_pos.row, true);
    int vertical_score = perform_search(b.obstacle_cols, b.obstacle_rows, THEROCK_EVALUATOR_MAX_DEPTH, b.king_pos.col, b.king_pos.row, false);

    int score = std::max(horizontal_score, vertical_score);

    int white_winpoints = calculate_white_winpoints(b);
    int black_winpoints = calculate_black_winpoints(b);
    int black_high_risk = calculate_black_count_mask(b, black_high_risk_mask);
    int black_risk = calculate_black_count_mask(b, black_risk_mask);

    // Consider also the number of free win points, the number
    // of white pawns and the number of black pawns.
    score += b.free_winpoints * THEROCK_EVALUATOR_FREE_WINPOINT_MULTIPLIER +
             b.white_count * THEROCK_EVALUATOR_WHITE_PAWN_MULTIPLIER +
             b.black_count * THEROCK_EVALUATOR_BLACK_PAWN_MULTIPLIER +
             white_winpoints * THEROCK_EVALUATOR_WHITE_WINPOINT_MULTIPLIER +
             black_winpoints * THEROCK_EVALUATOR_BLACK_WINPOINT_MULTIPLIER +
             black_high_risk * THEROCK_EVALUATOR_BLACK_HIGH_RISK_MULTIPLIER +
             black_risk * THEROCK_EVALUATOR_BLACK_RISK_MULTIPLIER;


    // Consider the amount of cells that surround the king
    score += calculate_surrounded_penality(b.black_cols, b.black_rows, b.king_pos.col, b.king_pos.row);

    // Consider the position weight
    score += position_weight[b.king_pos.col][b.king_pos.row];

    return score;
}

int
TheRockWhiteEvaluator::perform_search(const uint16_t *cols, const uint16_t *rows, int depth, int king_col, int king_row,
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
            return THEROCK_EVALUATOR_SEARCH_WIN_POTENTIAL_MULTIPLIER * depth * 2;
        } else if (king_col - low_moves == 0 || king_col + high_moves == 8) {  // Single side winning
            return THEROCK_EVALUATOR_SEARCH_WIN_POTENTIAL_MULTIPLIER * depth;
        }

        for (int i = (king_col + 1); i <= (king_col + high_moves); i++) {
            high_score = std::max(perform_search(cols, rows, depth - 1, i, king_row, false), high_score);
        }
        for (int i = (king_col - 1); i >= (king_col - low_moves); i--) {
            low_score = std::max(perform_search(cols, rows, depth - 1, i, king_row, false), low_score);
        }
    } else {
        int high_moves = BitUtils::get_high_moves(cols[king_col], king_row);
        int low_moves = BitUtils::get_low_moves(cols[king_col], king_row);

        // Check if the king is in a win position
        if (king_row - low_moves == 0 && king_row + high_moves == 8) {  // Double side winning
            return THEROCK_EVALUATOR_SEARCH_WIN_POTENTIAL_MULTIPLIER * depth * 2;
        } else if (king_row - low_moves == 0 || king_row + high_moves == 8) {  // Single side winning
            return THEROCK_EVALUATOR_SEARCH_WIN_POTENTIAL_MULTIPLIER * depth;
        }

        for (int i = (king_row + 1); i <= (king_row + high_moves); i++) {
            high_score = std::max(perform_search(cols, rows, depth - 1, king_col, i, true), high_score);
        }
        for (int i = (king_row - 1); i >= (king_row - low_moves); i--) {
            low_score = std::max(perform_search(cols, rows, depth - 1, king_col, i, true), low_score);
        }
    }

    return std::max(low_score, high_score);
}

int TheRockWhiteEvaluator::calculate_surrounded_penality(const uint16_t *cols, const uint16_t *rows, int king_col,
                                                         int king_row) const {

    int horizontal_surroundings = BitUtils::get_surrounded(rows[king_row] | citadels_mask[king_row], king_col);
    int vertical_surroundings = BitUtils::get_surrounded(cols[king_col] | citadels_mask[king_col], king_row);

    return horizontal_surroundings * THEROCK_EVALUATOR_BLACK_SURROUNDED_MULTIPLIER +
           vertical_surroundings * THEROCK_EVALUATOR_BLACK_SURROUNDED_MULTIPLIER;
}

int TheRockWhiteEvaluator::calculate_white_winpoints(const Board &b) const {
    int count = 0;
    count += BitUtils::popcount(b.white_rows[0] & 0b0000000'011000110);
    count += BitUtils::popcount(b.white_rows[8] & 0b0000000'011000110);
    count += BitUtils::popcount(b.white_cols[0] & 0b0000000'011000110);
    count += BitUtils::popcount(b.white_cols[8] & 0b0000000'011000110);
    return count;
}

int TheRockWhiteEvaluator::calculate_black_winpoints(const Board &b) const {
    int count = 0;
    count += BitUtils::popcount(b.black_rows[0] & 0b0000000'011000110);
    count += BitUtils::popcount(b.black_rows[8] & 0b0000000'011000110);
    count += BitUtils::popcount(b.black_cols[0] & 0b0000000'011000110);
    count += BitUtils::popcount(b.black_cols[8] & 0b0000000'011000110);
    return count;
}



int TheRockWhiteEvaluator::calculate_black_count_mask(const Board &b, const uint16_t *mask) const {
    int count = 0;
    for (int i = 0; i<9; i++) {
        count += BitUtils::popcount(b.black_cols[i] & mask[i]);
    }
    return count;
}
