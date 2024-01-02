#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "server.h"
#include "get_file.h"
#include "routes.h"


int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Please provide valid port as input");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    if (port == 0) {
        printf("Please provide valid port as input");
        exit(EXIT_FAILURE);
    }

    // initialize the server
    Server server;
    init_server(&server, port);

    // register routes
    Route *route = initRoute("/", "templates/home.html", "Content-Type: text/html");
    addRoute(route, "/about", "templates/about.html", "Content-Type: text/html");
    addRoute(route, "/static/index.css", "static/index.css", "Content-Type: text/css");
    addRoute(route, "/static/script.js", "static/script.js", "Content-Type: text/javascript");


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
        memset(template, '\0', sizeof(template));

        // get the path matching the requested page
        struct Route *dest = findRoute(route, path);
        if (dest == NULL) {
            strncpy(template, "templates/404.html", 98);
        } else {
            strncpy(template, dest->value, 98);
        }

        // get the page
        char *response_data = get_file(template);

        if (response_data == NULL) {
            // handle problem with loading file
            close(client_socket);
            continue;
        }

        // create the response header
        char http_header[8192] = "HTTP/1.1 200 OK\r\n";
        if (dest == NULL) {
            strcat(http_header, "Content-Type: text/html");    
        } else {
            strcat(http_header, dest->content_type);
        }
        strcat(http_header, "\r\n\r\n");
        strcat(http_header, response_data);
        strcat(http_header, "\r\n\r\n");

        // get the size of the buffer
        size_t content_length = strlen(http_header);

        // send the response, close socket and free allocated data
        send(client_socket, http_header, content_length, 0);
        close(client_socket);
        free(response_data);

    }

    freeRoutes(route);

    return 0;
}


