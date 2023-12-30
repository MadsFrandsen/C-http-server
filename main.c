#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "server.h"
#include "get_file.h"


int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Please provide valid port as input");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    Server server;
    init_server(&server, port);

    while (1) {
        // handle connections

        // passing NULL and NULL because not interested in client address
        int client_socket = accept(server.socket, NULL, NULL);


        // read client_socket msg and parse it
        
        char client_msg[8192];
        ssize_t bytes_received = read(client_socket, client_msg, 8191);
        client_msg[bytes_received] = '\0';

        printf("%s\n", client_msg);
    }

    return 0;
}
