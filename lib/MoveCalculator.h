//
// Created by max on 11/04/19.
//

#ifndef OSARRACINO_MOVECALCULATOR_H
#define OSARRACINO_MOVECALCULATOR_H


#include <bitset>
#include <memory>
#include "Board.h"

// Class used to calculate the possible moves of a pawn
class MoveCalculator {
public:
    /*Attributes*/

    // Matrix of bits used to prevent the white to go in a citadel.
    // It also prevents the black pawn to return in a citadel.
    static std::bitset<9> white_mask[9];

    static std::bitset<3> black_hor_mask[2];
    static std::bitset<2> black_ver_mask[3];

    /*Static methods*/

    // Method used to calculate all moves.
    static std::vector<Action::Position> Get_All_Moves(const Board &b, const Action::Position &pos);

    // Method used to setup the masks used for calculating all moves.
    static void init_masks();
};


#endif //OSARRACINO_MOVECALCULATOR// _H
