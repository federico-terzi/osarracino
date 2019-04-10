//
// Created by freddy on 10/04/19.
//

#ifndef OSARRACINO_CONNECTOR_H
#define OSARRACINO_CONNECTOR_H

#include <string>

class Connector {
private:
    int sd;
    int port;
public:
    Connector(int port);

    virtual ~Connector();

    void send_name(const std::string &name);
    void send_string(const std::string &payload);
    std::string receive_string();
};


#endif //OSARRACINO_CONNECTOR_H
