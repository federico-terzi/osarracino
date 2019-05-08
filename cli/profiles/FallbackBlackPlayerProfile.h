//
// Created by freddy on 08/05/19.
//

#ifndef OSARRACINO_FALLBACKBLACKPLAYERPROFILE_H
#define OSARRACINO_FALLBACKBLACKPLAYERPROFILE_H

#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ArnoldMoveGenerator.h>
#include <search/RamboSearchEngine.h>
#include "ParametrizedPlayerProfile.h"

class FallbackBlackPlayerProfile : public ParametrizedPlayerProfile<
        RamboSearchEngine,
        BlackEvaluator,
        ArnoldMoveGenerator>{
public:
    std::string get_profile_name() override {
        return "FallbackBlackPlayerProfile";
    }
};

#endif //OSARRACINO_FALLBACKBLACKPLAYERPROFILE_H
