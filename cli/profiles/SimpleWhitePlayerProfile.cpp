//
// Created by freddy on 26/04/19.
//

#include "SimpleWhitePlayerProfile.h"
#include <search/RamboSearchEngine.h>


std::string SimpleWhitePlayerProfile::calculate_move(const Board &b) {
    engine.table.clear();
    return engine.make_decision(b, eval, move_generator);
}
