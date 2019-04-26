//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H
#define OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H


#include <search/RamboSearchEngine.h>
#include "PlayerProfile.h"

class SimpleBlackPlayerProfile : public PlayerProfile {
public:
    std::string calculate_move(const Board &b) override;

private:
    RamboSearchEngine engine;
};


#endif //OSARRACINO_SIMPLEBLACKPLAYERPROFILE_H
