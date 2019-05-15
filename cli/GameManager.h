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
    explicit GameManager(Connector &connector, PlayerProfile *defensive, PlayerProfile *aggressive,
            Player player, ConfigSet config);

    void game_loop();
    void send_move(const Board& b);
private:
    Connector &connector;
    PlayerProfile *defensive_profile;
    PlayerProfile *aggressive_profile;
    Player player;
    ConfigSet config;
    bool is_aggressive = false;
    int turn_count = 0;

    void set_aggressive(bool is_aggressive);
    void analyze_board_status(const Board& b, Player player);
};


#endif //OSARRACINO_GAMEMANAGER_H
