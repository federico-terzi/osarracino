//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_POSITION_H
#define OSARRACINO_POSITION_H

#include <iostream>
#include <string>

struct Position {
    int8_t col;
    int8_t row;

    bool operator==(const Position &rhs) const;
    bool operator!=(const Position &rhs) const;
    friend std::ostream &operator<<(std::ostream &s, const Position &pos) {
        s << pos.to_move();
        return s;
    };

    std::string to_move() const;
};

struct pos_hash {
    std::size_t operator () (const Position &p) const {
        return p.row*10 + p.col;
    }
};


#endif //OSARRACINO_POSITION_H
