//
// Created by freddy on 26/04/19.
//

#include "SimpleWhitePlayerProfile.h"
#include <search/RamboSearchEngine.h>
#include <evaluator/TorettoWhiteEvaluator.h>
#include <movegenerator/ArnoldMoveGenerator.h>

std::string SimpleWhitePlayerProfile::calculate_move(const Board &b) {
    TorettoWhiteEvaluator eval;
    ArnoldMoveGenerator move_generator;

    return engine.make_decision(b, eval, move_generator);
}
