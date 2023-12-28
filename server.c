#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>


typedef struct {
    int socket;
    int port;
} Server;


void init_server(Server *server, int port) {
    int server_socket;
    struct sockaddr_in address;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    server->port = port;
    server->socket = server_socket;
    
    printf("Server at: %d\n", server->port);
}

