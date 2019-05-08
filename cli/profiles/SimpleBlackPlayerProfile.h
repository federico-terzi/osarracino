//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H
#define OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H


#include <search/RamboSearchEngine.h>
#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ArnoldMoveGenerator.h>
#include <movegenerator/HeisenbergMoveGenerator.h>
#include <evaluator/ReverseTorettoBlackEvaluator.h>
#include <search/CollisionSearchEngine.h>
#include <search/CarloSearchEngine.h>
#include "PlayerProfile.h"

class SimpleBlackPlayerProfile : public PlayerProfile {
public:
    std::string calculate_move(const Board &b) override;

private:
    CarloSearchEngine engine;
    //RamboSearchEngine engine;
    //CollisionSearchEngine engine;
    BlackEvaluator eval;
    //ReverseTorettoBlackEvaluator eval;
    //ArnoldMoveGenerator move_generator;
    HeisenbergMoveGenerator move_generator;
};


#endif //OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H
