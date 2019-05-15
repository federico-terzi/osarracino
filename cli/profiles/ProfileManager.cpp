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
    player_map["default"] = [](Player player) -> ProfilePair {
        if (player == Player::WHITE) {
            return ProfilePair{std::make_unique<SimpleWhitePlayerProfile>(), std::make_unique<AggressiveWhitePlayerProfile>()};
        }else{
            return ProfilePair{std::make_unique<SimpleBlackPlayerProfile>(), std::make_unique<SimpleBlackPlayerProfile>()};
        }
    };

    player_map["fallback"] = [](Player player) -> ProfilePair {
        if (player == Player::WHITE) {
            return ProfilePair{std::make_unique<FallbackWhitePlayerProfile>(), std::make_unique<FallbackWhitePlayerProfile>()};
        }else{
            return ProfilePair{std::make_unique<FallbackBlackPlayerProfile>(), std::make_unique<FallbackBlackPlayerProfile>()};
        }
    };
    player_map["aggressive"] = [](Player player) -> ProfilePair {
        if (player == Player::WHITE) {
            return ProfilePair{std::make_unique<AggressiveWhitePlayerProfile>(), std::make_unique<AggressiveWhitePlayerProfile>()};
        }else{
            return ProfilePair{std::make_unique<SimpleBlackPlayerProfile>(), std::make_unique<SimpleBlackPlayerProfile>()};
        }
    };
    player_map["defensive"] = [](Player player) -> ProfilePair {
        if (player == Player::WHITE) {
            return ProfilePair{std::make_unique<SimpleWhitePlayerProfile>(), std::make_unique<SimpleWhitePlayerProfile>()};
        }else{
            return ProfilePair{std::make_unique<SimpleBlackPlayerProfile>(), std::make_unique<SimpleBlackPlayerProfile>()};
        }
    };
}

ProfilePair ProfileManager::get_profile(const std::string &profile) {
    if (player_map.find(profile) == player_map.end()) {
        std::cerr << "Invalid profile: " << profile << std::endl;
        exit(2);
    }

    return player_map[profile](config.player);
}
