//
// Created by max on 24/04/19.
//

#include "ZobristHash.h"

ZobristHash::ZobristHash() {

    //Init the table with random bitstrings
}

uint16_t ZobristHash::hash(const Board &b) const {
    uint16_t h = 0;
    for (int i = 0; i < 81; i++) {
        Position pos = {i/9, i%9};
        if (b.board[pos.col][pos.row] != Pawn::Empty &&
            b.board[pos.col][pos.row] != Pawn::EmptyThrone &&
            b.board[pos.col][pos.row] != Pawn::EmptyCitadel) {


            /*Qua c'è da fare una mappa al post di sto coso*/
            int j;
            if (b.board[pos.col][pos.row] == Pawn::King) {
                j = king;
            } else if (b.board[pos.col][pos.row] == Pawn::White) {
                j = white_pawn;
            } else {
                j = black_pawn;
            }
            h = h ^ table[i][j];
        }
    }
    return h;
}
