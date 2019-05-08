//
// Created by freddy on 08/05/19.
//

#ifndef OSARRACINO_FALLBACKWHITEPLAYERPROFILE_H
#define OSARRACINO_FALLBACKWHITEPLAYERPROFILE_H

#include <evaluator/TorettoWhiteEvaluator.h>
#include <movegenerator/ArnoldMoveGenerator.h>
#include <search/RamboSearchEngine.h>
#include "ParametrizedPlayerProfile.h"

class FallbackWhitePlayerProfile : public ParametrizedPlayerProfile<
        RamboSearchEngine,
        TorettoWhiteEvaluator,
        ArnoldMoveGenerator>{
public:
    std::string get_profile_name() override {
        return "FallbackWhitePlayerProfile";
    }
};

#endif //OSARRACINO_FALLBACKWHITEPLAYERPROFILE_H
