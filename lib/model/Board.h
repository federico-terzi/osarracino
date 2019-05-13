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
#include <util/BitUtils.h>

#include "Position.h"

const int DIM = 9;

enum __attribute__ ((__packed__)) Pawn {
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

const uint16_t citadels_mask[9] = {
        0b0000000'000111000,
        0b0000000'000010000,
        0b0000000'000000000,
        0b0000000'100000001,
        0b0000000'110000011,
        0b0000000'100000001,
        0b0000000'000000000,
        0b0000000'000010000,
        0b0000000'000111000,
};

const uint16_t throne_mask[9] = {
        0b0000000'000000000,
        0b0000000'000000000,
        0b0000000'000000000,
        0b0000000'000000000,
        0b0000000'000010000,
        0b0000000'000000000,
        0b0000000'000000000,
        0b0000000'000000000,
        0b0000000'000000000,
};

const int KING_LOST = -100;

class Board {
public:
    /*CONSTRUCTORS*/

    Board();

    static Board from_board(Board b, const Position &from, const Position &to);
    static Board from_json(const std::string &json);
    static Board from_path(const std::string &path);

    uint16_t white_cols[9];
    uint16_t white_rows[9];
    uint16_t black_cols[9];
    uint16_t black_rows[9];

    uint16_t obstacle_cols[9];
    uint16_t obstacle_rows[9];

    uint8_t white_count;
    uint8_t black_count;
    uint8_t free_winpoints;

    // Main data structure to represent the board
    Pawn board[DIM][DIM];
    // Bool used to track which player turn is.
    bool is_white;
    Position king_pos;
    Position last_move;
    bool is_quiet;  // True if no pawns where eaten, false otherwise.

    inline bool has_black(int col, int row) const {
        return (board[col][row] & Pawn::Black) != 0;
    }

    inline bool has_white(int col, int row) const {
        return (board[col][row] & (Pawn::White | Pawn::King)) != 0;
    }

    inline bool has_king(int col, int row) const {
        return king_pos.col == col && king_pos.row == row;
    }

    inline bool has_white_or_wall(int col, int row) const {
        return (board[col][row] & (Pawn::White | Pawn::King | Pawn::EmptyCitadel | Pawn::EmptyThrone)) != 0;
    }

    inline bool has_citadel(int col, int row) const {
        return (board[col][row] & Pawn::EmptyCitadel) != 0;
    }

    inline bool is_king_in_throne() const {
        return king_pos.col == 4 && king_pos.row == 4;
    }

    inline bool is_king_surrounded_by_black_or_throne() const {
        uint16_t col = (black_cols[king_pos.col] | throne_mask[king_pos.col]);
        uint16_t row = (black_rows[king_pos.row] | throne_mask[king_pos.row]);
        int vertical_surrounded = BitUtils::get_surrounded(col, king_pos.row);
        int horizontal_surrounded = BitUtils::get_surrounded(row, king_pos.col);
        return vertical_surrounded + horizontal_surrounded == 4;
    }

    inline bool has_black_or_wall(int col, int row) const {
        return (board[col][row] & (Pawn::Black | Pawn::EmptyCitadel | Pawn::EmptyThrone)) != 0;
    }

    inline bool has_black_surrounded_throne() const {
        return ((black_cols[4] & 0b0000000'000101000) &
                (black_rows[4] & 0b0000000'000101000))
                == 0b0000000'000101000;
    }

    inline void move_pawn(Position from, Position to) {
        // MOVE THE ARRAY

        // Set the pawn
        Pawn pawn = board[from.col][from.row] & SelectPawn;
        board[to.col][to.row] |= pawn;

        // MOVE THE BITMASKS

        // Set the target position
        if (has_black(from.col, from.row)) {
            BitUtils::set_bit(black_cols[to.col], to.row);
            BitUtils::set_bit(black_rows[to.row], to.col);
        }else{
            BitUtils::set_bit(white_cols[to.col], to.row);
            BitUtils::set_bit(white_rows[to.row], to.col);
        }

        // Delete the pawn from the original position
        delete_pawn(from.col, from.row);

        // Update the obstacle masks
        BitUtils::set_bit(obstacle_cols[to.col], to.row);
        BitUtils::set_bit(obstacle_rows[to.row], to.col);

        // Adjust statistics
        if (has_black(to.col, to.row)) {
            black_count++;
        } else if (has_white(to.col, to.row)) {
            white_count++;
        }
        if (winpoints[to.col][to.row]) {
            free_winpoints--;
        }
    }

    inline void delete_pawn(int col, int row) {
        // DELETE FROM ARRAY

        board[col][row] &= ClearPawn;

        // DELETE FROM BITMASK

        // Adjust statistics
        if (has_black(col, row)) {
            black_count--;
        } else if (has_white(col, row)) {
            white_count--;
        }
        if (winpoints[col][row]) {
            free_winpoints++;
        }

        BitUtils::unset_bit(black_cols[col], row);
        BitUtils::unset_bit(white_cols[col], row);
        BitUtils::unset_bit(black_rows[row], col);
        BitUtils::unset_bit(white_rows[row], col);

        // Delete the pawn from the obstacles considering also citadels and throne

        BitUtils::unset_bit(obstacle_cols[col], row);
        obstacle_cols[col] |= citadels_mask[col];
        obstacle_cols[col] |= throne_mask[col];

        BitUtils::unset_bit(obstacle_rows[row], col);
        obstacle_rows[row] |= citadels_mask[row];
        obstacle_rows[row] |= throne_mask[row];
    }

    void load_board(const std::string &json_board);

    friend std::ostream& operator<<(std::ostream &s, const Board &board){
        std::string turn = (board.is_white) ? "WHITE" : "BLACK";
        s << "BOARD - Turn: " << turn << " last move: " << board.last_move << " king pos: " << board.king_pos << std::endl;
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
     * Check if the current board is a winning board for the black pawns.
     * When a black pawn eats the king, the column of the king is negative.
     * @return true if winning for blacks, false otherwise.
     */
    inline bool is_black_win() const {
        return king_pos.col < 0;
    }

    /**
     * Check if the current board is a winning board for the white panws.
     * @return true if winning for whites, false otherwise.
     */
    inline bool is_white_win() const{
        if (king_pos.col >= 0) {
            // Check if the king is in a win point
            return winpoints[king_pos.col][king_pos.row];
        }

        return false;
    }

};


#endif //OSARRACINO_BOARD_H
