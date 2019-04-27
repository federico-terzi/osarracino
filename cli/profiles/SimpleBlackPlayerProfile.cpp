//
// Created by freddy on 26/04/19.
//

#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ArnoldMoveGenerator.h>
#include "SimpleBlackPlayerProfile.h"

std::string SimpleBlackPlayerProfile::calculate_move(const Board &b) {
    return engine.make_decision(b, eval, move_generator);
}
