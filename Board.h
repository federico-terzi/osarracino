//
// Created by max on 10/04/19.
//

#ifndef OSARRACINO_BOARD_H
#define OSARRACINO_BOARD_H
#include <string>

const int DIM = 9;

class Board {
public:
    char board[DIM][DIM];
    void load_board(const std::string &json_board);

};


#endif //OSARRACINO_BOARD_H
