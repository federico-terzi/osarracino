//
// Created by freddy on 10/04/19.
//

#include "Connector.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


Connector::Connector(int port) : port(port) {
    struct hostent *host;
    struct sockaddr_in servaddr;

    // Inizializzo l'indirizzo del server
    memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    host = gethostbyname("localhost");

    servaddr.sin_addr.s_addr=((struct in_addr *)(host->h_addr))->s_addr;
    servaddr.sin_port = htons(port);

    // Crea la socket
    sd=socket(AF_INET, SOCK_STREAM, 0);
    if(sd<0) {perror("apertura socket"); exit(1);}
    printf("Client: creata la socket sd=%d\n", sd);

    // Effettuo la connect
    if(connect(sd,(struct sockaddr *) &servaddr, sizeof(struct sockaddr))<0)
    {perror("connect"); exit(1);}
    printf("Client: connect ok\n");
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

