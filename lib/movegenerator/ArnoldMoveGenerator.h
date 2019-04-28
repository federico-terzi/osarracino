//
// Created by freddy on 17/04/19.
//

#ifndef OSARRACINO_ARNOLDMOVEGENERATOR_H
#define OSARRACINO_ARNOLDMOVEGENERATOR_H

#include "MoveGenerator.h"

class ArnoldMoveGenerator  : public MoveGenerator<ArnoldMoveGenerator>{
public:
    std::vector<std::pair<Position,Position>> generate(const Board &b) const;
};


#endif //OSARRACINO_ARNOLDMOVEGENERATOR_H
