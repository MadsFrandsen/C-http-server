#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
    int content_type;
    Route *left;
    Route *right;
} Route;

Route *initRoute(char *key, char* value, int content_type);

void inorder(Route *root);

Route *addRoute(Route *root, char *key, char *value, int content_type);
