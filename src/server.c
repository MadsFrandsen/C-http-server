#include "../include/server.h"
#include "../include/routes.h"
#include "../include/get_file.h"
#include "../include/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

// function to initialize the server
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

// function to send a file in chunks
void send_file(int client_socket, FILE *fp) {
    char buffer[BUFFSIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0 ) {
        send(client_socket, buffer, bytes_read, 0);
    }
}

// function to handle a connection
void handle_connection(int client_socket) {
    char client_msg[BUFFSIZE];
        ssize_t bytes_received = read(client_socket, client_msg, 8191);

        if (bytes_received <= 0) {
            // handle read error or connection closed
            close(client_socket);
            return;
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
        struct Route *dest = findRoute(root, path);
        if (dest == NULL) {
            strncpy(template, "templates/404.html", 98);
        } else {
            strncpy(template, dest->value, 98);
        }

        // open requested file
        FILE *fp = fopen(template, "r");

        if (fp == NULL) {
            // error with opening file
            const char *error_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><body><h1>404 Not Found</h1></body></html>";
            send(client_socket, error_response, strlen(error_response), 0);
            close(client_socket);
            return;
        }

        // create the response header
        char http_header[BUFFSIZE] = "HTTP/1.1 200 OK\r\n";
        if (dest == NULL) {
            strcat(http_header, "Content-Type: text/html");   
        } else {
            strcat(http_header, dest->content_type);
        }
        strcat(http_header, "\r\n\r\n");

        // send the response and close socket
        send(client_socket, http_header, strlen(http_header), 0);
        send_file(client_socket, fp);
        close(client_socket);
}

// function each thread performs
// __attribute__((unused)) to remove warning when compiling
void *thread_function(void *arg __attribute__((unused))) {
    while (1) {
        int *pclient;

        // thread takes mutex
        pthread_mutex_lock(&mutex);

        // if there is no work in the queue, wait for signal.
        if ((pclient = dequeue()) == NULL) {
            pthread_cond_wait(&condition_var, &mutex);
            pclient = dequeue();
        }
        
        // thread returns mutex
        pthread_mutex_unlock(&mutex);
        if (pclient != NULL) {
            // we have a connection
            handle_connection(*pclient);
        }
    }
}

