#ifndef _TFS_ARGS_H
#define _TFS_ARGS_H

struct tfs_args_t {
    int argc;
    char **argv;
};

struct tfs_args_t tfs_args_init();
void tfs_args__destroy(struct tfs_args_t self);
void tfs_args__push(struct tfs_args_t *self, char *str);

#endif
