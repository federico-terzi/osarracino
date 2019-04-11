//
// Created by max on 10/04/19.
//

#include "Board.h"
#include <jsoncpp/json/json.h>
#include <memory>
#include <iostream>

void Board::load_board(const std::string &json_board) {
    Json::Value root;
    Json::CharReaderBuilder rbuilder;
    rbuilder["collectComments"] = false;
    std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());
    std::string errors;
    bool parsingSuccessful = reader->parse(json_board.c_str(), json_board.c_str() + json_board.size(), &root, &errors);
    std::cout << root << std::endl;
    int y = 0;
    for(auto row : root["board"]) {
        int x = 0;
        for(auto column: row) {
            if(column == "EMPTY") {
                board[x][y] = 0;
            } else if (column == "WHITE") {
                board[x][y] = Pawn::White;
            } else if (column == "BLACK") {
                board[x][y] = Pawn::Black;
            } else if (column == "KING") {
                board[x][y] = Pawn::King;
            } else {
                std::cerr << "Not recognized column " << column << std::endl;
            }
            x++;
        }
        y++;
    }


}
