//
// Created by freddy on 12/05/19.
//

#ifndef OSARRACINO_EXPERIMENTALBLACKPLAYERPROFILE_H
#define OSARRACINO_EXPERIMENTALBLACKPLAYERPROFILE_H

#include <evaluator/BlackEvaluator.h>
#include <movegenerator/MadaraMoveGenerator.h>
#include <search/FSMSearchEngine.h>
#include "ParametrizedPlayerProfile.h"

class ExperimentalBlackPlayerProfile : public ParametrizedPlayerProfile<
        FSMSearchEngine,
        BlackEvaluator,
        MadaraMoveGenerator>{
public:
    std::string get_profile_name() override {
        return "ExperimentalBlackPlayerProfile";
    }
};

#endif //OSARRACINO_EXPERIMENTALBLACKPLAYERPROFILE_H
