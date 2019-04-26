//
// Created by freddy on 23/04/19.
//

#ifndef OSARRACINO_GAMEMANAGER_H
#define OSARRACINO_GAMEMANAGER_H

#include <network/Connector.h>
#include <stdexcept>
#include "profiles/PlayerProfile.h"
#include "Player.h"

class GameManager {
public:
    explicit GameManager(Connector &connector, PlayerProfile *currentProfile, Player player);

    void game_loop();
private:
    Connector &connector;
    PlayerProfile *current_profile;
    Player player;
};


#endif //OSARRACINO_GAMEMANAGER_H
