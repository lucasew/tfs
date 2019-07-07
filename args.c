/*
 * Stack para armazenamento de argumentos, atualmente tem limite de até 10 argumentos.
 * Retorna argc e argv como é recebido do main quando se chama o programa.
 */

#ifndef _TFS_ARGS
#define _TFS_ARGS
#include <stdlib.h>

struct tfs_args_t {
    int argc;
    char **argv;
};


struct tfs_args_t tfs_args_init() {
    struct tfs_args_t ret = {
        .argc = 0,
        .argv = malloc(sizeof(char*)*10)
    };
    return ret;
}
void tfs_args__destroy(struct tfs_args_t self) {
    for (int i = 0; i < self.argc; i++) {
        free(*(self.argv + i));
    }
    free(self.argv);
}

void tfs_args__push(struct tfs_args_t *self, char *str) {
    self->argv[self->argc] = str;
    self->argc++;
}

#endif
