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

    /**
     * Check if the current board is a winning board for the black pawns
     * @return true if winning for blacks, false otherwise.
     */
    inline bool is_black_win() const {
        // Make sure the last move is a black pawn
        if ((board[last_move.col][last_move.row] & Black) == 0) {
            return false;
        }

        // Check if the last move is surrounding the king
        if ((last_move.col == king_pos.col &&
             (last_move.row == king_pos.row - 1 || last_move.row == king_pos.row + 1)
            ) ||
            (last_move.row == king_pos.row &&
             (last_move.col == king_pos.col - 1 || last_move.col == king_pos.col + 1)
            )) {

            // Check the cases based on the king position
            if (king_pos.col == 4 && king_pos.row == 4) {  // King in throne
                return (board[4][3] & Black) != 0 &&
                       (board[5][4] & Black) != 0 &&
                       (board[4][5] & Black) != 0 &&
                       (board[3][4] & Black) != 0;
            } else if (adiacent_throne[king_pos.col][king_pos.row]) {  // King adiacent throne
                return (board[king_pos.col + 1][king_pos.row] & (Black | EmptyThrone)) != 0 &&
                       (board[king_pos.col - 1][king_pos.row] & (Black | EmptyThrone)) != 0 &&
                       (board[king_pos.col][king_pos.row + 1] & (Black | EmptyThrone)) != 0 &&
                       (board[king_pos.col][king_pos.row - 1] & (Black | EmptyThrone)) != 0;
            } else {  // Everywhere else
                // Left eat
                if (last_move.col > 1) {
                    if ((board[last_move.col - 1][last_move.row] & King) != 0 &&
                        (board[last_move.col - 2][last_move.row] & (Black | EmptyCitadel | EmptyThrone)) != 0) {
                        return true;
                    }
                }
                // Right eat
                if (last_move.col < 7) {
                    if ((board[last_move.col + 1][last_move.row] & King) != 0 &&
                        (board[last_move.col + 2][last_move.row] & (Black | EmptyCitadel | EmptyThrone)) != 0) {
                        return true;
                    }
                }
                // Up eat
                if (last_move.row > 1) {
                    if ((board[last_move.col][last_move.row - 1] & King) != 0 &&
                        (board[last_move.col][last_move.row - 2] & (Black | EmptyCitadel | EmptyThrone)) != 0) {
                        return true;
                    }
                }
                // Down eat
                if (last_move.row < 7) {
                    if ((board[last_move.col][last_move.row + 1] & King) != 0 &&
                        (board[last_move.col][last_move.row + 2] & (Black | EmptyCitadel | EmptyThrone)) != 0) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    /**
     * Check if the current board is a winning board for the white panws.
     * @return true if winning for whites, false otherwise.
     */
    inline bool is_white_win() const{
        // Check if the king is in a win point
        return winpoints[king_pos.col][king_pos.row];
    }

};


#endif //OSARRACINO_BOARD_H
