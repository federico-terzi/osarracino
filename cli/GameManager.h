//
// Created by freddy on 23/04/19.
//

#ifndef OSARRACINO_GAMEMANAGER_H
#define OSARRACINO_GAMEMANAGER_H


#include <network/Connector.h>

class GameManager {
public:
    explicit GameManager(Connector &connector);

    void game_loop();
private:
    Connector &connector;
};


#endif //OSARRACINO_GAMEMANAGER_H
