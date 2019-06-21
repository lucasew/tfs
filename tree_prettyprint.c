#include "./defs.h"
#include <stdio.h>

void print_members(struct tfs_members_t *mems, int level);
void print_node(struct tfs_node_t *node, int level);

void print_members(struct tfs_members_t *mems, int level) {
    if (mems == NULL)
        return;
    for (int i = 0; i < level; i++)
        printf(" ");
    printf("%s\n", mems->name);
    print_node(mems->child, level);
    print_members(mems->next, level);
}

void print_node(struct tfs_node_t *node, int level) {
    if (node == NULL)
        return;
    if (node->children == NULL)
        return;
    print_members(*node->children, level + 1);
}
