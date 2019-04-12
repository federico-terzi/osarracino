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
                board[x][y] |= 0;
                empty[y].set(x);

            } else if (column == "WHITE") {
                board[x][y] = Pawn::White;
                if(is_white){
                    pawns[y].set(x);
                    to_be_moved.push_back(Action::Position{x, y});
                }else {
                    opposite_pawns++;
                }
            } else if (column == "BLACK") {
                board[x][y] |= Pawn::Black;
                if(!is_white){
                    pawns[y].set(x);
                    to_be_moved.push_back(Action::Position{x, y});
                } else {
                    opposite_pawns++;
                }
            } else if (column == "KING") {
                board[x][y] = Pawn::King;
                kingPos = Action::Position{x, y};
                if(is_white){
                    pawns[y].set(x);
                    to_be_moved.push_back(Action::Position{x, y});
                }
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

Board::Board(Board b, Action::Position &from, Action::Position &to) {
    b.empty[from.row].reset(from.column);
    b.empty[to.row].set(to.column);

    if(b.is_white) {
        if((b.board[from.column][from.row] & Pawn::King) != 0) {
            b.board[from.column][from.row] -= Pawn::King;
            b.board[to.column][to.row] += Pawn::King;
        }
        b.board[from.column][from.row] -= Pawn::White;
        b.board[to.column][to.row] += Pawn::White;
    } else {
        b.board[from.column][from.row] -= Pawn::Black;
        b.board[to.column][to.row] += Pawn::Black;
    }

    this->is_white = !b.is_white;
    //TODO: Assign toChange to empty
}

Board::Board() {
    board[0][3] = Pawn::EmptyCitadel;
    board[0][4] = Pawn::EmptyCitadel;
    board[0][5] = Pawn::EmptyCitadel;
    board[1][4] = Pawn::EmptyCitadel;

    board[8][3] = Pawn::EmptyCitadel;
    board[8][4] = Pawn::EmptyCitadel;
    board[8][5] = Pawn::EmptyCitadel;
    board[7][5] = Pawn::EmptyCitadel;

    board[3][0] = Pawn::EmptyCitadel;
    board[4][0] = Pawn::EmptyCitadel;
    board[5][0] = Pawn::EmptyCitadel;
    board[4][1] = Pawn::EmptyCitadel;

    board[3][8] = Pawn::EmptyCitadel;
    board[4][8] = Pawn::EmptyCitadel;
    board[5][8] = Pawn::EmptyCitadel;
    board[4][7] = Pawn::EmptyCitadel;

    for(auto &mask: empty) {
        mask.reset();
    }

}

bool Action::operator==(const Action::Position &lhs, const Action::Position &rhs) {
    return lhs.row == rhs.row && lhs.column == rhs.column;
}
