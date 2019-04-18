//
// Created by freddy on 15/04/19.
//

#include "Position.h"

std::string Position::to_move() const {
    return std::string(std::string(1, ('a'+this->col))+std::to_string(((this->row)+1)));
}

bool Position::operator==(const Position &rhs) const {
    return col == rhs.col &&
           row == rhs.row;
}

bool Position::operator!=(const Position &rhs) const {
    return !(rhs == *this);
}
