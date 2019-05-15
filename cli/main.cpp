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
#include "profiles/ProfileManager.h"
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
    ProfileManager profile_manager(config);
    ProfilePair profilePair = profile_manager.get_profile(config.profile);
    profilePair.aggressive->set_config(config);
    profilePair.defensive->set_config(config);

    std::cout << "Using aggressive profile: " << profilePair.aggressive->get_profile_name() << std::endl;
    profilePair.aggressive->print_configuration();
    std::cout << "Using defensive profile: " << profilePair.defensive->get_profile_name() << std::endl;
    profilePair.defensive->print_configuration();

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

    GameManager game_manager(connector, profilePair.defensive, profilePair.aggressive, config.player, config);
    game_manager.game_loop();

}