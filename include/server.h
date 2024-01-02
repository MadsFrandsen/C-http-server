#ifndef SERVER_H
#define SERVER_H

#define BUFFSIZE 8192

typedef struct {
    int socket;
    int port;
} Server;

void init_server(Server *server, int port);

// void handle_connection(int client_socket);

// void *thread_function(void *arg);

#endif
