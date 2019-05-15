//
// Created by freddy on 23/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "GameManager.h"

GameManager::GameManager(Connector &connector, PlayerProfile *defensive, PlayerProfile *aggressive, Player player, ConfigSet config)
        : connector(connector), defensive_profile(defensive), aggressive_profile(aggressive), player(player), config(config) {
}

void GameManager::set_aggressive(bool is_aggressive) {
    this->is_aggressive = is_aggressive;
    if (is_aggressive) {
        std::cout << "Switching to AGGRESSIVE profile " << std::endl;
    }else{
        std::cout << "Switching to DEFENSIVE profile " << std::endl;
    }
}


void GameManager::send_move(const Board &b) {
    Timer timer {Timer(config.timeout)};

    if (!config.fork_enabled) {
        std::string move {defensive_profile->calculate_move(b, timer)};
        connector.send_string(move);
        return;
    }

    while(!timer.is_timed_out()) {
        if (fork() == 0) {  // Child
            timer.update_start_time();
            std::string move;
            if (is_aggressive) {
                std::cout << "Starting elaboration with AGGRESSIVE profile." << std::endl;
                move = aggressive_profile->calculate_move(b, timer);
            }else{
                std::cout << "Starting elaboration with DEFENSIVE profile." << std::endl;
                move = defensive_profile->calculate_move(b, timer);
            }
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
            turn_count++;
        }else{
            analyze_profile_change_policy(b, Player::BLACK);
        }

        Board b2;
        b2.load_board(connector.receive_string());
        std::cout << b2 << std::endl;

        if (player == Player::BLACK) {
            send_move(b2);
            turn_count++;
        }else{
            analyze_profile_change_policy(b2, Player::WHITE);
        }
    }
}

void GameManager::analyze_profile_change_policy(const Board &b, Player player) {
    if (player == Player::WHITE) {
        if (turn_count == 2) {
            int defence_score = b.count_black_defensive() + b.white_count;
            // If the black player is defensive, start the aggressive strategy
            if (defence_score > 9) {
                set_aggressive(true);
            }
        }
    }
}
