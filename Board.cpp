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
}
