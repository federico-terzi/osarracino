#include <iostream>
#include <unistd.h>
#include <memory>
#include "model/Board.h"
#include "network/Connector.h"
#include "GameManager.h"
#include "profiles/SimpleWhitePlayerProfile.h"
#include "profiles/SimpleBlackPlayerProfile.h"
#include "util/MemoryManager.h"
#include "util/ConfigSet.h"
#include "util/ArgParser.h"
#include <map>
#include <locale>
#include <search/RamboSearchEngine.h>

int main(int argc, char **argv) {
    std::cout << " ┌─┐  ┌ ┌─┐ ┌─┐ ┬─┐ ┬─┐ ┌─┐ ┌─┐ ┬ ┌┐┌ ┌─┐ ┐" << std::endl;
    std::cout << " │ │  │ └─┐ ├─┤ ├┬┘ ├┬┘ ├─┤ │   │ │││ │ │ │" << std::endl;
    std::cout << " └─┘  └ └─┘ ┴ ┴ ┴└─ ┴└─ ┴ ┴ └─┘ ┴ ┘└┘ └─┘ ┘" << std::endl;
    std::cout << "    ♫ ♪ ( BEEELLUU FUNTOOREEEE ) ♫ ♪   " << std::endl << std::endl;

    // Argument parsing and setup Configurations
    ConfigSet config;
    ArgParser arg_parser(argc, argv);
    arg_parser.populate_config(config);
    config.print();

    // Initialize player profile
    std::unique_ptr<PlayerProfile> profile = nullptr;

    // TODO: let the user change the profile using a command line argument
    if (config.player == Player::WHITE) {
        profile = std::make_unique<SimpleWhitePlayerProfile>();
    }else{
        profile = std::make_unique<SimpleBlackPlayerProfile>();
    }

    profile->set_config(config);

    std::cout << "Using profile: " << profile->get_profile_name() << std::endl;
    profile->print_configuration();

    // Connect to server
    std::cout << "Connecting to server... " << std::flush;
    Connector connector{config.host, config.port};
    std::cout << "Connected!" << std::endl;
    std::cout << "Sending player name... " << std::flush;
    connector.send_name("osarracino");
    std::cout << "Done!" << std::endl;

    // Increase stack size
    long before_memory = MemoryManager::get_stack_size();
    MemoryManager::increase_stack_size();
    long after_memory = MemoryManager::get_stack_size();
    std::cout << "Increased STACK size from "<< before_memory << " to " << after_memory << std::endl;

    GameManager game_manager(connector, profile.get(), config.player);
    game_manager.game_loop();

}