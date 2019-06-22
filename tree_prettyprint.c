#ifndef _TFS_TREE_PRETTYPRINT
#define _TFS_TREE_PRETTYPRINT

#include "./defs.h"
#include <stdio.h>

void tfs_members__print(struct tfs_members_t *mems, int level);
void tfs_node__print(struct tfs_node_t *node, int level);

void tfs_members__print(struct tfs_members_t *mems, int level) {
    if (mems == NULL)
        return;
    for (int i = 0; i < level; i++)
        printf(" ");
    printf("%s\n", mems->name);
    tfs_node__print(mems->child, level);
    tfs_members__print(mems->next, level);
}

void tfs_node__print(struct tfs_node_t *node, int level) {
    if (node == NULL)
        return;
    if (node->children == NULL)
        return;
    tfs_members__print(*node->children, level + 1);
}

#endif
