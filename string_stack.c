#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "./string_stack.h"

struct tfs_strstack_t tfs_strstack__init(int size) {
    struct tfs_strstack_t ret = {
        .raw = malloc(sizeof(char)*size),
        .cursor = 0,
        .size = size
    };
    assert(ret.raw);
    return ret;
}

int tfs_strstack__resize(struct tfs_strstack_t *this, int howmuch) {
    this->raw = realloc(this->raw, sizeof(char)*(this->size + howmuch));
    this->size += howmuch;
    return this->raw == NULL;
}

int tfs_strstack__push(struct tfs_strstack_t *this, char c) {
    if (this->cursor == this->size - 1) {
        if (tfs_strstack__resize(this, 10)) {
            return 1; // Resize falhou
        }
    }
    this->raw[this->cursor] = c;
    this->cursor++;
    return 0;
}

int tfs_strstack__read_until(struct tfs_strstack_t *this, FILE *f, char until) {
    char c;
    while (feof(f)) {
        c = getc(f);
        if (c == until) {
            ungetc(c, f);
            return 0; // Se terminar certinho no char ele vai pra cá
        }
        tfs_strstack__push(this, c);
    }
    return 1; // Se chegar no final do arquivo ele chega aqui
}

char *tfs_strstack__read_until_str(char *in, char symbol) {
    if (in == NULL || *in == 0)
        return NULL;
    struct tfs_strstack_t stack = tfs_strstack__init(10);
    while (*in != '\0') {
        if (*in == symbol)
            return tfs_strstack__unwrap(&stack);
        tfs_strstack__push(&stack, *in);
        in++;
    }
    return tfs_strstack__unwrap(&stack);
}
char *tfs_strstack__unwrap(struct tfs_strstack_t *this) {
    this->raw[this->cursor] = 0;
    return this->raw;
}

#ifdef TESTMODE
#include <string.h>
int main() {
    struct tfs_strstack_t stack = tfs_strstack__init(10);
    char *valor = "meu nome é lucas, testando 123";
    for (int i = 0; i < strlen(valor); i++)
        tfs_strstack__push(&stack, valor[i]);
    char *res = tfs_strstack__unwrap(&stack);
    assert(!strcmp(valor, res));
}
#endif
