/*

This file contains a binary search tree datastructure used to structure
the routes. This works fine, however a hashmap would probably be a better
choice.

*/

#include "routes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// initialize a route
Route *initRoute(char *key, char *value, char *content_type) {
    Route *route = (Route *)malloc(sizeof(Route));
    
    route->key = key;
    route->value = value;
    route->content_type = content_type;

    route->left = NULL;
    route->right = NULL;

    return route;
}

// print the tree inorder using recursion
void inorder(Route *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("Key: %s, Value: %s\n", root->key, root->value);
        inorder(root->right);
    }
}

// add a route to the tree
Route *addRoute(Route *root, char *key, char *value, char *content_type) {
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

// search the tree for a given route
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

// free the tree
void freeRoutes(Route *root) {
    if (root != NULL) {
        freeRoutes(root->left);
        freeRoutes(root->right);
        free(root);
    }
}
