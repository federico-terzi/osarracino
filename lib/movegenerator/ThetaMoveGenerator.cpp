//
// Created by freddy on 15/04/19.
//

#include "ThetaMoveGenerator.h"
#include <algorithm>
#include <util/BoardUtils.h>


std::unordered_map<Position, std::vector<Position>, pos_hash> ThetaMoveGenerator::generate(const Board &b) const {
    std::unordered_map<Position, std::vector<Position>, pos_hash> moves_map;

    auto to_be_moved_empty = BoardUtils::Get_empty_and_to_move(b);

    for (const auto &pawn: to_be_moved_empty.first) {
        std::string vertical, horizontal;
        //Generate the vertical string
        if (b.board[pawn.col][pawn.row] == Pawn::Black || b.board[pawn.col][pawn.row] == Pawn::BlackWinPoint) {
            //Citadels are considered empty but if you are out them you must consider them full!
            for (int i = 0; i < 9; i++) {
                vertical += std::to_string((to_be_moved_empty.second[i] & citadel_mask[i]).test(pawn.col));
            }
        } else {
            for (int i = 0; i < 9; i++) {
                vertical += std::to_string(to_be_moved_empty.second[i].test(pawn.col));
            }
        }
        // Same as before.
        if (b.board[pawn.col][pawn.row] == Pawn::Black || b.board[pawn.col][pawn.row] == Pawn::BlackWinPoint) {
            horizontal = (to_be_moved_empty.second[pawn.row] & citadel_mask[pawn.row]).to_string();
        } else {
            horizontal = to_be_moved_empty.second[pawn.row].to_string();

        }
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

ThetaMoveGenerator::ThetaMoveGenerator() {
    for (auto &row: citadel_mask) {
        row.set();
    }
    citadel_mask[0].reset(3);
    citadel_mask[0].reset(4);
    citadel_mask[0].reset(5);

    citadel_mask[1].reset(4);

    citadel_mask[3].reset(0);
    citadel_mask[3].reset(8);

    citadel_mask[4].reset(0);
    citadel_mask[4].reset(1);
    citadel_mask[4].reset(7);
    citadel_mask[4].reset(8);

    citadel_mask[5].reset(0);
    citadel_mask[5].reset(8);

    citadel_mask[7].reset(4);

    citadel_mask[8].reset(3);
    citadel_mask[8].reset(4);
    citadel_mask[8].reset(5);


}
