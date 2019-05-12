//
// Created by freddy on 12/05/19.
//

#ifndef OSARRACINO_MADARAMOVEGENERATOR_H
#define OSARRACINO_MADARAMOVEGENERATOR_H

#include <cstdint>
#include "MoveGenerator.h"

class MadaraMoveGenerator  : public MoveGenerator<MadaraMoveGenerator>{
public:
    std::vector<Move> generate(const Board &b) const;

    MadaraMoveGenerator();

    std::string get_name() const {
        return "MadaraMoveGenerator";
    }

private:
    std::vector<Position> col_to_positions[512][9];
    std::vector<Move> row_to_horizontal_moves[512][9][9][9][9][2];
    std::vector<Move> col_to_vertical_moves[512][9][9][9][9][2];
};


#endif //OSARRACINO_MADARAMOVEGENERATOR_H
