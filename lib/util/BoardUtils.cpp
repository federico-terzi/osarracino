//
// Created by max on 16/04/19.
//

#include "BoardUtils.h"

std::pair<std::vector<Position>, std::array<std::bitset<9>,9>> BoardUtils::Get_empty_and_to_move(const Board &b) {
    std::vector<Position> to_be_moved;
    std::array<std::bitset<9>,9> empty;

    for(int col = 0; col < 9; col++) {
        for (int row = 0; row < 9; row++) {
            //If is empty set the mask
            //TODO: Add winpoints.
            if((b.is_white && b.board[col][row] == Pawn::Empty) || (b.board[col][row] == Pawn::WinPoint)) {
                empty[row].set(col);
            } else if ((!b.is_white && b.board[col][row] == Pawn::EmptyCitadel) || b.board[col][row] == Pawn::Empty || b.board[col][row] == Pawn::WinPoint) {
                empty[row].set(col);
                //Is white turn and is or a simple white or a king then add that position
            } else if(b.is_white && (b.board[col][row] == Pawn::White || b.board[col][row] == Pawn::King || b.board[col][row] == Pawn::FullThrone
                                     || b.board[col][row] == Pawn::WhiteWinPoint)) {
                to_be_moved.push_back(Position{col, row});

                //Is black turn and is a simple black or a black in a citadel then add that position
            } else if(!b.is_white && (b.board[col][row] == Pawn::Black || b.board[col][row] == Pawn::FullCitadel ||
                                      b.board[col][row] == Pawn::BlackWinPoint)) {
                to_be_moved.push_back(Position{col, row});
            }
        }
    }

    //Return the pair
    return std::pair<std::vector<Position>, std::array<std::bitset<9>,9>>(to_be_moved, empty);
}