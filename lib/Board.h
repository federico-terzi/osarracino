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

const int DIM = 9;

namespace Pawn {
    // Consts used to model the content of a cell in the board.
    const char White = 1 << 0;
    const char Black = 1 << 1;
    const char King = 1 << 2;
    const char EmptyThrone = 1 << 3;
    const char EmptyCitadel = 1 << 4;
    const char WinPoint = 1 << 5;
    const char FullCitadel = Black | EmptyCitadel;
    const char FullThrone = EmptyThrone | King;
    const char WinCondition = King | WinPoint;

    // Map used to print out in a clever way the board
    static std::map<char,std::string> names = {{0,"║   "}, {White, "║▓▓▓"}, {Black, "║░░░"}, {King, "║ ® "},
                                               {EmptyThrone, "║ + "}, {EmptyCitadel, "║ O "}, {WinPoint, "║   "},
                                               {FullThrone, "║▓▓▓"}, {FullCitadel, "║░░░"}, {WinCondition, "║WWW"}};
}

namespace Action {
    // Map used to print columns as characters not as integer
    // Usefull also to create the move json to send to the server.
    static std::map<int, char> toCol = {
            {0,'a'},
            {1,'b'},
            {2,'c'},
            {3,'d'},
            {4,'e'},
            {5,'f'},
            {6,'g'},
            {7,'h'},
            {8,'i'},
    };

    // Struct used to represents a Position in the board
    struct Position {
        int column;
        int row;

        friend bool operator==(const Position &lhs, const Position &rhs);
        friend std::ostream &operator<<(std::ostream &s, const Position &pos) {
            s << "COL: " << toCol[pos.column] << " ROW: " << pos.row;
        };
        std::string to_move();
    };
}

class Board {
public:
    /*CONSTRUCTORS*/

    Board();
    static Board from_board(Board b, Action::Position &from, Action::Position &to);

    /*ATTRIBUTES*/

    // Main data structure to represent the board
    char board[DIM][DIM];
    // Bool used to track which player turn is.
    bool is_white;
    // int that tracks the number of enemy pawns.
    int opposite_pawns = 0;
    // Used to store the king position.
    Action::Position kingPos;
    // 9x9 Matrix used to track the empty spaces in the board.
    std::bitset<9> empty[9];
    // 9x9 Matrix used to track the position of pawn of the current turn player.
    // Probably not needed because we have the vector -- to be moved--.
    std::bitset<9> pawns[9];
    // Vector of the pawns that could to be moved.
    std::vector<Action::Position> to_be_moved;

    void load_board(const std::string &json_board);
    friend std::ostream& operator<<(std::ostream &s, const Board &board){
        s << "BOARD" << std::endl;
        s << "╔═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╗" << std::endl;
        for (int y = 0; y < DIM; y++) {
            for (int x = 0; x < DIM; x++) {
                if ((board.board[x][y] & Pawn::White) != 0) {
                    s << "║▓▓▓";
                }else if ((board.board[x][y] & Pawn::Black) != 0) {
                    s << "║░░░";
                }else if ((board.board[x][y] & Pawn::King) != 0) {
                    s << "║ ® ";
                }else{
                    s << "║   ";
                }
                //s << Pawn::names[board.board[x][y]];
            }
            s << "║ " << (y + 1) << std::endl;
        }
        s << "╚═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╝" << std::endl;
        s << "  A   B   C   D   E   F   G   H   I" << std::endl;
    }

};



#endif //OSARRACINO_BOARD_H
