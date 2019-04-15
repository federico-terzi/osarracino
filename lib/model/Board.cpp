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
            if (column == "WHITE") {
                board[x][y] |= Pawn::White;
            } else if (column == "BLACK") {
                board[x][y] |= Pawn::Black;
            } else if (column == "KING") {
                board[x][y] |= Pawn::King;
                king_pos = Position{x, y};
            } else if (column == "EMPTY" || column == "THRONE") {
                // Do nothing
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
    for (int x = 0; x<9; x++) {
        for (int y = 0; y<9; y++) {
            board[x][y] = Pawn::Empty;
        }
    }

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

    board[4][4] = Pawn::EmptyThrone;

    // TODO: add win point
}


// Input:
// Board object passed by value to create a new state of the board
// Action::Position from, to represents the move of a pawn
// This constructor builds the data structures of the new Board instance based on the move

Board Board::from_board(Board b, Position &from, Position &to) {
    // TODO
}
