//
// Created by max on 10/04/19.
//

#include "Board.h"
#include <jsoncpp/json/json.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>
#include <fstream>
#include <stdio.h>
#include <string.h>

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

    if (root["turn"] == "BLACKWIN" || root["turn"] == "WHITEWIN") {
        std::cout << "The match has concluded.";
        exit(0);
    }

    for (auto &row : root["board"]) {
        int x = 0;
        for (auto &column: row) {
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

    // Populate bitmask
    memset(white_cols, 0, sizeof(white_cols));
    memset(white_rows, 0, sizeof(white_rows));
    memset(black_cols, 0, sizeof(black_cols));
    memset(black_rows, 0, sizeof(black_rows));
    memset(obstacle_cols, 0, sizeof(obstacle_cols));
    memset(obstacle_rows, 0, sizeof(obstacle_rows));
    white_count = 0;
    black_count = 0;
    free_winpoints = 0;

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            if ((board[x][y] & (White | King)) != 0) {
                white_cols[x] |= 1 << y;
                white_count++;
            }
            if ((board[x][y] & Black) != 0) {
                black_cols[x] |= 1 << y;
                black_count++;
            }
            if ((board[x][y] & (Pawn::White | Pawn::Black | Pawn::King)) != 0) {
                obstacle_cols[x] |= 1 << y;
            }else if ((board[x][y] & Pawn::WinPoint) != 0) {
                free_winpoints++;
            }
        }
    }

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if ((board[x][y] & (White | King)) != 0) {
                white_rows[y] |= 1 << x;
            }
            if ((board[x][y] & Black) != 0) {
                black_rows[y] |= 1 << x;
            }
            if ((board[x][y] & (Pawn::White | Pawn::Black | Pawn::King)) != 0) {
                obstacle_rows[y] |= 1 << x;
            }
        }
    }

    // Add throne and citadels
    for (int i = 0; i<DIM; i++) {
        obstacle_cols[i] |= citadels_mask[i];
        obstacle_rows[i] |= citadels_mask[i];
        obstacle_cols[i] |= throne_mask[i];
        obstacle_rows[i] |= throne_mask[i];
    }

}

// This constructor initializes the board citadels, is needed because the server doesn't tell us which cells are citadels
Board::Board() {
    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            board[x][y] = Pawn::Empty;
        }
    }

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
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

    // Initialize the fields
    king_pos = {0, 0};
    last_move = {0, 0};
    is_quiet = true;
}


// Input:
// Board object passed by value to create a new state of the board
// Action::Position from, to represents the move of a pawn
// This constructor builds the data structures of the new Board instance based on the move

