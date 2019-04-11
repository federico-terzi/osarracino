//
// Created by max on 10/04/19.
//

#include "Board.h"
#include <jsoncpp/json/json.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>


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
    for(auto &row : root["board"]) {
        int x = 0;
        //TODO: Implement citadel assignment also in empty
        for(auto &column: row) {
            if(column == "EMPTY") {
                board[x][y] = 0;
                moves[x+OFFSET].push_back(y);
                moves[y].push_back(x);

            } else if (column == "WHITE") {
                board[x][y] = Pawn::White;
                if(is_white){
                    to_be_moved.push_back(Action::Position{x, y});
                }
            } else if (column == "BLACK") {
                board[x][y] = Pawn::Black;
                if(!is_white){
                    to_be_moved.push_back(Action::Position{x, y});
                }
            } else if (column == "KING") {
                board[x][y] = Pawn::King;
                to_be_moved.push_back(Action::Position{x, y});
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
std::vector<Action::Position> Board::get_vertical_moves(const Action::Position &pos) {
    std::vector<Action::Position> outUp;
    std::vector<Action::Position> outDown;
    int stuckVert = -1;
    int prevValue = -1;
    for (auto row : moves[pos.column + OFFSET]) {
        if (prevValue == -1) {
            prevValue = row;
            outUp.push_back(Action::Position{pos.column, static_cast<int>(row)});
        } else if (stuckVert >= 0 || (row - prevValue > 1 && row != pos.row)) {
            //We have a wall
            if(stuckVert >= 0 && row - prevValue >=1){
                return outDown;
            }
            if (stuckVert == -1) {
                stuckVert = row - 1;
            }
            outDown.push_back(Action::Position{pos.column, static_cast<int>(row)});
            prevValue = row;
        } else {
            outUp.push_back(Action::Position{pos.column, static_cast<int>(row)});
            prevValue = row;
        }

    }
    if(stuckVert == -1){
        return outUp;
    } else if(stuckVert < pos.row){
        return outDown;
    }
}


std::vector<Action::Position> Board::get_legal_moves(const Action::Position &pos) {

}

Board::Board() {}

Board::Board(std::unordered_map<int, std::vector<int>> moves, Action::Position &from, Action::Position &to) {

 /*TODO: Appena funziona la generazione delle mosse l'idea di base è dato che si è spostata una pedina si libera un posto e se ne occupa un altro, quindi cambio la mappa e bona
  *
  */
}


bool Action::operator==(const Action::Position &lhs, const Action::Position &rhs) {
    return lhs.row == rhs.row && lhs.column == rhs.column;
}
