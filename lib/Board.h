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
    struct Position {
        char row;
        int column;
        bool operator==(const Position &a);
    };
    static std::map<int, char> toCol = {
            {1,'a'},
            {2,'b'},
            {3,'c'},
            {4,'d'},
            {5,'e'},
            {6,'f'},
            {7,'g'},
            {8,'h'},
            {9,'i'},
            };
}


class Board {
public:
    char board[DIM][DIM];
    bool is_white;
    std::unordered_map<int, std::vector<int>> moves;
    std::vector<Action::Position> to_be_moved;
    void load_board(const std::string &json_board);
    std::vector<Action::Position> get_legal_moves(Action::Position pos);

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
