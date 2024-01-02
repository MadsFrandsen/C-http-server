#include "../include/server.h"
// #include "../include/routes.h"
// #include "../include/get_file.h"
// #include "../include/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


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


// void handle_connection(int client_socket) {
//     char client_msg[BUFFSIZE];
//         ssize_t bytes_received = read(client_socket, client_msg, 8191);

//         if (bytes_received <= 0) {
//             // handle read error or connection closed
//             close(client_socket);
//             return;
//         }

//         // null terminate received data
//         client_msg[bytes_received] = '\0';

//         printf("%s\n", client_msg);

//         // get the method and path from the request
//         char method[16];
//         char path[256];
//         sscanf(client_msg, "%15s %255s", method, path);

//         // buffer to store the path
//         char template[100];
//         memset(template, '\0', sizeof(template));

//         // get the path matching the requested page
//         struct Route *dest = findRoute(root, path);
//         if (dest == NULL) {
//             strncpy(template, "templates/404.html", 98);
//         } else {
//             strncpy(template, dest->value, 98);
//         }

//         // get the page
//         char *response_data = get_file(template);

//         if (response_data == NULL) {
//             // handle problem with loading file
//             close(client_socket);
//             return;
//         }

//         // create the response header
//         char http_header[BUFFSIZE] = "HTTP/1.1 200 OK\r\n";
//         if (dest == NULL) {
//             strcat(http_header, "Content-Type: text/html");   
//         } else {
//             strcat(http_header, dest->content_type);
//         }
//         strcat(http_header, "\r\n\r\n");
//         strcat(http_header, response_data);
//         strcat(http_header, "\r\n\r\n");

//         // get the size of the buffer
//         size_t content_length = strlen(http_header);

//         // send the response, close socket and free allocated data
//         send(client_socket, http_header, content_length, 0);
//         close(client_socket);
//         free(response_data);
// }


// void *thread_function(void *arg) {
//     while (1) {
//         int *pclient = dequeue();
//         if (pclient != NULL) {
//             // we have a connection
//             handle_connection(pclient);
//         }
//     }
// }

