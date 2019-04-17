//
// Created by max on 10/04/19.
//

#ifndef OSARRACINO_BOARD_H
#define OSARRACINO_BOARD_H
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <bitset>
#include "Position.h"

const int DIM = 9;

enum Pawn {
    Empty = 0,
    White = 1 << 0,
    Black = 1 << 1,
    King  = 1 << 2,
    EmptyThrone = 1 << 3,
    EmptyCitadel = 1 << 4,
    WinPoint = 1 << 5,
    BlackWinPoint = WinPoint | Black,
    WhiteWinPoint = WinPoint | White,
    FullCitadel = Black | EmptyCitadel,
    FullThrone = EmptyThrone | King,
    ClearPawn = ~(White | Black | King),
    SelectPawn = (White | Black | King),
    KingOrWhite = (White | King),
    WinCondition = King | WinPoint
};

inline Pawn operator~ (Pawn a) { return (Pawn)~(int)a; }
inline Pawn operator| (Pawn a, Pawn b) { return (Pawn)((int)a | (int)b); }
inline Pawn operator& (Pawn a, Pawn b) { return (Pawn)((int)a & (int)b); }
inline Pawn operator^ (Pawn a, Pawn b) { return (Pawn)((int)a ^ (int)b); }
inline Pawn& operator|= (Pawn& a, Pawn b) { return (Pawn&)((int&)a |= (int)b); }
inline Pawn& operator&= (Pawn& a, Pawn b) { return (Pawn&)((int&)a &= (int)b); }
inline Pawn& operator^= (Pawn& a, Pawn b) { return (Pawn&)((int&)a ^= (int)b); }

class Board {
public:
    /*CONSTRUCTORS*/

    Board();

    static Board from_board(Board b, const Position &from, const Position &to);

    /*ATTRIBUTES*/

    // Main data structure to represent the board
    Pawn board[DIM][DIM];
    // Bool used to track which player turn is.
    bool is_white;
    Position king_pos;
    Position last_move;


    void load_board(const std::string &json_board);

    friend std::ostream& operator<<(std::ostream &s, const Board &board){
        s << "BOARD - Turn: " << board.is_white << std::endl;
        s << "╔═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╗" << std::endl;
        for (int y = 0; y < DIM; y++) {
            for (int x = 0; x < DIM; x++) {
                if ((board.board[x][y] & White) > 0) {
                    s << "║▓▓▓";
                }else if ((board.board[x][y] & Black ) > 0) {
                    s << "║░░░";
                }else if ((board.board[x][y] & King) > 0) {
                    s << "║ ® ";
                }else{
                    s << "║   ";
                }
            }
            s << "║ " << (y + 1) << std::endl;
        }
        s << "╚═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╝" << std::endl;
        s << "  A   B   C   D   E   F   G   H   I" << std::endl;
        return s;
    }

};


#endif //OSARRACINO_BOARD_H
