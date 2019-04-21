//
// Created by freddy on 15/04/19.
//

#ifndef OSARRACINO_BLACKEVALUATOR_H
#define OSARRACINO_BLACKEVALUATOR_H

#include "Evaluator.h"
#include <vector>
#include <tuple>
#include <array>
#include <unordered_map>
#include <functional>

enum Direction {Up=1, Down=2, Right=3, Left=4, None=false};
const std::array<int, 4> win_rows_cols = {1, 2, 6, 7};

class BlackEvaluator : public Evaluator<BlackEvaluator>{
public:
    BlackEvaluator();
    std::unordered_map<Position, Direction, pos_hash> near_throne;
    std::unordered_map<Direction, std::function<bool(const Board&)>> near_checks;
    std::function<bool(const Board&, const Position&)> is_moved_near;

    int evaluate(const Board &b) const;
    int black_block_king(const Board &b) const;
    int pawn_differences(const Board &b) const;
    bool simple_win_condition(const Board &b) const;
    bool throne_win_condition(const Board &b) const;
    bool near_throne_win_condition(const Board &b) const;
    bool is_king_in_throne(const Board &b) const;
    Direction is_king_near_throne(const Board &b) const;
    std::vector<Direction> get_direction_of_move_check(const Board &b)const;

    static const int BLANK_WG = 0;
    static const int CYAN_WG = 1;
    static const int PURPLE_WG = 2;
    static const int ALL_COLS_COVERED = 0;
    static const int ALL_ROWS_COVERED = 0;
    static const int KINGS_OPP = 0;
    static const int EZPZ = 5000;

    int color_matrix[9][9] = {
            {BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG},
            {BLANK_WG, BLANK_WG, PURPLE_WG, BLANK_WG, CYAN_WG, BLANK_WG, PURPLE_WG},
            {BLANK_WG, PURPLE_WG, BLANK_WG, CYAN_WG, BLANK_WG, CYAN_WG, BLANK_WG, PURPLE_WG, BLANK_WG },
            {BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG},
            {BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG},
            {BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG, BLANK_WG, CYAN_WG, BLANK_WG, BLANK_WG},
            {BLANK_WG, PURPLE_WG, BLANK_WG, CYAN_WG, BLANK_WG, CYAN_WG, BLANK_WG, PURPLE_WG, BLANK_WG},
            {BLANK_WG, BLANK_WG, PURPLE_WG, BLANK_WG, CYAN_WG, BLANK_WG, PURPLE_WG},
            {BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG, BLANK_WG},

    };
};


#endif //OSARRACINO_BLACKEVALUATOR_H
