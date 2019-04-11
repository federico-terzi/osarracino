//
// Created by max on 10/04/19.
//

#ifndef OSARRACINO_BOARD_H
#define OSARRACINO_BOARD_H
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <unordered_map>

const int DIM = 9;
const int OFFSET = 10;

namespace Pawn {
    const char White = 1 << 0;
    const char Black = 1 << 1;
    const char King = 1 << 2;
    const char EmptyThrone = 1 << 3;
    const char EmptyCitadel = 1 << 4;
    const char WinPoint = 1 << 5;
    const char FullCitadel = Black | EmptyCitadel;
    const char FullThrone = EmptyThrone | King;
    const char WinCondition = King | WinPoint;

    static std::map<char,std::string> names = {{0,"║   "}, {White, "║▓▓▓"}, {Black, "║░░░"}, {King, "║ ® "}};
}

namespace Action {
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

    struct Position {
        int column;
        int row;
        friend bool operator==(const Position& lhs, const Position& rhs);
    };

}

class Board {
public:
    Board();
    char board[DIM][DIM];
    bool is_white;
    std::vector<Action::Position> to_be_moved;
    void load_board(const std::string &json_board);
    std::vector<Action::Position> get_legal_moves(const Action::Position &pos);

    friend std::ostream& operator<<(std::ostream &s, const Board &board){
        s << "BOARD" << std::endl;
        s << "╔═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╗" << std::endl;
        for (int y = 0; y < DIM; y++) {
            for (int x = 0; x < DIM; x++) {
                s << Pawn::names[board.board[x][y]];
            }
            s << "║ " << (y + 1) << std::endl;
        }
        s << "╚═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╝" << std::endl;
        s << "  A   B   C   D   E   F   G   H   I" << std::endl;
    }

};



#endif //OSARRACINO_BOARD_H
