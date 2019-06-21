#include "defs.h"
#include "./string_stack.h"
#include "./members.c"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Checa se o caminho referencia subpastas
int tfs_is_depth_path(char *path) {
    for (int i = 0; i < strlen(path); i++) {
        if (path[i] == '/')
            return 1;
    }
    return 0;
}

struct tfs_node_t** tfs_node_chdir(struct tfs_node_t **cwd, char *path) {
    if (cwd == NULL)
        return NULL;
    if (*cwd == NULL) {
        printf("E: Apontando para uma pasta que não existe\n");
        return NULL;
    }
    if (path == NULL) {
        printf("E: Caminho realmente inválido\n");
        return NULL;
    }
    if (path[0] == '.' && strlen(path) == 1) {
        printf("W: Caminho não alterado\n");
        return cwd;
    }
    if (path[0] == '/')
        path++; // Ignorar o primeiro / se tiver
    if (tfs_is_depth_path(path)) {
        char *folder = tfs_strstack__read_until_str(path, '/');
        if (folder == NULL)
            return cwd;
        cwd = tfs_node_chdir(cwd, folder);
        int foldersize = strlen(folder);
        free(folder);
        /* printf("%s\n", path + foldersize + 1); */
        return tfs_node_chdir(cwd, path + foldersize + 1);
    }
    struct tfs_members_t *dummy = *(*cwd)->children;
    struct tfs_members_t **cursor = &dummy;
    while (*cursor != NULL) {
        if(!strncmp((*cursor)->name, path, strlen(path))) {
            /* printf("Seguindo por %s\n", (*cursor)->name); */
            return &(*cursor)->child;
        };
        *cursor = (*cursor)->next;
    }
    printf("E: Nada encontrado\n");
    return NULL;
};

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
    if (*next != NULL)
        (*next)->father = current;
    return tfs_node_mkdir(root, next, path);
}

// Dando um help para a dependencia circular que temos
void tfs_node__destroy(struct tfs_node_t** root);
void tfs_members__destroy(struct tfs_members_t **root);

void tfs_members__destroy(struct tfs_members_t **root) {
    if (root == NULL)
        return;
    if (*root == NULL)
        return;
    tfs_members__destroy(&(*root)->next);
    tfs_node__destroy(&(*root)->child);
    free((*root)->name);
    free((*root)->child);
    free(*root);
    *root = NULL;
}

void tfs_node__destroy(struct tfs_node_t** root) {
    if (root == NULL)
        return;
    if (*root == NULL)
        return;
    tfs_members__destroy((*root)->children);
    free((*root)->children);
    /* free((*root)->father); */
    free(*root);
    *root = NULL;
}

int tfs_string_is_file(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '.')
            return 1;
    }
    return 0;
}

int tfs_members_is_file(struct tfs_members_t *obj) {
    return tfs_string_is_file(obj->name);
}


#ifdef TESTMODE
#include <assert.h>
int main() {
    struct tfs_node_t** root = tfs_node__init();
    assert(*root == NULL);
    tfs_node_destroy(root);
}
#endif
