//
// Created by freddy on 27/04/19.
//

#ifndef OSARRACINO_HEISENBERGMOVEGENERATOR_H
#define OSARRACINO_HEISENBERGMOVEGENERATOR_H

#include <cstdint>
#include "MoveGenerator.h"

class HeisenbergMoveGenerator  : public MoveGenerator<HeisenbergMoveGenerator>{
public:
    std::vector<Move> generate(const Board &b) const;

    HeisenbergMoveGenerator();

private:
    std::vector<Position> col_to_positions[512][9];
    std::vector<Move> row_to_horizontal_moves[512][9][9];
    std::vector<Move> col_to_vertical_moves[512][9][9];
};


#endif //OSARRACINO_HEISENBERGMOVEGENERATOR_H
