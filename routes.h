#ifndef ROUTES_H
#define ROUTES_H

struct Route;

typedef struct Route {
    char *key;
    char *value;
    char *content_type;
    struct Route *left;
    struct Route *right;
} Route;

Route *initRoute(char *key, char *value, char *content_type);

void inorder(Route *root);

Route *addRoute(Route *root, char *key, char *value, char *content_type);

Route *findRoute(Route *root, char *key);

void freeRoutes(Route *root);

#endif
