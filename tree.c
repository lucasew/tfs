#include "defs.h"
#include "./string_stack.h"
#include "./members.c"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int tfs_node_mkdir(struct tfs_node_t** root, struct tfs_node_t** current, char *path) {
    if (root == NULL)
        return 0;
    if (path == NULL)
        return 0;
    if (strlen(path) == 0)
        return 0;
    if (*root == NULL)
        assert((*root = malloc(sizeof(struct tfs_node_t))) != NULL);
    if (*path == '/')
        path++;
    char *filename = tfs_strstack__read_until_str(path, '/');
    if (filename == NULL)
        return 0;
    while (*path != '/') {
        if (*path == '\0') {
            tfs_members__append(current, filename);
            return 0;
        }
        path++;
    }
    path++; // Ignorar / também
    struct tfs_node_t **next = tfs_members__append(current, filename);
    return tfs_node_mkdir(root, next, path);
}


#ifdef TESTMODE
#include <assert.h>
int main() {
    struct tfs_node_t** root = tfs_node__init();
    assert(*root == NULL);
    tfs_node_destroy(root);
}
#endif
