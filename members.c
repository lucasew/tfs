#include "./defs.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct tfs_node_t **tfs_members__append(struct tfs_node_t **node, char *name) {
    if (name == NULL) 
        return NULL;
    if (node == NULL)
        return NULL;
    if (*node == NULL) {
        assert((*node = malloc(sizeof(struct tfs_node_t))) != NULL);
        (*node)->children = NULL;
    }
    if ((*node)->children == NULL) {
        assert(((*node)->children = malloc(sizeof(struct tfs_members_t*))) != NULL);
        *(*node)->children = NULL;
    }
    struct tfs_members_t *member = malloc(sizeof(struct tfs_members_t));
    member->child = NULL;
    member->name = name;
    member->next = NULL;
    if (*(*node)->children == NULL) {
        *(*node)->children = member;
        return &member->child;
    }
    struct tfs_members_t *dummy = *(*node)->children;
    struct tfs_members_t **this = &dummy;
    assert(this != (*node)->children); // Não quero conflito de ponteiro hoje
    while((*this) != NULL) {
        if (!strcmp((*this)->name, name)) { // strcmp retorna 0 quando igual ^^
            free(member->name);
            free(member);
            return &(*this)->child;
        }
        if ((*this)->next == NULL) {
            (*this)->next = member;
            return &member->child;
        }
        *this = (*this)->next;
    }
    return NULL;
}

