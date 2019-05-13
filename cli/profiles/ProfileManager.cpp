//
// Created by freddy on 08/05/19.
//

#include "ProfileManager.h"
#include "SimpleWhitePlayerProfile.h"
#include "SimpleBlackPlayerProfile.h"
#include "FallbackWhitePlayerProfile.h"
#include "FallbackBlackPlayerProfile.h"
#include "AggressiveWhitePlayerProfile.h"

ProfileManager::ProfileManager(const ConfigSet &config) : config(config) {
    player_map["default"] = [](Player player) -> std::unique_ptr<PlayerProfile>{
        if (player == Player::WHITE) {
            return std::make_unique<AggressiveWhitePlayerProfile>();
        }else{
            return std::make_unique<SimpleBlackPlayerProfile>();
        }
    };

    player_map["fallback"] = [](Player player) -> std::unique_ptr<PlayerProfile>{
        if (player == Player::WHITE) {
            return std::make_unique<FallbackWhitePlayerProfile>();
        }else{
            return std::make_unique<FallbackBlackPlayerProfile>();
        }
    };

    player_map["defensive"] = [](Player player) -> std::unique_ptr<PlayerProfile>{
        if (player == Player::WHITE) {
            return std::make_unique<SimpleWhitePlayerProfile>();
        }else{
            return std::make_unique<SimpleBlackPlayerProfile>();
        }
    };
}

std::unique_ptr<PlayerProfile> ProfileManager::get_profile(const std::string &profile) {
    if (player_map.find(profile) == player_map.end()) {
        std::cerr << "Invalid profile: " << profile << std::endl;
        exit(2);
    }

    return player_map[profile](config.player);
}
