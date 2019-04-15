//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_POSITION_H
#define OSARRACINO_POSITION_H

#include <iostream>
#include <string>

struct Position {
    int col;
    int row;

    friend bool operator==(const Position &lhs, const Position &rhs);
    friend std::ostream &operator<<(std::ostream &s, const Position &pos) {
        s << pos.to_move();
    };

    std::string to_move() const;
};

struct pos_hash {
    std::size_t operator () (const Position &p) const {
        return p.row*10 + p.col;
    }
};


#endif //OSARRACINO_POSITION_H
