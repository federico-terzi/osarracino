//
// Created by freddy on 13/05/19.
//

#ifndef OSARRACINO_AGGRESSIVEWHITEPLAYERPROFILE_H
#define OSARRACINO_AGGRESSIVEWHITEPLAYERPROFILE_H

#include <evaluator/TheRockWhiteEvaluator.h>
#include <movegenerator/HeisenbergMoveGenerator.h>
#include <search/FSMSearchEngine.h>
#include "ParametrizedPlayerProfile.h"

class AggressiveWhitePlayerProfile : public ParametrizedPlayerProfile<
        FSMSearchEngine,
        TheRockWhiteEvaluator,
        HeisenbergMoveGenerator>{
public:
    std::string get_profile_name() override {
        return "AggressiveWhitePlayerProfile";
    }
};

#endif //OSARRACINO_AGGRESSIVEWHITEPLAYERPROFILE_H
