//
// Created by freddy on 23/04/19.
//

#include "GameManager.h"

GameManager::GameManager(Connector &connector, PlayerProfile *currentProfile, Player player)
        : connector(connector), current_profile(currentProfile), player(player) {

    std::cout << "Using profile: " << currentProfile->get_profile_name() << std::endl;
    currentProfile->print_configuration();
}

void GameManager::game_loop() {
    bool end_game = true;
    while (end_game) {
        Board b;
        b.load_board(connector.receive_string());
        std::cout << b << std::endl;

        if (player == Player::WHITE) {
            std::string move {current_profile->calculate_move(b)};
            connector.send_string(move);
        }

        Board b2;
        b2.load_board(connector.receive_string());
        std::cout << b2 << std::endl;

        if (player == Player::BLACK) {
            std::string move {current_profile->calculate_move(b2)};
            connector.send_string(move);
        }
    }
}