//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_THETAMOVEGENERATOR_H
#define OSARRACINO_THETAMOVEGENERATOR_H

#include "MoveGenerator.h"

class ThetaMoveGenerator : public MoveGenerator<ThetaMoveGenerator>{
public:
    std::unordered_map<Position, std::vector<Position>, pos_hash> generate(const Board &b) const;
};


#endif //OSARRACINO_THETAMOVEGENERATOR_H
