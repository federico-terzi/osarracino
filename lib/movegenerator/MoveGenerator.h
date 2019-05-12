//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_MOVEGENERATOR_H
#define OSARRACINO_MOVEGENERATOR_H

#include "model/Position.h"
#include "model/Board.h"
#include <unordered_map>
#include <vector>

struct Move {
    Position from;
    Position to;
    int penality;

    friend std::ostream &operator<<(std::ostream &s, const Move &move) {
        s << move.from.to_move() << "->" << move.to.to_move();
        return s;
    };

    bool operator==(const Move &rhs) const {
        return from == rhs.from &&
               to == rhs.to;
    }

    bool operator!=(const Move &rhs) const {
        return !(rhs == *this);
    }
};

template <typename T>
class MoveGenerator {
public:
    std::vector<Move> generate(const Board &b) const {
        return static_cast<T const&>(*this).generate(b);
    }

    std::string get_name() const {
        return static_cast<T const&>(*this).get_name();
    }
};

namespace moves {
    /**
     * Return a vector of all the moves that start from the given position.
     * Used to refactor tests that previously used maps and now has to use vectors.
     * @param moves
     * @param from_position
     * @return
     */
    static std::vector<Position> all_from_position(const std::vector<Move> &moves, const Position &from_position) {
        std::vector<Position> filtered_moves;

        for (const auto& move : moves) {
            if (move.from == from_position) {
                filtered_moves.push_back(move.to);
            }
        }

        return filtered_moves;
    }
}



#endif //OSARRACINO_MOVEGENERATOR_H
