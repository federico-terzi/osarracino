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
    explicit GameManager(Connector &connector, PlayerProfile *currentProfile, Player player, ConfigSet config);

    void game_loop();
    void send_move(const Board& b);
private:
    Connector &connector;
    PlayerProfile *current_profile;
    Player player;
    ConfigSet config;
};


#endif //OSARRACINO_GAMEMANAGER_H
