//
// Created by max on 11/04/19.
//

#include "BitMaskHandler.h"

std::vector<Action::Position> BitMaskHandler::xorAndRun(std::bitset<9> set[], int col, int row) {
    std::string vert = "";
    for(int i = 0; i < 9; i++){
        vert+= set[i].to_string()[col];
    }

    std::string right = set[row].to_string().substr(col+1);
    std::string left = set[row].to_string().substr(0, col);
    std::string up = vert.substr(0, row);
    std::string down = vert.substr(row+1);

    std::vector<Action::Position> moves;
    //right
    for (int i = 0; i+col < 9 && right[i] != '0'; i++){
        moves.push_back(Action::Position{i+col, row});
    }

    //down
    for (int i = 0; i+row < 9 && down[i] != '0'; i++){
        moves.push_back(Action::Position{col, i+row});
    }

    for(int i = col-1; i >= 0 && left[i] != '0'; i--) {
        moves.push_back(Action::Position{i, row});
    }

    for(int i = row -1; i>= 0 && up[i] != '0'; i--) {
        moves.push_back(Action::Position{col, i});
    }


    return moves;
}


