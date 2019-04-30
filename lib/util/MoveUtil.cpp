//
// Created by max on 30/04/19.
//

#include "MoveUtil.h"
#include <algorithm>

std::vector<Move> MoveUtil::Order_Move(std::vector<Move> &moves, std::vector<Move> &place_it_first) {

    std::vector<Move> to_return;
    to_return.insert(to_return.end(), place_it_first.begin(), place_it_first.end());
    to_return.insert(to_return.end(), moves.begin(), moves.end());

    return to_return;
}
