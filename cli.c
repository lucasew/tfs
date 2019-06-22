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
    if (argc < 2)
        return 1;
    mkdir(argc - 1, argv + 1); // Posso passar mais parâmetros numa tacada
    if (tfs_string_is_file(argv[1])) {
        printf("E: O caminho especificado '%s' é um arquivo!\n", argv[1]);
        return 1;
    }
    return tfs_node_mkdir(ctx.root, tfs_get_cwd(), argv[1]);
}

int touch(int argc, char **argv) {
    if (argc < 2)
        return 1;
    if (!tfs_string_is_file(argv[1]))
        printf("E: O caminho especificado '%s' não é um arquivo!\n", argv[1]);
    touch(argc - 1, argv + 1);
    return tfs_node_mkdir(ctx.root, tfs_get_cwd(), argv[1]);
}

int ls(int argc, char **argv) {
    struct tfs_node_t **to_ls = tfs_get_cwd();
    if (argc >= 2) {
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
    if (argc >= 2) {
        origin = tfs_node_chdir(origin, argv[1]);
    }
    if (!origin) {
        printf("E: Caminho inválido\n");
        return 1;
    }
    if (!*origin)
        return 1;
    print_node(*origin, 0);
    return 0;
}

int cd(int argc, char **argv) {
    if (argc < 2) {
        printf("E: Argumentos insuficientes\n");
        return 1;
    }
    struct tfs_node_t **cwd = tfs_get_cwd();
    cwd = tfs_node_chdir(cwd, argv[1]);
    if ((*cwd)->node_of)
        if (tfs_string_is_file((*cwd)->node_of->name)) {
            printf("E: Não é possível dar CD em arquivo\n");
            return 1;
        };
    if (cwd == NULL) {
        printf("E: Não foi possível encontrar a pasta\n");
        return 1;
    }
    if (*cwd != NULL) {
        ctx.cwd = cwd;
        return 0;
    }
    printf("E: Caminho inválido");
    return 1;
}

struct arg_acc_t {
    int argc;
    char **argv;
};

void handle_command(struct arg_acc_t args);
int load(int argc, char **argv) {
    int errs = 0;
    while (argc >= 2) {
        FILE *f;
        printf("I: Importando %s...", argv[1]);
        if ((f = fopen(argv[1], "r")) == NULL) {
            errs++;
            printf("\nE: Não foi possível importar %s!\n", argv[1]);
            continue;
        }
        struct tfs_strstack_t stack = tfs_strstack__init(30);
        char c;
        while (!feof(f)) {
            c = fgetc(f);
            if (c == '\n') {
                char *path = tfs_strstack__unwrap(&stack);
                printf("ADD %s\n", path);
                if (!tfs_node_mkdir(ctx.root, ctx.root, path))
                    printf(".");
                else printf("x");
                free(path);
                stack = tfs_strstack__init(30);
                continue;
            }
            tfs_strstack__push(&stack, c);
        }
        fclose(f);
        argc--;
        argv++;
        printf("\n");
    }
    return errs;
}

struct tfs_command cmds[] = {
    {"mkdir", mkdir},
    {"touch", touch},
    {"ls", ls},
    {"dir", ls},
    {"tree", tree},
    {"cd", cd},
    {"load", load},
    {NULL, NULL}
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
            continue;
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

void print_cwd(struct tfs_node_t *cwd) {
    if (cwd == NULL)
        return;
    if (cwd->father != NULL)
        print_cwd(*cwd->father);
    if (cwd->node_of)
        printf("/%s", cwd->node_of->name);
}

void handle_command(struct arg_acc_t args) {
        for (int i = 0; cmds[i].function != NULL; i++) {
            if (!strcmp(args.argv[0], cmds[i].name)) {
                if (cmds[i].function(args.argc, args.argv))
                    printf("W: Comando terminado com erro\n");
                return;
            }
        }
        printf("W: Comando não encontrado\n");
}


int main(int argc, char **argv) {
    tfs_init();
    load(argc, argv);
    while (!feof(stdin)) {
        print_cwd(*tfs_get_cwd());
        printf(" $ -> ");
        struct arg_acc_t args = handle_repl();
        if (!strcmp(args.argv[0], "exit")) {
            tfs_acc_destroy(args);
            break;
        }
        if (!args.argc)
            goto endloop;
        handle_command(args);
endloop:
        tfs_acc_destroy(args);
    }
    tfs_destroy();
}
