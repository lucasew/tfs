#ifndef TFS_STRING_STACK
#define TFS_STRING_STACK

#include <stdio.h>

struct tfs_strstack_t {
    char *raw;
    int cursor;
    int size;
};

struct tfs_strstack_t tfs_strstack__init(int size);
int tfs_strstack__resize(struct tfs_strstack_t *this, int howmuch);
int tfs_strstack__push(struct tfs_strstack_t *this, char c);
int tfs_strstack__read_until(struct tfs_strstack_t *this, FILE *f, char until);
char *tfs_strstack__unwrap(struct tfs_strstack_t *this);
char *tfs_strstack__read_until_str(char *in, char symbol);

#endif
