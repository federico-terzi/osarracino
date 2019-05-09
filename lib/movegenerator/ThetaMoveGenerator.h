//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_THETAMOVEGENERATOR_H
#define OSARRACINO_THETAMOVEGENERATOR_H

#include "MoveGenerator.h"

class ThetaMoveGenerator : public MoveGenerator<ThetaMoveGenerator>{
public:
    ThetaMoveGenerator();
    std::array<std::bitset<9>, 9> citadel_mask;
    std::vector<Move> generate(const Board &b) const;

    std::string get_name() const {
        return "ThetaMoveGenerator";
    }
};


#endif //OSARRACINO_THETAMOVEGENERATOR_H
