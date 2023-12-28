#ifndef SERVER_H
#define SERVER_H

typedef struct {
    int socket;
    int port;
} Server;

void init_server(Server *server, int port);

#endif