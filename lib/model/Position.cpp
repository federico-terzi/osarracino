//
// Created by freddy on 15/04/19.
//

#include "Position.h"

bool operator==(const Position &lhs, const Position &rhs) {
    return lhs.row == rhs.row && lhs.col == rhs.col;
}

std::string Position::to_move() const {
    return std::string(std::string(1, ('a'+this->col))+std::to_string(((this->row)+1)));
}
