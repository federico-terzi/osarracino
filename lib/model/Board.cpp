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
    //std::cout << root << std::endl;
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

    for(int i = 0; i < DIM; i++) {
        for (int j = 0; j< DIM; j++) {
            board[i][j] = Pawn::Empty;
        }
    }

    //LEFT BLOCK CITADEL
    board[0][3] = Pawn::EmptyCitadel;
    board[0][4] = Pawn::EmptyCitadel;
    board[0][5] = Pawn::EmptyCitadel;
    board[1][4] = Pawn::EmptyCitadel;

    //RIGHT BLOCK CITADEL
    board[8][3] = Pawn::EmptyCitadel;
    board[8][4] = Pawn::EmptyCitadel;
    board[8][5] = Pawn::EmptyCitadel;
    board[7][4] = Pawn::EmptyCitadel;

    //UP BLOCK CITADEL
    board[3][0] = Pawn::EmptyCitadel;
    board[4][0] = Pawn::EmptyCitadel;
    board[5][0] = Pawn::EmptyCitadel;
    board[4][1] = Pawn::EmptyCitadel;

    //DOWN BLOCK CITADEL
    board[3][8] = Pawn::EmptyCitadel;
    board[4][8] = Pawn::EmptyCitadel;
    board[5][8] = Pawn::EmptyCitadel;
    board[4][7] = Pawn::EmptyCitadel;

    //THRONE
    board[4][4] = Pawn::EmptyThrone;

    //UP-LEFT CORNER CITADEL
    board[1][0] = Pawn::WinPoint;
    board[2][0] = Pawn::WinPoint;
    board[6][0] = Pawn::WinPoint;
    board[7][0] = Pawn::WinPoint;

    //UP-RIGHT CORNER CITADEL
    board[1][8] = Pawn::WinPoint;
    board[2][8] = Pawn::WinPoint;
    board[6][8] = Pawn::WinPoint;
    board[7][8] = Pawn::WinPoint;

    //DOWN-LEFT CORNER CITADEL
    board[0][1] = Pawn::WinPoint;
    board[0][2] = Pawn::WinPoint;
    board[0][6] = Pawn::WinPoint;
    board[0][7] = Pawn::WinPoint;

    //DOWN-RIGHT CORNER CITADEL
    board[8][1] = Pawn::WinPoint;
    board[8][2] = Pawn::WinPoint;
    board[8][6] = Pawn::WinPoint;
    board[8][7] = Pawn::WinPoint;
}


// Input:
// Board object passed by value to create a new state of the board
// Action::Position from, to represents the move of a pawn
// This constructor builds the data structures of the new Board instance based on the move

Board Board::from_board(Board b, const Position &from, const Position &to) {
    if (b.king_pos == from) {
        b.king_pos = to;
    }

    // Save the pawn
    Pawn pawn = b.board[from.col][from.row] & SelectPawn;

    // Clear the previous
    b.board[from.col][from.row] &= ClearPawn;

    b.board[to.col][to.row] |= pawn;

    b.is_white = !b.is_white;

    b.last_move = to;

    // TODO: improve performance
    // Eat the pawn

    Pawn enemy_pawn = Black;
    if (pawn == Black) {
        enemy_pawn = KingOrWhite;
    }

    // Left eat
    if (to.col > 1) {
        if ((b.board[to.col-1][to.row] & enemy_pawn) != 0 &&
            (b.board[to.col-2][to.row] & (pawn | EmptyCitadel | EmptyThrone)) != 0) {
            b.board[to.col-1][to.row] &= ClearPawn;
        }
    }
    // Right eat
    if (to.col < 7) {
        if ((b.board[to.col+1][to.row] & enemy_pawn) != 0 &&
            (b.board[to.col+2][to.row] & (pawn | EmptyCitadel | EmptyThrone)) != 0) {
            b.board[to.col+1][to.row] &= ClearPawn;
        }
    }
    // Up eat
    if (to.row > 1) {
        if ((b.board[to.col][to.row-1] & enemy_pawn) != 0 &&
            (b.board[to.col][to.row-2] & (pawn | EmptyCitadel | EmptyThrone)) != 0) {
            b.board[to.col][to.row-1] &= ClearPawn;
        }
    }
    // Down eat
    if (to.row < 7) {
        if ((b.board[to.col][to.row+1] & enemy_pawn) != 0 &&
            (b.board[to.col][to.row+2] & (pawn | EmptyCitadel | EmptyThrone)) != 0) {
            b.board[to.col][to.row+1] &= ClearPawn;
        }
    }

    // TODO: mangia pedine
    // TODO: tests

    return std::move(b);
}

Board Board::from_json(const std::string &json) {
    Board b;
    b.load_board(json);
    return b;
}