Board Board::from_board(Board b, const Position &from, const Position &to) {
    // Make sure the king is still alive
    if (b.king_pos.col < 0) {
        std::cerr << "ERROR: creating board after dead king!" << std::endl;
        return b;
    }

    // Update board status
    b.is_white = !b.is_white;
    b.last_move = to;
    b.is_quiet = true;

    // Update king position
    if (b.king_pos == from) {
        b.king_pos = to;
        b.is_quiet = false;
    }

    // Determine the turn based on the color of the moved pawn
    bool is_white_moving = false;  // True if the white is moving, false if the black is moving
    if (b.has_white(from.col, from.row)) {
        is_white_moving = true;
    }

    // Move the the pawn
    b.move_pawn(from, to);

    // Eat the pawn

    if (is_white_moving) {  // White moving
        // Left eat
        if (to.col > 1 && b.has_black(to.col - 1, to.row) && b.has_white_or_wall(to.col - 2, to.row)) {
            b.delete_pawn(to.col - 1, to.row);
            b.is_quiet = false;
        }

        // Right eat
        if (to.col < 7 && b.has_black(to.col + 1, to.row) && b.has_white_or_wall(to.col + 2, to.row)) {
            b.delete_pawn(to.col + 1, to.row);
            b.is_quiet = false;
        }

        // Up eat
        if (to.row > 1 && b.has_black(to.col, to.row - 1) && b.has_white_or_wall(to.col, to.row - 2)) {
            b.delete_pawn(to.col, to.row - 1);
            b.is_quiet = false;
        }

        // Down eat
        if (to.row < 7 && b.has_black(to.col, to.row + 1) && b.has_white_or_wall(to.col, to.row + 2)) {
            b.delete_pawn(to.col, to.row + 1);
            b.is_quiet = false;
        }
    }else{  // Black moving
        // This variable is needed to delay the deletion of the king pawn, in order to verify all the
        // conditions for a king kill.
        bool is_king_targeted = false;

        // Left eat
        if (to.col > 1 && b.has_white(to.col - 1, to.row) && b.has_black_or_wall(to.col - 2, to.row)) {
            // If the deleted pawn is a king, delay the deletion to verify all the conditions
            if (b.has_king(to.col - 1, to.row)) {
                is_king_targeted = true;
            }else{  // Normal pawn, delete immediately
                b.delete_pawn(to.col - 1, to.row);
                b.is_quiet = false;
            }
        }

        // Right eat
        if (to.col < 7 && b.has_white(to.col + 1, to.row) && b.has_black_or_wall(to.col + 2, to.row)) {
            // If the deleted pawn is a king, delay the deletion to verify all the conditions
            if (b.has_king(to.col + 1, to.row)) {
                is_king_targeted = true;
            }else{  // Normal pawn, delete immediately
                b.delete_pawn(to.col + 1, to.row);
                b.is_quiet = false;
            }
        }

        // Up eat
        if (to.row > 1 && b.has_white(to.col, to.row - 1) && b.has_black_or_wall(to.col, to.row - 2)) {
            // If the deleted pawn is a king, delay the deletion to verify all the conditions
            if (b.has_king(to.col, to.row - 1)) {
                is_king_targeted = true;
            }else{  // Normal pawn, delete immediately
                b.delete_pawn(to.col, to.row - 1);
                b.is_quiet = false;
            }
        }

        // Down eat
        if (to.row < 7 && b.has_white(to.col, to.row + 1) && b.has_black_or_wall(to.col, to.row + 2)) {
            // If the deleted pawn is a king, delay the deletion to verify all the conditions
            if (b.has_king(to.col, to.row + 1)) {
                is_king_targeted = true;
            }else{  // Normal pawn, delete immediately
                b.delete_pawn(to.col, to.row + 1);
                b.is_quiet = false;
            }
        }

        // If the king was targeted, verify all the conditions
        if (is_king_targeted) {
            bool should_king_be_killed = false;

            if (b.is_king_in_throne()) {  // KING IN THRONE
                should_king_be_killed = b.has_black_surrounded_throne();
            }else if (adiacent_throne[b.king_pos.col][b.king_pos.row]) { // KING ADIACENT THE THRONE
                should_king_be_killed = b.is_king_surrounded_by_black_or_throne();
            }else{ // KING AWAY FROM THRONE, DELETE ALWAYS
                should_king_be_killed = true;
            }

            if (should_king_be_killed) {
                b.delete_pawn(b.king_pos.col, b.king_pos.row);
                b.king_pos.col = KING_LOST;
                b.is_quiet = false;
            }
        }
    }

    return b;
}

Board Board::from_json(const std::string &json) {
    Board b;
    b.load_board(json);
    return b;
}

bool board_equal(const Pawn b1[DIM][DIM], const Pawn b2[DIM][DIM]) {
    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            if (b1[x][y] != b2[x][y]) {
                return false;
            }
        }
    }
    return true;
}

bool Board::operator==(const Board &rhs) const {
    return board_equal(board, rhs.board) &&
           is_white == rhs.is_white &&
           king_pos == rhs.king_pos &&
           last_move == rhs.last_move;
}

bool Board::operator!=(const Board &rhs) const {
    return !(rhs == *this);
}

Board Board::from_path(const std::string &path) {
    std::ifstream t(path);
    if (t.fail()) {
        throw std::invalid_argument("The file does not exist");
    }
    std::stringstream buffer;
    buffer << t.rdbuf();
    t.close();
    return Board::from_json(buffer.str());
}