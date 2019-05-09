//
// Created by freddy on 15/04/19.
//

#include "BlackEvaluator.h"
#include <set>
#include <bitset>
#include <algorithm>
#include <util/BoardUtils.h>

#define  FIRST_QUARTER 4,8,0,4
#define  SECOND_QUARTER 4,8,4,8
#define  THIRD_QUARTER 0,4,4,8
#define  FOURTH_QUARTER 0,4,0,4

//If the blacks is near the king -> bonus points!
int BlackEvaluator::black_block_king(const Board &b) const {
    int blocks {0};

    auto check_pos = [&b](const Position &pos) ->bool {
        return b.board[pos.col][pos.row] == Pawn::Black ||
               b.board[pos.col][pos.row] == Pawn::EmptyCitadel ||
               b.board[pos.col][pos.row] == Pawn::FullCitadel;
    };

    if (check_pos(Position{b.king_pos.col, b.king_pos.row-1})) { //UP
        blocks++;
    }
    if (check_pos(Position{b.king_pos.col, b.king_pos.row+1})) { //DOWN
        blocks++;
    }
    if (check_pos(Position{b.king_pos.col-1, b.king_pos.row})) { //LEFT
        blocks++;
    }
    if (check_pos(Position{b.king_pos.col+1, b.king_pos.row})) { // RIGHT
        blocks++;
    }

    return blocks;
}

//Differences of pawns based on quarters! TO TEST
int BlackEvaluator::pawn_differences(const Board &b) const {
    return b.black_count - b.white_count - 1;
}


std::vector<Direction> BlackEvaluator::get_direction_of_move_check(const Board &b) const {

    //King not in place to win.
    auto col = std::find(win_rows_cols.begin(), win_rows_cols.end(), b.king_pos.col);
    auto row = std::find(win_rows_cols.begin(), win_rows_cols.end(), b.king_pos.row);

    std::vector<Direction> positions;

    // Check obstacle, if a part contains obstacles don't checkit.

    if (row == win_rows_cols.end() && col == win_rows_cols.end()) {
        return positions;
    }

    if (col != win_rows_cols.end()) { // Re su una riga possibilmente vincente quindi o destra o sinistra
        for (int i = b.king_pos.row+1; i < 9 && (b.board[b.king_pos.col][i] == Pawn::Empty ||b.board[b.king_pos.col][i] == Pawn::WinPoint); i++) {
            if (i == 8) {
                positions.push_back(Direction::Down);
            }
        }
        for (int i = b.king_pos.row - 1; i >=0 && (b.board[b.king_pos.col][i] == Pawn::Empty ||b.board[b.king_pos.col][i] == Pawn::WinPoint); i--) {
            if (i == 0) {
                positions.push_back(Direction::Up);
            }
        }
    }

    if (row != win_rows_cols.end()) { //Re su una colonna vincente quindi o su o giu
        for (int i = b.king_pos.col+1; i < 9 && (b.board[i][b.king_pos.row] == Pawn::Empty ||b.board[i][b.king_pos.row] == Pawn::WinPoint); i++) {
            if (i == 8) {
                positions.push_back(Direction::Right);
            }
        }

        for (int i = b.king_pos.col-1; i >=0 && (b.board[i][b.king_pos.row] == Pawn::Empty ||b.board[i][b.king_pos.row] == Pawn::WinPoint); i--) {
            if (i == 0) {
                positions.push_back(Direction::Left);
            }
        }
    }
    return positions;

}


// La color matrix ora si baserà sulla presenza nel quadrante del re!

int BlackEvaluator::evaluate(const Board &b) const {
    //Block the king must be relational in time
    int block_weight = 1;
    int block_the_king = black_block_king(b);

    if(b.is_black_win()) {
        return EZPZ;
    } else if (b.is_white_win()) {
        return -EZPZ;
    } else {
        int geometry = geometry_points(b);
        if (geometry >= 3) {
            block_the_king = 2;
        }
        return (block_the_king * block_weight) +
               2*pawn_differences(b) +
               geometry+
               PREVENT_CHECKMATE *
               (get_empty_col_left(b)+
                get_empty_col_right(b)+
                get_empty_row_down(b)+
                get_empty_row_up(b));
    }

}

