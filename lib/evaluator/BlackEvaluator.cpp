//
// Created by freddy on 15/04/19.
//

#include "BlackEvaluator.h"
#include <set>


bool BlackEvaluator::is_king_in_throne(const Board &b) {
    return b.board[b.king_pos.col][b.king_pos.row] == Pawn::FullThrone;
}

Direction BlackEvaluator::is_king_near_throne(const Board &b) {
    auto it = near_throne.find(b.king_pos);
    if (it != near_throne.end()) {
        return it->second;
    }
    return Direction::None;
}

bool BlackEvaluator::throne_win_condition(const Board &b) {
    if (is_king_in_throne(b)) {
        return b.board[b.king_pos.row][b.king_pos.col+1] == Pawn::Black &&      //RIGHT
               b.board[b.king_pos.row][b.king_pos.col-1] == Pawn::Black &&      //LEFT
               b.board[b.king_pos.row-1][b.king_pos.col] == Pawn::Black &&      //UP
               b.board[b.king_pos.row+1][b.king_pos.col] == Pawn::Black;        //DOWN

    }
    return false;
}

bool BlackEvaluator::near_throne_win_condition(const Board &b) {
    Direction dir = is_king_near_throne(b);
    if (dir == Direction::None) {
        return false;
    } else {
        return near_checks[dir](dir, b);
    }

}

bool BlackEvaluator::simple_win_condition(const Board &b) {
    //Se il re non è sul trono o adiacente
    return (!is_king_in_throne(b)) && (! is_king_in_throne(b)) &&

           (((b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::Black
              || b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::FullCitadel
              || b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::EmptyCitadel)
             &&
             (b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::Black
              || b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::FullCitadel
              || b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::EmptyCitadel))

            ||

            ((b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::Black
              || b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::FullCitadel
              || b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::EmptyCitadel)
             &&
             (b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::Black
              || b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::FullCitadel
              || b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::EmptyCitadel)));

}


std::pair<std::vector<Position>, std::array<std::bitset<9>,9>> BlackEvaluator::Get_empty_and_to_move(const Board &b) {
    std::vector<Position> to_be_moved;
    std::array<std::bitset<9>,9> empty;

    for(int col = 0; col < 9; col++) {
        for (int row = 0; row < 9; row++) {
            //If is empty set the mask
            //TODO: Add winpoints.
            if(b.is_white && b.board[col][row] == Pawn::Empty) {
                empty[row].set(col);
            } else if ((!b.is_white && b.board[col][row] == Pawn::EmptyCitadel) || b.board[col][row] == Pawn::Empty) {
                empty[row].set(col);
                //Is white turn and is or a simple white or a king then add that position
            } else if(b.is_white && (b.board[col][row] == Pawn::White || b.board[col][row] == Pawn::King || b.board[col][row] == Pawn::FullThrone)) {
                to_be_moved.push_back(Position{col, row});

                //Is black turn and is a simple black or a black in a citadel then add that position
            } else if(!b.is_white && (b.board[col][row] == Pawn::Black || b.board[col][row] == Pawn::FullCitadel)) {
                to_be_moved.push_back(Position{col, row});
            }
        }
    }

    //Return the pair
    return std::pair<std::vector<Position>, std::array<std::bitset<9>,9>>(to_be_moved, empty);
}

int BlackEvaluator::evaluate(const Board &b) {
    int geometry_points {0};
    int row_covering_points {0};
    int col_covering_points {0};
    bool win_move = false;
    std::set<int> row_covered, col_covered;


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            //Calcolo della geometria
            if (b.board[i][j] == Pawn::Black || b.board[i][j] == Pawn::FullCitadel) {
                geometry_points += color_matrix[i][j];
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

    win_move = simple_win_condition(b) | near_throne_win_condition(b) | throne_win_condition(b);

    return geometry_points + row_covering_points + col_covering_points + EZPZ * win_move;
}

BlackEvaluator::BlackEvaluator() {
    near_throne[Position{5,4}] = Direction::Right;
    near_throne[Position{3,4}] = Direction::Left;
    near_throne[Position{4,3}] = Direction::Up;
    near_throne[Position{4,5}] = Direction::Down;

    near_checks[Direction::Right] = [](Direction dir, const Board &b) -> bool {
        return (b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::Black &&   //UP
                b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::Black &&   //DOWN
                b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::Black      //RIGHT
        );
    };
    near_checks[Direction::Left] = [](Direction dir, const Board &b) -> bool {
        return (b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::Black &&   //UP
                b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::Black &&   //DOWN
                b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::Black      //LEFT
        );
    };
    near_checks[Direction::Up] = [](Direction dir, const Board &b) -> bool {
        return (b.board[b.king_pos.col][b.king_pos.row-1] == Pawn::Black &&   //UP
                b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::Black &&   //LEFT
                b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::Black      //RIGHT
        );
    };
    near_checks[Direction::Down] = [](Direction dir, const Board &b) -> bool {
        return (b.board[b.king_pos.col-1][b.king_pos.row] == Pawn::Black &&   //LEFT
                b.board[b.king_pos.col][b.king_pos.row+1] == Pawn::Black &&   //DOWN
                b.board[b.king_pos.col+1][b.king_pos.row] == Pawn::Black      //RIGHT
        );
    };
}




