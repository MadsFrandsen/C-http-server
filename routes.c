#include "routes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Route *initRoute(char *key, char* value, int content_type) {
    Route *route = (Route *)malloc(sizeof(Route));
    
    route->key = key;
    route->value = value;
    route->content_type = content_type;

    route->left = NULL;
    route->right = NULL;

    return route;
}

void inorder(Route *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("Key: %s, Value: %s\n", root->key, root->value);
        inorder(root->right);
    }
}

Route *addRoute(Route *root, char *key, char *value, int content_type) {
    Route *route = initRoute(key, value, content_type);

    if (root == NULL) {
        root = route;
    } else {
        Route *curr = root;
        Route *parent;
        while (curr != NULL) {
            parent = curr;

            if (strcmp(key, curr->key) > 0) {
                curr = curr->right;
            } else if (strcmp(key, curr->key) < 0) {
                curr = curr->left;
            } else {
                printf("A Route For \"%s\" Already Exists.\n", key);
                free(route);
                return NULL;
            }
        }
        if (strcmp(key, parent->key) > 0) {
            parent->right = route;
        } else {
            parent->left = route;
        }
    }
    return route;
}


Route *findRoute(Route *root, char *key) {
    Route *curr = root;

    while (curr != NULL) {
        if (strcmp(key, curr->key) == 0) {
            return curr;
        } else if (strcmp(key, curr->key) > 0) {
            curr = curr->right;
        } else {
            curr = curr->left;
        }
    }

    return NULL;
}
