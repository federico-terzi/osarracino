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
    is_white = root["turn"] == "WHITE";
    for(auto row : root["board"]) {
        int x = 0;
        //TODO: Implement citadel assignment also in empty
        for(auto column: row) {
            if(column == "EMPTY") {
                board[x][y] = 0;
                moves[Action::toCol[x+1]].push_back(y+1);
                moves[y+1].push_back(Action::toCol[x+1]);
            } else if (column == "WHITE") {
                board[x][y] = Pawn::White;
                if(is_white){
                    to_be_moved.push_back(Action::Position{Action::toCol[x], y});
                }
            } else if (column == "BLACK") {
                board[x][y] = Pawn::Black;
                if(!is_white){
                    to_be_moved.push_back(Action::Position{Action::toCol[x], y});
                }
            } else if (column == "KING") {
                board[x][y] = Pawn::King;
                to_be_moved.push_back(Action::Position{Action::toCol[x], y});
            } else if (column == "THRONE") {
                board[x][y] = Pawn::EmptyThrone;
            } else {
                std::cerr << "Not recognized column " << column << std::endl;
            }
            x++;
        }
        y++;
    }

}

std::vector<Action::Position> Board::get_legal_moves(const Action::Position pos) {
    std::vector<Action::Position> out;
    for(auto row : moves[pos.column]){
        out.push_back(Action::Position{static_cast<char>(row), pos.column});
    }
    for(auto col : moves[pos.row]){
        out.push_back(Action::Position{pos.row, col});
    }
    return out;
}
