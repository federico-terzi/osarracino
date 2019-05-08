//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H
#define OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H

#include <evaluator/BlackEvaluator.h>
#include <movegenerator/HeisenbergMoveGenerator.h>
#include <search/FSMSearchEngine.h>
#include "ParametrizedPlayerProfile.h"

class SimpleBlackPlayerProfile : public ParametrizedPlayerProfile<
        FSMSearchEngine,
        BlackEvaluator,
        HeisenbergMoveGenerator>{
public:
    std::string get_profile_name() override {
        return "SimpleBlackPlayerProfile";
    }
};

#endif //OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H
