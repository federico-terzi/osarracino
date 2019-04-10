#include <iostream>
#include <unistd.h>
#include "Board.h"
#include "Connector.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Board b;
    b.load_board("{\"board\":[[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"BLACK\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\"],[\"BLACK\",\"BLACK\",\"WHITE\",\"WHITE\",\"KING\",\"WHITE\",\"WHITE\",\"BLACK\",\"BLACK\"],[\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"BLACK\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\"]],\"turn\":\"WHITE\"}");
    std::cout << b;

    Connector connector{5800};
    connector.send_name("Pippo");

    b.load_board(connector.receive_string());
    std::cout << b << std::endl;

    while (true) {
        std::string from;
        std::string to;

        std::cout << "From To" << std::endl;
        std::cin >> from >> to;

        std::string move {"{\"from\":\""+from+"\",\"to\":\""+to+"\",\"turn\":\"WHITE\"}"};
        std::cout << move << std::endl;
        connector.send_string(move);

        b.load_board(connector.receive_string());
        std::cout << b << std::endl;

        b.load_board(connector.receive_string());
        std::cout << b << std::endl;
    }

}