#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <stdio.h>
#include "routes.h"

#define BUFFSIZE 8192
#define THREAD_POOL_SIZE 5

extern pthread_t thread_pool[THREAD_POOL_SIZE];
extern pthread_mutex_t mutex;
extern pthread_cond_t condition_var;

typedef struct {
    int socket;
    int port;
} Server;

extern Route *root;

void init_server(Server *server, int port);

void send_file(int client_socket, FILE *fp);

void handle_connection(int client_socket);

void *thread_function(void *arg);

#endif
