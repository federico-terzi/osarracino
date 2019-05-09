//
// Created by freddy on 08/05/19.
//

#ifndef OSARRACINO_PROFILEMANAGER_H
#define OSARRACINO_PROFILEMANAGER_H

#include <unordered_map>
#include <memory>
#include <functional>
#include "PlayerProfile.h"

class ProfileManager {
public:
    ProfileManager(const ConfigSet &config);

    std::unique_ptr<PlayerProfile> get_profile(const std::string& profile);
private:
    ConfigSet config;
    std::unordered_map<std::string, std::function<std::unique_ptr<PlayerProfile>(Player)>> player_map;
};


#endif //OSARRACINO_PROFILEMANAGER_H
