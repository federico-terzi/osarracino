//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_SIMPLEWHITEPLAYERPROFILE_H
#define OSARRACINO_SIMPLEWHITEPLAYERPROFILE_H


#include <search/RamboSearchEngine.h>
#include <evaluator/TorettoWhiteEvaluator.h>
#include <movegenerator/ArnoldMoveGenerator.h>
#include <movegenerator/HeisenbergMoveGenerator.h>
#include <search/CarloSearchEngine.h>
#include "PlayerProfile.h"

class SimpleWhitePlayerProfile : public PlayerProfile {
public:
    std::string calculate_move(const Board &b) override;

private:
    //RamboSearchEngine engine;
    CarloSearchEngine engine;
    TorettoWhiteEvaluator eval;
    //ArnoldMoveGenerator move_generator;
    HeisenbergMoveGenerator move_generator;
};


#endif //OSARRACINO_SIMPLEWHITEPLAYERPROFILE_H
