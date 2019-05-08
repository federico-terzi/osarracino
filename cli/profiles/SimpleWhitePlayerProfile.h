//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_SIMPLEWHITEPLAYERPROFILE_H
#define OSARRACINO_SIMPLEWHITEPLAYERPROFILE_H

#include <evaluator/TorettoWhiteEvaluator.h>
#include <movegenerator/HeisenbergMoveGenerator.h>
#include <search/FSMSearchEngine.h>
#include "ParametrizedPlayerProfile.h"

class SimpleWhitePlayerProfile : public ParametrizedPlayerProfile<
        FSMSearchEngine,
        TorettoWhiteEvaluator,
        HeisenbergMoveGenerator>{
public:
    std::string get_profile_name() override {
        return "SimpleWhitePlayerProfile";
    }
};

#endif //OSARRACINO_SIMPLEWHITEPLAYERPROFILE_H
