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


bool BlackEvaluator::is_king_in_throne(const Board &b) const{
    return b.board[b.king_pos.col][b.king_pos.row] == Pawn::FullThrone;
}

Direction BlackEvaluator::is_king_near_throne(const Board &b) const {
    auto it = near_throne.find(b.king_pos);
    if (it != near_throne.end()) {
        return it->second;
    }
    return Direction::None;
}

bool BlackEvaluator::throne_win_condition(const Board &b) const {
        return b.board[b.king_pos.row][b.king_pos.col+1] == Pawn::Black &&      //RIGHT
               b.board[b.king_pos.row][b.king_pos.col-1] == Pawn::Black &&      //LEFT
               b.board[b.king_pos.row-1][b.king_pos.col] == Pawn::Black &&      //UP
               b.board[b.king_pos.row+1][b.king_pos.col] == Pawn::Black;        //DOWN

}

bool BlackEvaluator::near_throne_win_condition(const Board &b) const {
    Direction dir = is_king_near_throne(b);
    if (dir == Direction::None) {
        return false;
    } else {
        return near_checks.at(dir)(b);
    }

}

bool BlackEvaluator::simple_win_condition(const Board &b) const {
    //Se il re non è sul trono o adiacente
    return ((!is_king_in_throne(b)) && (!is_king_in_throne(b))) &&

           (((b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::Black
              || b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::BlackWinPoint
              || b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::FullCitadel
              || b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::EmptyCitadel)
             &&
             (b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::Black
              || b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::BlackWinPoint
              || b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::FullCitadel
              || b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::EmptyCitadel))

            ||

            ((b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::Black
              || b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::BlackWinPoint
              || b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::FullCitadel
              || b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::EmptyCitadel)
             &&
             (b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::Black
              || b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::FullCitadel
              || b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::BlackWinPoint
              || b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::EmptyCitadel)));

}

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
    int current_difference {0};

    // Function that calculates the differences based on the cols;
    auto counter = [&b](int start_col, int max_col, int start_row,int max_row) -> int {
        int black {0};
        int white {0};
        for (int col = start_col; col <= max_col; col++) {
            for (int row = start_row; row <= max_row; row++) {
                if (b.board[col][row] == Pawn::White || b.board[col][row] == Pawn::FullThrone || b.board[col][row] == Pawn::King) {
                    white++;
                } else if (b.board[col][row] == Pawn::Black || b.board[col][row] == Pawn::FullCitadel) {
                    black++;
                }
            }
        }
        return black - white - 1;
    };

    current_difference += counter(FIRST_QUARTER);
    current_difference += counter(SECOND_QUARTER);
    current_difference += counter(THIRD_QUARTER);
    current_difference += counter(FOURTH_QUARTER);

    return current_difference;
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


// TODO: Block the king based on the turns
// TODO: Add coverage for the rows and columns for every quarter using the color matrix
// TODO: Coverage based on white moves.


BlackEvaluator::BlackEvaluator() {
    near_throne[Position{5,4}] = Direction::Right;
    near_throne[Position{3,4}] = Direction::Left;
    near_throne[Position{4,3}] = Direction::Up;
    near_throne[Position{4,5}] = Direction::Down;

    near_checks[Direction::Right] = [](const Board &b) -> bool {
        return (b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::Black &&   //UP
                b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::Black &&   //DOWN
                b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::Black      //RIGHT
        );
    };
    near_checks[Direction::Left] = [](const Board &b) -> bool {
        return (b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::Black &&   //UP
                b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::Black &&   //DOWN
                b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::Black      //LEFT
        );
    };
    near_checks[Direction::Up] = [](const Board &b) -> bool {
        return (b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::Black &&   //UP
                b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::Black &&   //LEFT
                b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::Black      //RIGHT
        );
    };
    near_checks[Direction::Down] = [](const Board &b) -> bool {
        return (b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::Black &&   //LEFT
                b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::Black &&   //DOWN
                b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::Black      //RIGHT
        );
    };

    is_moved_near = [](const Board &b , const Position &pos) -> bool {
        return (b.last_move == Position{pos.col+1, pos.row} || //RIGHT
                b.last_move == Position{pos.col-1, pos.row} || //LEFT
                b.last_move == Position{pos.col, pos.row-1} || //UP
                b.last_move == Position{pos.col, pos.row+1}    //DOWN
        );
    };
}

int BlackEvaluator::evaluate(const Board &b) const {
    bool win_move = false;

    int block_the_king = black_block_king(b);

    if (is_moved_near(b, b.king_pos)) { //Check if blacks can win can win
        if (is_king_in_throne(b)) {
            win_move = throne_win_condition(b);
        } else if (is_king_near_throne(b)) {
            win_move = near_throne_win_condition(b);
        } else {
            win_move = simple_win_condition(b);
        }
    }

    if (win_move) { //We can win
        return EZPZ;
    } else { // We cannot win, so we have to defend, insert here all state calculation
        return -EZPZ * get_direction_of_move_check(b).size()
        + block_the_king
        + pawn_differences(b)
        + geometry_points(b)
        + get_empty_row(b) * PREVENT_CHECKMATE
        + get_empty_col(b) * PREVENT_CHECKMATE;
    }
}

int BlackEvaluator::geometry_points(const Board &b) const {
    int result {0};
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (b.board[i][j] == Pawn::Black) {
                result += color_matrix[i][j];
            }
        }
    }
    return result;
}


// Da considerare colonna 2 colonna 6
// Da considerare riga 2 riga 6

int BlackEvaluator::get_empty_row(const Board &b) const {
    bool isEmpty2 = true;
    bool isEmpty6 = true;
    for (int i = 0; i < 9 && isEmpty2; i++) {
        isEmpty2 = isEmpty2 && b.board[2][i] == Pawn::Empty;
    }
    for (int i = 0; i < 9 && isEmpty6; i++) {
        isEmpty6 = isEmpty6 && b.board[6][i] == Pawn::Empty;
    }
    return isEmpty2 || isEmpty6 ? 1: 0;
}

int BlackEvaluator::get_empty_col(const Board &b) const {
    bool isEmpty2 = true;
    bool isEmpty6 = true;
    for (int i = 0; i < 9 && isEmpty2; i++) {
        isEmpty2 = isEmpty2 && b.board[i][2] == Pawn::Empty;
    }
    for (int i = 0; i < 9 && isEmpty6; i++) {
        isEmpty6 = isEmpty6 && b.board[i][6] == Pawn::Empty;
    }
    return isEmpty2 || isEmpty6 ? 1 : 0;
}



