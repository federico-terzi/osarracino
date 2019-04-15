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

// Input: Json String that represents the board
// This method initializes all the data structures based on the json string.
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
        for(auto &column: row) {
            if(column == "EMPTY") {
                board[x][y] |= 0;
                // Setting the appropriate bit in the matrix of bit that represent the empty cells.
                empty[y].set(x);

            } else if (column == "WHITE") {
                // Is White turn so the board must assign Pawn::White to that cell
                // And if is white turn we must insert that position in the vector of the pawns to be moved
                // If is not white turn we track the number of opposite pawns.
                board[x][y] = Pawn::White;
                if(is_white){
                    pawns[y].set(x);
                    to_be_moved.push_back(Action::Position{x, y});
                }else {
                    opposite_pawns++;
                }
            } else if (column == "BLACK") {
                // Is White turn so the board must assign Pawn::BLACK to that cell
                // And if is BLACK turn we must insert that position in the vector of the pawns to be moved
                // If is not BLACK turn we track the number of opposite pawns.
                board[x][y] |= Pawn::Black;
                if(!is_white){
                    pawns[y].set(x);
                    to_be_moved.push_back(Action::Position{x, y});
                } else {
                    opposite_pawns++;
                }
            } else if (column == "KING") {
                // We Place the king in the board
                board[x][y] = Pawn::King;
                kingPos = Action::Position{x, y};
                if(is_white){
                    pawns[y].set(x);
                    to_be_moved.push_back(Action::Position{x, y});
                }
            } else if (column == "THRONE") {
                // If the KING is not on his Throne, the server comunicates that there is a throne in the board
                board[x][y] = Pawn::EmptyThrone;
            } else {
                std::cerr << "Not recognized column " << column << std::endl;
            }
            x++;
        }
        y++;
    }

}

// This constructor initializes the board citadels, is needed because the server doesn't tell us which cells are citadels
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


// Input:
// Board object passed by value to create a new state of the board
// Action::Position from, to represents the move of a pawn
// This constructor builds the data structures of the new Board instance based on the move

Board Board::from_board(Board b, Action::Position &from, Action::Position &to) {
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

    b.is_white = !b.is_white;
    //TODO: Assign toChange to empty
    //TODO: Change king position if the king has been moved.

    // TODO: mangiate

    return std::move(b);
}

// Redefined operator == for the Action::Position data Structure.
bool Action::operator==(const Action::Position &lhs, const Action::Position &rhs) {
    return lhs.row == rhs.row && lhs.column == rhs.column;
}

std::string Action::Position::to_move() {
    return std::string(std::string(1, Action::toCol[this->column])+std::to_string(((this->row)+1)));
}
