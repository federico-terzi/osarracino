//
// Created by max on 11/04/19.
//

#include "BitMaskHandler.h"
#include <array>

BitMaskHandler::BitMaskHandler(int col, int row) {
    masks = std::make_unique<std::bitset<9>[]>(9);
    for(int i = 0; i < row; i++){
        masks[i].flip(col);
    }
    masks[row].set();
    this->col = col;
    this->row = row;
}

void BitMaskHandler::xorAndRun(std::bitset<9> set[]) {
    std::string vert = "";
    for(int i = 0; i < 9; i++){
        masks[i] ^= set[i];
        vert+= static_cast<char>(masks[i].test(col));
    }

    std::string right = masks[row].to_string().substr(col+1);
    std::string left = masks[row].to_string().substr(0, col);
    std::string up = vert.substr(0, row);
    std::string down = vert.substr(row+1);
    // Ho generato un quadrato bastando due cicli.
}


