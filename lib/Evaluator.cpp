//
// Created by max on 12/04/19.
//

#include "Evaluator.h"

// Consts used to represents the weights of the evaluation function.
const int PESO_PEDINE = 1;
const int MAX = 50;
const int MIN = -MAX;
const int COVERING = 10;
const int KING_OPTIONS = -2;
const int PAWN_OPT = 2;
const int ISOLATE = -1;
const int WIN_COND = MAX * 2;
const int BEHIND = -5;
const int MAX_OPT = 40;
const int DEVELOPED = 4;
const int DIAG_CONNECTION = 10;

// Input: Board and a position of a pawn.
// Output: The number of directions that pawn can move to.
int Evaluator::evaluate_pawn_opt(Board &b, const Action::Position &pos) {
    int pawn_opt = 0;

    for (int i = -1; i < 2; i += 2) {
        if (pos.column + i > 0 && pos.column + i < 9) {
            if ((b.empty[pos.row].test(pos.column + i))) {
                pawn_opt++;
            }
        } else {
            continue;
        }
    }

    for (int j = -1; j < 2; j += 2) {
        if (pos.row + j > 0 && pos.row + j < 9) {
            if ((b.empty[pos.row + j].test(pos.column))) {
                pawn_opt++;
            }
        }
        else {
            continue;
        }
    }

    return pawn_opt;
}

bool Evaluator::evaluate_black_win(Board &b) {

}

// Input: Board object
// Ouput: A score of the formation of the board based on matrix coefficients.
// This approach could be usefull to make some geometries with the pawns.
int Evaluator::eval_formation(Board &b) {
    int i = 0;
    int result = 0;
    for (const auto &row: b.pawns) {
        for (int j = 0; j < 9; j++) {
            result += row.test(j) * matrix[j][i];
        }
        i++;
    }
    return result;
}

// Input: Board object
// Output: Total Score of the board
// This is the black evaluation function.
int Evaluator::evaluate_black(Board &b) {
    int king_opt {evaluate_pawn_opt(b, b.kingPos)};


    int pawns_opt = 0;
    for(const auto &toMove: b.to_be_moved) {
        pawns_opt += evaluate_pawn_opt(b, toMove);
    }

    //TODO: Implement eat-moves.
    return PESO_PEDINE * b.to_be_moved.size() +
           BEHIND*(b.to_be_moved.size()-7 - b.opposite_pawns) +
           PAWN_OPT * pawns_opt - MAX_OPT -
           KING_OPTIONS * king_opt;
}

int Evaluator::evaluate_white(Board &board) {
    if (board.board[board.kingPos.column][board.kingPos.row] == Pawn::WinCondition) {
        return 10000;
    }

    if (board.board[board.kingPos.column+1][board.kingPos.row] == Pawn::Black &&
        board.board[board.kingPos.column-1][board.kingPos.row] == Pawn::Black) {
        if (board.board[board.kingPos.column][board.kingPos.row] == Pawn::FullThrone) {
            return -5000;
        }else{
            return -10000;
        }
    }

    if (board.board[board.kingPos.column][board.kingPos.row-1] == Pawn::Black &&
        board.board[board.kingPos.column][board.kingPos.row+1] == Pawn::Black) {
        if (board.board[board.kingPos.column][board.kingPos.row] == Pawn::FullThrone) {
            return -5000;
        }else{
            return -10000;
        }
    }

    if (board.board[board.kingPos.column][board.kingPos.row-1] == Pawn::Black &&
        board.board[board.kingPos.column][board.kingPos.row+1] == Pawn::Black &&
        board.board[board.kingPos.column+1][board.kingPos.row] == Pawn::Black &&
        board.board[board.kingPos.column-1][board.kingPos.row] == Pawn::Black) {

        return -10000;
    }

    return 100;
}


