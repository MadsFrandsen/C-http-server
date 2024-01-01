#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>



void init_server(Server *server, int port) {
    int server_socket;
    struct sockaddr_in address;

    // create the socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // clear data stored &adress and set values
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // bind the socket to the address
    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // set socket to listen for connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    server->port = port;
    server->socket = server_socket;
    
    printf("Server at: %d\n", server->port);
}


// char send_content(int content_type, char *response_data) {
//     switch (content_type) {
//         case 1:
//             char http_header[8192] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
//             strcat(http_header, response_data);
//             strcat(http_header, "\r\n\r\n");
//             return http_header;
//         case 2:
//             char http_header[8192] = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n";
//             strcat(http_header, response_data);
//             strcat(http_header, "\r\n\r\n");
//             return http_header;
//         case 3:
//             char http_header[8192] = "HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n";
//             strcat(http_header, response_data);
//             strcat(http_header, "\r\n\r\n");
//             return http_header;
//     }
// }
