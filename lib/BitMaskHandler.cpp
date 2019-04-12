//
// Created by max on 11/04/19.
//

#include "BitMaskHandler.h"

std::vector<Action::Position> BitMaskHandler::xorAndRun(Board &b, int col, int row) {
    std::string vert = "";

    for(int i = 0; i < 9; i++){
        if(b.is_white || b.board[col][row] != Pawn::FullCitadel){
            vert+= (b.empty[i] & white_mask[i]).to_string()[col];
        } else {
            vert+= b.empty[i].to_string()[col];
        }

    }

    std::string right;
    std::string left;
    std::string down;
    std::string up;

    if (b.is_white || b.board[col][row] != Pawn::FullCitadel){

        right = (b.empty[row] & white_mask[row]).to_string().substr(col+1);
        left = (b.empty[row] & white_mask[row]).to_string().substr(0, col);
        up = vert.substr(0, row);
        down = vert.substr(row+1);

    }else {
        right = b.empty[row].to_string().substr(col+1);
        left = b.empty[row].to_string().substr(0, col);
        up = vert.substr(0, row);
        down = vert.substr(row+1);

    }

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

void BitMaskHandler::init_masks() {

    for (auto &bitmask: white_mask) {
        bitmask.set();
    }

    /*Setting citadel masks for white*/
    white_mask[0].reset(3);
    white_mask[0].reset(4);
    white_mask[0].reset(5);

    white_mask[1].reset(5);

    white_mask[3].reset(0);
    white_mask[3].reset(8);

    white_mask[4].reset(0);
    white_mask[4].reset(1);
    white_mask[4].reset(7);
    white_mask[4].reset(8);

    white_mask[5].reset(0);
    white_mask[5].reset(8);

    white_mask[7].reset(5);

    white_mask[8].reset(3);
    white_mask[8].reset(4);
    white_mask[8].reset(5);

    /*Set black horizontal mask*/
    black_hor_mask[0].set(0);
    black_hor_mask[1].set(2);

    /*Set black vertical mask*/
    black_ver_mask[0].set(1);
    black_ver_mask[2].set(1);
}



