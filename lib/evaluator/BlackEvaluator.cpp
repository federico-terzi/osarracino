//
// Created by freddy on 15/04/19.
//

#include "BlackEvaluator.h"
#include <set>
#include <bitset>
#include <algorithm>
#include <util/BoardUtils.h>


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
    if (is_king_in_throne(b)) {
        return b.board[b.king_pos.row][b.king_pos.col+1] == Pawn::Black &&      //RIGHT
               b.board[b.king_pos.row][b.king_pos.col-1] == Pawn::Black &&      //LEFT
               b.board[b.king_pos.row-1][b.king_pos.col] == Pawn::Black &&      //UP
               b.board[b.king_pos.row+1][b.king_pos.col] == Pawn::Black;        //DOWN

    }
    return false;
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
    return (!is_king_in_throne(b)) && (!is_king_in_throne(b)) &&

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


int BlackEvaluator::evaluate(const Board &b) const {
    int geometry_points {0};
    int row_covering_points {0};
    int col_covering_points {0};
    bool win_move = false;
    std::set<int> row_covered, col_covered;


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            //Calcolo della geometria
            if (b.board[i][j] == Pawn::Black || b.board[i][j] == Pawn::FullCitadel) {
                //  geometry_points += color_matrix[i][j];
                row_covered.insert(i);
                col_covered.insert(j);
                //TODO: This is not global covering, for global covering we must consider every quarter of the board.
            }
        }
    }

    // Calculate if all rows are covered
    if (row_covered.size() < 9) {
        row_covering_points = (row_covered.size() - 9) * ALL_ROWS_COVERED;
    } else {
        row_covering_points = ALL_ROWS_COVERED;
    }

    if (col_covered.size() < 9) {
        col_covering_points = (col_covered.size() - 9) * ALL_COLS_COVERED;
    } else {
        col_covering_points = ALL_COLS_COVERED;
    }

    if (is_moved_near(b, b.king_pos)) {
        win_move = simple_win_condition(b) || near_throne_win_condition(b) || throne_win_condition(b);
    }

    return geometry_points + row_covering_points + col_covering_points + EZPZ * win_move - EZPZ * get_direction_of_move_check(b).size();
}

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

//TODO: Tests of the positions of the king.
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



