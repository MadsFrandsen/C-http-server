#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>


#include "../include/server.h"
#include "../include/queue.h"

Route *root;

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

    // initialize thread pool
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], NULL, thread_function, NULL);
    }

    // initialize the server
    Server server;
    init_server(&server, port);

    // register routes
    root = initRoute("/", "templates/home.html", "Content-Type: text/html");
    addRoute(root, "/about", "templates/about.html", "Content-Type: text/html");
    addRoute(root, "/download", "templates/download.html", "Content-Type: text/html");
    addRoute(root, "/static/index.css", "static/index.css", "Content-Type: text/css");
    addRoute(root, "/static/script.js", "static/script.js", "Content-Type: text/javascript");
    addRoute(root, "/download-test", "static/test.txt", "Content-Disposition: attachment; filename=test.txt");


    while (1) {
        // handle connections

        // passing NULL and NULL because not interested in client address
        int client_socket = accept(server.socket, NULL, NULL);
        
        if (client_socket == -1) {
            printf("accept failed.\n");
            exit(EXIT_FAILURE);
        }

        // add work to the queue
        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;
        
        // take mutex since max 1 thread can access queue at a time
        pthread_mutex_lock(&mutex);
        enqueue(pclient);

        // singal that there is job in the queue now
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&mutex);

    }


    freeRoutes(root);
    return 0;
}
