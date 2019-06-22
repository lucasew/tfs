#include "./tree.c"
#include "./tree_prettyprint.c"
#include "./defs.h"
#include "./string_stack.h"

struct tfs_app_ctx_t ctx;

void tfs_init() {
    ctx.root = malloc(sizeof(struct tfs_node_t*));
    *ctx.root = NULL;
    ctx.cwd = NULL;
}

void tfs_destroy() {
    tfs_node__destroy(ctx.root);
    free(ctx.root);
}

struct tfs_command {
    char *name;
    int (*function) (int argc, char **argv);
};

struct tfs_node_t **tfs_get_cwd() {
    return ctx.cwd // Se cwd não foi definido usar a raiz msm
        ? ctx.cwd
        : ctx.root;
}

int mkdir(int argc, char **argv) {
    if (argc < 1)
        return 1;
    if (tfs_string_is_file(argv[1])) {
        printf("E: O caminho especificado '%s' é um arquivo!\n", argv[1]);
        return 1;
    }
    mkdir(argc - 1, argv + 1); // Posso passar mais parâmetros numa tacada
    return tfs_node_mkdir(ctx.root, tfs_get_cwd(), argv[1]);
}

int touch(int argc, char **argv) {
    if (argc < 1)
        return 1;
    if (!tfs_string_is_file(argv[1]))
        printf("E: O caminho especificado '%s' não é um arquivo!\n", argv[1]);
    touch(argc - 1, argv + 1);
    return tfs_node_mkdir(ctx.root, tfs_get_cwd(), argv[1]);
}

int ls(int argc, char **argv) {
    struct tfs_node_t **to_ls = tfs_get_cwd();
    if (argc >= 1) {
        to_ls = tfs_node_chdir(to_ls, argv[1]);
    }
    if (*to_ls != NULL) {
        if ((*to_ls)->children != NULL) {
            struct tfs_members_t *members = *(*to_ls)->children;
            while (members != NULL) {
                printf("%s\n", members->name);
                members = members->next;
            }
            return 0;
        }
    }
    printf("E: Caminho não encontrado\n");
    return 1;
}

int tree(int argc, char **argv) {
    struct tfs_node_t **origin = tfs_get_cwd();
    if (argc >= 1) {
        origin = tfs_node_chdir(origin, argv[1]);
    }
    if (!*origin)
        return 1;
    print_node(*origin, 0);
    return 0;
}
struct tfs_command cmds[] = {
    {"mkdir", mkdir},
    {"touch", touch},
    {"ls", ls},
    {"tree", tree},
    {NULL, NULL}
};

struct arg_acc_t {
    int argc;
    char **argv;
};

struct arg_acc_t tfs_acc_init() {
    struct arg_acc_t ret = {
        .argc = 0,
        .argv = malloc(sizeof(char*)*10)
    };
    return ret;
}
void tfs_acc_destroy(struct arg_acc_t self) {
    for (int i = 0; i < self.argc; i++) {
        free(*(self.argv + i));
    }
    free(self.argv);
}

void tfs_acc_push(struct arg_acc_t *self, char *str) {
    self->argv[self->argc] = str;
    self->argc++;
}

struct arg_acc_t handle_repl() {
    struct tfs_strstack_t strstack = tfs_strstack__init(10);
    struct arg_acc_t args = tfs_acc_init();
    char c;
    int level = 0;
    while(1) {
        c = fgetc(stdin);
        if (c == '\n')
            break;
        if (c == EOF)
            break;
        if (c ==  '"') { // Escape para espaço
            level = !level;
        }
        if (c == ' ' && !level) {
            tfs_acc_push(&args, tfs_strstack__unwrap(&strstack));
            strstack = tfs_strstack__init(10);
        } else {
            tfs_strstack__push(&strstack, c);
        }
    }
    tfs_acc_push(&args, tfs_strstack__unwrap(&strstack));
    return args;
}

int main() {
    tfs_init();
    while (!feof(stdin)) {
        printf("%s $ -> ", "caminho");
        struct arg_acc_t args = handle_repl();
        if (!strcmp(args.argv[0], "exit")) {
            tfs_acc_destroy(args);
            break;
        }
        if (!args.argc)
            goto endloop;
        for (int i = 0; cmds[i].function != NULL; i++) {
            if (!strcmp(args.argv[0], cmds[i].name)) {
                if (cmds[i].function(args.argc - 1, args.argv))
                    printf("W: Comando terminado com erro\n");
                goto endloop;
            }
        }
        printf("W: Comando não encontrado\n");
endloop:
        tfs_acc_destroy(args);
    }
    tfs_destroy();
}
