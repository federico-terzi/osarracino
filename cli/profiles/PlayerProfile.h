//
// Created by freddy on 23/04/19.
//

#ifndef OSARRACINO_PLAYERPROFILE_H
#define OSARRACINO_PLAYERPROFILE_H

#include <model/Board.h>
#include <string>
#include <util/Timer.h>
#include "../util/ConfigSet.h"

class PlayerProfile {
public:
    virtual std::string get_profile_name() = 0;
    virtual void print_configuration() = 0;
    virtual std::string calculate_move(const Board &b, const Timer &timer) = 0;

    void set_config(const ConfigSet& config) {
        this->config = config;
    }

    const ConfigSet &get_config() const {
        return config;
    }

private:
    ConfigSet config;
};

#endif //OSARRACINO_PLAYERPROFILE_H
