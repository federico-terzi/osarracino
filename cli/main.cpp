#include <iostream>
#include <unistd.h>
#include "model/Board.h"
#include "network/Connector.h"
#include "GameManager.h"
#include <map>
#include <locale>
#include <search/Minimax.h>
#include <search/RamboSearchEngine.h>

const int PLAYER_COLOR = 1;

std::map<std::string, int> ports {{"white", 5800},{"black",5801}};

int main(int argc, char **argv) {
    std::string player_color;
    if (argc < 2) {
        std::cout << "Missing Player Color {Black|White}" << std::endl;
        player_color= "white";
    } else {
        player_color = std::string(argv[PLAYER_COLOR]);
    }

    std::cout << "Starting as " << player_color << std::endl;

    std::cout << " ┌─┐ ┌─┐ ┌─┐ ┬─┐ ┬─┐ ┌─┐ ┌─┐ ┬ ┌┐┌ ┌─┐ " << std::endl;
    std::cout << " │ │ └─┐ ├─┤ ├┬┘ ├┬┘ ├─┤ │   │ │││ │ │ " << std::endl;
    std::cout << " └─┘ └─┘ ┴ ┴ ┴└─ ┴└─ ┴ ┴ └─┘ ┴ ┘└┘ └─┘ " << std::endl;
    std::cout << "    ♫ ♪ ( BEEELLUU FUNTOOREEEE ) ♫ ♪   " << std::endl << std::endl;

    std::cout << "Connecting to server... " << std::flush;

    Connector connector{ports[player_color]};
    std::cout << "Connected!" << std::endl;

    std::cout << "Sending player name... " << std::flush;

    connector.send_name("osarracino");
    std::cout << "Done!" << std::endl;

    GameManager game_manager(connector);
    game_manager.game_loop();

    RamboSearchEngine searchEngine;

    bool end_game = true;
    while (end_game) {
        Board b;
        b.load_board(connector.receive_string());
        std::cout << b << std::endl;

        // TODO: change
        if (player_color == "white") {
            //std::string move {Minimax::best_move(b)};
            std::string move {searchEngine.make_decision(b)};
            //std::cout << move << std::endl;
            connector.send_string(move);
        }

        Board b2;
        b2.load_board(connector.receive_string());
        std::cout << b2 << std::endl;

        if (player_color == "black") {
            //std::string move {Minimax::best_move(b2)};
            std::string move {searchEngine.make_decision(b2)};
            //std::cout << move << std::endl;
            connector.send_string(move);
        }
    }

}