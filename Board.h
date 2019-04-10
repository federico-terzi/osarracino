//
// Created by max on 10/04/19.
//

#ifndef OSARRACINO_BOARD_H
#define OSARRACINO_BOARD_H
#include <string>
#include <iostream>
#include <map>

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



class Board {
public:
    char board[DIM][DIM];
    void load_board(const std::string &json_board);


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
