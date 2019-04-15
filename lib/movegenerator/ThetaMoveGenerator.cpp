//
// Created by freddy on 15/04/19.
//

#include <evaluator/BlackEvaluator.h>
#include "ThetaMoveGenerator.h"
#include <algorithm>


std::unordered_map<Position, std::vector<Position>, pos_hash> ThetaMoveGenerator::generate(const Board &b) const {
    std::unordered_map<Position, std::vector<Position>, pos_hash> moves_map;

    auto to_be_moved_empty = BlackEvaluator::Get_empty_and_to_move(b);

    for (const auto &pawn: to_be_moved_empty.first) {
        std::string vertical, horizontal;
        //Generate the vertical string
        for (int i = 0; i < 9; i++) {
            vertical += std::to_string(to_be_moved_empty.second[i].test(pawn.col));
        }
        horizontal = to_be_moved_empty.second[pawn.row].to_string();
        // Reverse the string to match the real representation
        std::reverse(horizontal.begin(), horizontal.end());

        //Define the direction strings
        std::string left, right, up , down;

        left = horizontal.substr(0, pawn.col);
        right = horizontal.substr(pawn.col);
        up = vertical.substr(0, pawn.row);
        down = vertical.substr(pawn.row);

        std::vector<Position> positions;
        //Left
        for (int i = left.length() - 1; i >= 0 && left[i] != '0'; i--) {
            positions.push_back(Position{i, pawn.row});
        }
        //Right
        for (int i = 1; i < right.length() && right[i] != '0'; i++) {
            positions.push_back(Position{pawn.col+i, pawn.row});
        }
        //Up
        for (int i = up.length() - 1; i >= 0 && up[i] != '0'; i--) {
            positions.push_back(Position{pawn.col, i});
        }
        //Down
        for (int i = 1; i < down.length() && down[i] != '0'; i++) {
            positions.push_back(Position{pawn.col, pawn.row+i});
        }
        moves_map[pawn] = positions;
    }

    return std::move(moves_map);
}