//
// Created by freddy on 23/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "GameManager.h"

GameManager::GameManager(Connector &connector, PlayerProfile *currentProfile, Player player, ConfigSet config)
        : connector(connector), current_profile(currentProfile), player(player), config(config) {
}

void GameManager::send_move(const Board &b) {
    Timer timer {Timer(config.timeout)};

    while(!timer.is_timed_out()) {
        if (fork() == 0) {  // Child
            timer.update_start_time();
            std::string move {current_profile->calculate_move(b, timer)};
            connector.send_string(move);
            exit(0);
        }else{ // Father
            int stat;
            wait(&stat);

            if (WIFEXITED(stat)) {
                std::cout << "Child process returned correctly" << std::endl;
                return;
            }else if (WIFSIGNALED(stat)) {
                int signal = WTERMSIG(stat);
                std::cerr << "WARNING: Child process crashed with signal "<< signal << std::endl;
                std::cout << "Respawning child process..." << std::endl;
            }
        }
    }
}

void GameManager::game_loop() {
    bool end_game = true;
    while (end_game) {
        Board b;
        b.load_board(connector.receive_string());
        std::cout << b << std::endl;

        if (player == Player::WHITE) {
            send_move(b);
        }

        Board b2;
        b2.load_board(connector.receive_string());
        std::cout << b2 << std::endl;

        if (player == Player::BLACK) {
            send_move(b2);
        }
    }
}
