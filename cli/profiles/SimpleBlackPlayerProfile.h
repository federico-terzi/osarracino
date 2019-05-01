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
#include <search/ZaWarudoSearchEngine.h>
#include "PlayerProfile.h"

class SimpleBlackPlayerProfile : public PlayerProfile {
public:
    std::string calculate_move(const Board &b) override;

private:
    //RamboSearchEngine engine;
    //ZaWarudoSearchEngine engine;
    BlackEvaluator eval;
    CollisionSearchEngine engine;
    //ReverseTorettoBlackEvaluator eval;
    //ArnoldMoveGenerator move_generator;
    HeisenbergMoveGenerator move_generator;
};


#endif //OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H