int BlackEvaluator::geometry_points(const Board &b) const {

    if (b.king_pos.row < 4 && b.king_pos.col < 4) { //TOP LEFT
        return geometry_calculator(b, top_left_color_matrix);

    } else if (b.king_pos.row < 4 && b.king_pos.col > 4) { //TOP RIGHT
        return geometry_calculator(b, top_right_color_matrix);

    } else if (b.king_pos.row > 4 && b.king_pos.col > 4) { //BOTTOM RIGHT
        return geometry_calculator(b, bottom_right_color_matrix);

    } else if (b.king_pos.row > 4 && b.king_pos.col < 4) { // BOTTOM LEFT
        return geometry_calculator(b, bottom_left_color_matrix);

    } else if (b.king_pos.row == 4 && b.king_pos.col < 4) { // LEFT
        return geometry_calculator(b, left_color_matrix);

    } else if (b.king_pos.row == 4 && b.king_pos.col > 4) { // RIGHT
        return geometry_calculator(b, right_color_matrix);

    } else if (b.king_pos.row < 4 && b.king_pos.col == 4) { // TOP
        return geometry_calculator(b, top_color_matrix);

    } else if (b.king_pos.row > 4 && b.king_pos.col == 4) { // DOWN
        return geometry_calculator(b, bottom_color_matrix);

    } else {
        return geometry_calculator(b, color_matrix);
    }
}


// Da considerare colonna 2 colonna 6
// Da considerare riga 2 riga 6


int BlackEvaluator::get_empty_row_down(const Board &b) const {
    //Vedere se il king può arrivare a riga 1,2,6,7!
    int row_counter = 0;
    for (int i = b.king_pos.row+1; i < 7 ; i++) { //UPSIDE
        if(b.board[b.king_pos.col][i] == Pawn::Empty) {
            row_counter = i;
        }
    }

    if (row_counter != 6) {
        return 0;
    }

    for(int i = 0; i < 9; i++) {
        if (b.board[i][6] != Pawn::Empty) {
            return 0;
        }
    }
    return 1;
}

int BlackEvaluator::get_empty_row_up(const Board &b) const {
    int row_counter = b.king_pos.row;
    for (int i = b.king_pos.row-1; i > 1; i--) { //DOWNSIDE
        if(b.board[b.king_pos.col][i] == Pawn::Empty) {
            row_counter = i;
        }
    }
    if (row_counter != 2) {
        return 0;
    }

    for(int i = 0; i < 9; i++) {
        if (b.board[i][2] != Pawn::Empty) {
            return 0;
        }
    }

    return 1;
}

int BlackEvaluator::get_empty_col_right(const Board &b) const {
    int col_counter = b.king_pos.col;
    for (int i = b.king_pos.col+1; i < 7; i++) { //TO THE RIGHT
        if(b.board[i][b.king_pos.row] == Pawn::Empty) {
            col_counter = i;
        }
    }

    if (col_counter != 6) {

        return 0;
    }

    for(int i = 0; i < 9; i++) {
        if (b.board[6][i] != Pawn::Empty) {
            return 0;
        }
    }
    return 1;
}


int BlackEvaluator::get_empty_col_left(const Board &b) const {
    int col_counter = b.king_pos.col;
    for (int i = b.king_pos.col-1; i > 1 ; i--) { //TO THE LEFT
        if(b.board[i][b.king_pos.row] == Pawn::Empty) {
            col_counter = i;
        }
    }

    if (col_counter != 1) {
        return 0;
    }

    for(int i = 0; i < 9; i++) {
        if (b.board[2][i] != Pawn::Empty) {
            return 0;
        }
    }
    return 1;
}


BlackEvaluator::BlackEvaluator() {
    geometry_calculator = [](const Board &b, const uint8_t (&matrix)[9][9]) ->int {
        int result {0};
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (b.board[i][j] == Pawn::Black) {
                    result += matrix[i][j];
                }
            }
        }
        return result;
    };

}



