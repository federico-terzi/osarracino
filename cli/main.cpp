#include <iostream>
#include <unistd.h>
#include "model/Board.h"
#include "network/Connector.h"
#include <map>
#include <locale>
#include <search/Minimax.h>

const int PLAYER_COLOR = 1;

std::map<std::string, int> ports {{"white", 5800},{"black",5801}};

int main(int argc, char **argv) {
    std::string player_color;
    if (argc < 2) {
        std::cout << "Missing Player Color {Black|White}" << std::endl;
        std::cout << "Starting as white" << std::endl;
        player_color= "white";
    } else {
        player_color = std::string(argv[PLAYER_COLOR]);
    }

    std::cout << "Hello, World!" << std::endl;
    Board b;
    std::cout << b;

    Connector connector{ports[player_color]};
    connector.send_name("Pippo");

    b.load_board(connector.receive_string());
    std::cout << b << std::endl;

    while (true) {
        std::string move {Minimax::best_move(b)};
        std::cout << move << std::endl;
        connector.send_string(move);

        b.load_board(connector.receive_string());
        std::cout << b << std::endl;

        b.load_board(connector.receive_string());
        std::cout << b << std::endl;
    }

}