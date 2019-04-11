//
// Created by freddy on 10/04/19.
//

#include "Connector.h"
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

const char* TARGET  {"localhost"};

Connector::Connector(int port) : port(port) {
    struct hostent *host;
    struct sockaddr_in servaddr;

    // Inizializzo l'indirizzo del server
    memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    host = gethostbyname(TARGET);

    servaddr.sin_addr.s_addr=((struct in_addr *)(host->h_addr))->s_addr;
    servaddr.sin_port = htons(port);

    // Crea la socket
    sd=socket(AF_INET, SOCK_STREAM, 0);
    if(sd<0) {
        std::cerr << "Error in socket creation" << std::endl;
        exit(1);
    }

    // Effettuo la connect
    if(connect(sd,(struct sockaddr *) &servaddr, sizeof(struct sockaddr))<0){
        std::cerr << "Error in connect call" << std::endl;
        exit(1);
    }
}

void Connector::send_name(const std::string &name) {
    std::string payload {'"' + name + '"'};
    send_string(payload);
}

void Connector::send_string(const std::string &payload) {
    // Convert the name length to little endian
    int len {static_cast<int>(htonl(static_cast<uint32_t>(payload.length())))};

    write(sd, &len, sizeof(len));
    write(sd, payload.c_str(), sizeof(char)*payload.length());
}

Connector::~Connector() {
    // Chiusura socket
    close(sd);
}

bool read_all(int socket, char* buffer, unsigned int x)
{
    int bytesRead = 0;
    int result;
    while (bytesRead < x)
    {
        result = read(socket, buffer + bytesRead, x - bytesRead);
        if (result < 0) {
            return false;
        }
        bytesRead += result;
    }
    return true;
}

std::string Connector::receive_string() {
    int strlen;
    // Ricevo la lunghezza
    if (!read_all(sd, reinterpret_cast<char *>(&strlen), sizeof(strlen))) {
        std::cerr << "Error: read_all returned false reading the size" << std::endl;
        exit(1);
    }
    strlen = ntohl(strlen);

    std::vector<char> buffer;
    buffer.reserve(strlen);

    // Ricevo il messaggio
    if (!read_all(sd, buffer.data(), sizeof(char)*strlen)) {
        std::cerr << "Error: read_all returned false reading data" << std::endl;
        exit(1);
    }

    std::string output(buffer.data());
    return std::move(output);
}

