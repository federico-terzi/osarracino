#include <iostream>
#include <unistd.h>
#include <memory>
#include "model/Board.h"
#include "network/Connector.h"
#include "GameManager.h"
#include "profiles/SimpleWhitePlayerProfile.h"
#include "profiles/SimpleBlackPlayerProfile.h"
#include "util/MemoryManager.h"
#include <map>
#include <locale>
#include <search/RamboSearchEngine.h>


const int PLAYER_COLOR = 1;

int main(int argc, char **argv) {
    Player player = Player::WHITE;
    if (argc >= 2) {
        player = Player::from_string(argv[PLAYER_COLOR]);
    }

    std::cout << " ┌─┐ ┌─┐ ┌─┐ ┬─┐ ┬─┐ ┌─┐ ┌─┐ ┬ ┌┐┌ ┌─┐ " << std::endl;
    std::cout << " │ │ └─┐ ├─┤ ├┬┘ ├┬┘ ├─┤ │   │ │││ │ │ " << std::endl;
    std::cout << " └─┘ └─┘ ┴ ┴ ┴└─ ┴└─ ┴ ┴ └─┘ ┴ ┘└┘ └─┘ " << std::endl;
    std::cout << "    ♫ ♪ ( BEEELLUU FUNTOOREEEE ) ♫ ♪   " << std::endl << std::endl;

    std::cout << "Starting as " << player << std::endl;

    // Increase stack size
    long before_memory = MemoryManager::get_stack_size();
    MemoryManager::increase_stack_size();
    long after_memory = MemoryManager::get_stack_size();
    std::cout << "Increased STACK size from "<< before_memory << " to " << after_memory << std::endl;

    std::cout << "Connecting to server... " << std::flush;

    Connector connector{player.default_port()};
    std::cout << "Connected!" << std::endl;

    std::cout << "Sending player name... " << std::flush;

    connector.send_name("osarracino");
    std::cout << "Done!" << std::endl;

    std::unique_ptr<PlayerProfile> profile = nullptr;

    // TODO: let the user change the profile using a command line argument
    if (player == Player::WHITE) {
        profile = std::make_unique<SimpleWhitePlayerProfile>();
    }else{
        profile = std::make_unique<SimpleBlackPlayerProfile>();
    }

    GameManager game_manager(connector, profile.get(), player);
    game_manager.game_loop();

}