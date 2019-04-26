//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_SIMPLEWHITEPLAYERPROFILE_H
#define OSARRACINO_SIMPLEWHITEPLAYERPROFILE_H


#include <search/RamboSearchEngine.h>
#include "PlayerProfile.h"

class SimpleWhitePlayerProfile : public PlayerProfile {
public:
    std::string calculate_move(const Board &b) override;

private:
    RamboSearchEngine engine;
};


#endif //OSARRACINO_SIMPLEWHITEPLAYERPROFILE_H
