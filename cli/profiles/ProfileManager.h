//
// Created by freddy on 08/05/19.
//

#ifndef OSARRACINO_PROFILEMANAGER_H
#define OSARRACINO_PROFILEMANAGER_H

#include <unordered_map>
#include <memory>
#include <functional>
#include "PlayerProfile.h"

struct ProfilePair {
    std::unique_ptr<PlayerProfile> defensive;
    std::unique_ptr<PlayerProfile> aggressive;
};

class ProfileManager {
public:
    ProfileManager(const ConfigSet &config);

    ProfilePair get_profile(const std::string& profile);
private:
    ConfigSet config;
    std::unordered_map<std::string, std::function<ProfilePair(Player)>> player_map;
};


#endif //OSARRACINO_PROFILEMANAGER_H
