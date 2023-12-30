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

        if (bytes_received <= 0) {
            // handle read error or connection closed
            close(client_socket);
            continue;
        }

        // null terminate received data
        client_msg[bytes_received] = '\0';

        printf("%s\n", client_msg);

        // get the method and path from the request
        char method[16];
        char path[256];
        sscanf(client_msg, "%15s %255s", method, path);

        // buffer to store the path
        char template[100];

        // get the path matching the requested page
        if (strcmp(method, "GET") == 0) {
            if (strstr(path, "/static/") != NULL) {
                strcpy(template, "static/index.css");
            } else if (strcmp(path, "/") == 0) {
                strcpy(template, "templates/home.html");
            } else if (strcmp(path, "/about") == 0) {
                strcpy(template, "templates/about.html");
            } else {
                strcpy(template, "templates/404.html");
            }
        }

        // get the page
        char *response_data = get_file(template);

        // create the response header
        char http_header[8192] = "HTTP/1.1 200 OK\r\n\r\n";
        strcat(http_header, response_data);
        strcat(http_header, "\r\n\r\n");

        // send the response, close socket and free allocated data
        send(client_socket, http_header, sizeof(http_header), 0);
        close(client_socket);
        free(response_data);

    }

    return 0;
}
