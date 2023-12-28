#include <stdlib.h>
#include <stdio.h>

#include <server.h>
#include <get_file.h>


int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Please provide valid port as input");
        exit(EXIT_FAILURE);
    }

    Server server;
    init_server(&server, argv[1]);

    while (1) {
        // handle connections
        int client_socket = accept(server.socket, NULL, NULL);
    }

    return 0;
}