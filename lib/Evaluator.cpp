//
// Created by max on 12/04/19.
//

#include "Evaluator.h"

const int PESO_PEDINE = 1;
const int MAX = 50;
const int MIN = -MAX;
const int COVERING = 10;
const int KING_OPTIONS = -2;
const int PAWN_OPT = 2;
const int ISOLATE = -1;
const int WIN_COND = MAX * 2;
const int BEHIND = -10;
const int MAX_OPT = 40;
const int DEVELOPED = 4;
const int DIAG_CONNECTION = 10;


int Evaluator::evaluate_pawn_opt(Board &b,const Action::Position &pos) {
    int pawn_opt = 0;

    for (int i = -1; i < 2; i += 2) {
        if (pos.column + i > 0 && pos.column + i < 9) {
            if ((b.empty[pos.row].test(pos.column + i))) {
                pawn_opt++;
            }
        } else {
            continue;
        }
    }

    for (int j = -1; j < 2; j += 2) {
        if (pos.row + j > 0 && pos.row + j < 9) {
            if ((b.empty[pos.row + j].test(pos.column))) {
                pawn_opt++;
            }
        }
        else {
            continue;
        }
    }

    return pawn_opt;
}


int Evaluator::evaluate_black(Board &b) {
    int king_opt {evaluate_pawn_opt(b, b.kingPos)};

    if(king_opt <= -2) {
        king_opt *= 2;
    }

    int pawns_opt = 0;
    for(const auto &toMove: b.to_be_moved) {
        pawns_opt += evaluate_pawn_opt(b, toMove);
    }

    //TODO: Implement eat-moves.
    return PESO_PEDINE * b.to_be_moved.size() +
           BEHIND*(b.to_be_moved.size()-7 - b.opposite_pawns) +
           PAWN_OPT * pawns_opt - MAX_OPT+
           KING_OPTIONS * king_opt;
}

int Evaluator::evaluate_white(Board &b) {
    return 0;
}

