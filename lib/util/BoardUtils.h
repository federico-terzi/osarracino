//
// Created by max on 16/04/19.
//

#ifndef OSARRACINO_BOARDUTILS_H
#define OSARRACINO_BOARDUTILS_H

#include <vector>
#include <array>
#include <bitset>
#include <model/Board.h>
#include <model/Position.h>

class BoardUtils {
public:
  static std::pair<std::vector<Position>, std::array<std::bitset<9>,9>> Get_empty_and_to_move(const Board &b);
  static bool Is_Near_King(const Board &b, int col, int row);
};


#endif //OSARRACINO_BOARDUTILS_H
