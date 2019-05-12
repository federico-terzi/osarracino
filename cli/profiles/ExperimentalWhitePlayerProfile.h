//
// Created by freddy on 12/05/19.
//

#ifndef OSARRACINO_EXPERIMENTALWHITEPLAYERPROFILE_H
#define OSARRACINO_EXPERIMENTALWHITEPLAYERPROFILE_H

#include <evaluator/TorettoWhiteEvaluator.h>
#include <movegenerator/MadaraMoveGenerator.h>
#include <search/FSMSearchEngine.h>
#include "ParametrizedPlayerProfile.h"

class ExperimentalWhitePlayerProfile : public ParametrizedPlayerProfile<
        FSMSearchEngine,
        TorettoWhiteEvaluator,
        MadaraMoveGenerator>{
public:
    std::string get_profile_name() override {
        return "ExperimentalWhitePlayerProfile";
    }
};

#endif //OSARRACINO_EXPERIMENTALWHITEPLAYERPROFILE_H
