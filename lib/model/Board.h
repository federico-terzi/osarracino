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

const bool winpoints[9][9] = {
        {0, 1, 1, 0, 0, 0, 1, 1, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 1, 1, 0, 0, 0, 1, 1, 0},
};

const bool adiacent_throne[9][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
};

class Board {
public:
    /*CONSTRUCTORS*/

    Board();

    static Board from_board(Board b, const Position &from, const Position &to);
    static Board from_json(const std::string &json);
    static Board from_path(const std::string &path);

    /*ATTRIBUTES*/

    // Main data structure to represent the board
    Pawn board[DIM][DIM];
    // Bool used to track which player turn is.
    bool is_white;
    Position king_pos;
    Position last_move;

    /**
     * Check if the current board is a winning board for the black pawns
     * @return true if winning for blacks, false otherwise.
     */
    bool is_black_win() const;

    /**
     * Check if the current board is a winning board for the white panws.
     * @return true if winning for whites, false otherwise.
     */
    bool is_white_win() const;

    void load_board(const std::string &json_board);

    friend std::ostream& operator<<(std::ostream &s, const Board &board){
        std::string turn = (board.is_white) ? "WHITE" : "BLACK";
        s << "BOARD - Turn: " << turn << std::endl;
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

    bool operator==(const Board &rhs) const;

    bool operator!=(const Board &rhs) const;

};


#endif //OSARRACINO_BOARD_H
