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
    Route *route = initRoute("/", "templates/home.html", 1);
    addRoute(route, "/about", "templates/about.html", 1);
    addRoute(route, "/static/index.css", "static/index.css", 2);
    addRoute(route, "/static/script.js", "static/index.js", 3);


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
        // not very pretty, needs to be modified
        // and serve routes using a data structure (hmap or btree)
        struct Route *dest = findRoute(route, path);
        strcpy(template, dest->value);



        // if (strcmp(method, "GET") == 0) {
        //     if (strstr(path, "/static/") != NULL) {
        //         if (strcmp(path, "/static/index.css") == 0) {
        //             strcpy(template, "static/index.css");
        //         } else {
        //             strcpy(template, "static/script.js");
        //         }
        //     } else if (strcmp(path, "/") == 0) {
        //         strcpy(template, "templates/home.html");
        //     } else if (strcmp(path, "/about") == 0) {
        //         strcpy(template, "templates/about.html");
        //     } else {
        //         strcpy(template, "templates/404.html");
        //     }
        // }

        // get the page
        char *response_data = get_file(template);

        // create the response header
        char http_header[8192] = "HTTP/1.1 200 OK\r\n\r\n";
        strcat(http_header, response_data);
        strcat(http_header, "\r\n\r\n");
        // char http_header = send_content(content_type, &response_data);

        // send the response, close socket and free allocated data
        send(client_socket, http_header, sizeof(http_header), 0);
        close(client_socket);
        free(response_data);

    }

    return 0;
}
