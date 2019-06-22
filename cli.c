#include "./tree.c"
#include "./tree_prettyprint.c"
#include "./defs.h"
#include "./string_stack.h"
#include "./args.c"
#include "./log.c"

struct tfs_app_ctx_t ctx;

int check_dirs = 1;

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

int tfs_cmd_mkdir(int argc, char **argv) {
    if (argc < 2)
        return 1;
    tfs_cmd_mkdir(argc - 1, argv + 1); // Posso passar mais parâmetros numa tacada
    if (check_dirs && tfs_string_is_file(argv[1])) {
        tfs_log_err("O caminho especificado '%s' é um arquivo!\n", argv[1]);
        return 1;
    }
    return tfs_node_mkdir(ctx.root, tfs_get_cwd(), argv[1]);
}

int tfs_cmd_touch(int argc, char **argv) {
    if (argc < 2)
        return 1;
    if (check_dirs && !tfs_string_is_file(argv[1])) {
        tfs_log_err("O caminho especificado '%s' não é um arquivo!\n", argv[1]);
    }
    tfs_cmd_touch(argc - 1, argv + 1);
    return tfs_node_mkdir(ctx.root, tfs_get_cwd(), argv[1]);
}

int tfs_cmd_ls(int argc, char **argv) {
    struct tfs_node_t **to_ls = tfs_get_cwd();
    if (argc >= 2) {
        to_ls = tfs_node_chdir(to_ls, argv[1]);
    }
    if (!to_ls) {
        tfs_log_err("Não encontrado\n");
        return 1;
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
    tfs_log_err("Caminho não encontrado\n");
    return 1;
}

int tfs_cmd_tree(int argc, char **argv) {
    struct tfs_node_t **origin = tfs_get_cwd();
    if (argc >= 2) {
        origin = tfs_node_chdir(origin, argv[1]);
    }
    if (!origin) {
        tfs_log_err("Caminho inválido\n");
        return 1;
    }
    if (!*origin)
        return 1;
    tfs_node__print(*origin, 0);
    return 0;
}

int tfs_cmd_cd(int argc, char **argv) {
    if (argc < 2) {
        tfs_log_err("Argumentos insuficientes\n");
        return 1;
    }
    struct tfs_node_t **cwd = tfs_get_cwd();
    cwd = tfs_node_chdir(cwd, argv[1]);
    if (!cwd) {
        return 1;
    }
    if ((*cwd)->node_of)
        if (check_dirs && tfs_string_is_file((*cwd)->node_of->name)) {
            tfs_log_err("Não é possível dar CD em arquivo\n");
            return 1;
        };
    if (cwd == NULL) {
        tfs_log_err("Não foi possível encontrar a pasta\n");
        return 1;
    }
    if (*cwd != NULL) {
        ctx.cwd = cwd;
        return 0;
    }
    tfs_log_err("Caminho inválido");
    return 1;
}

int tfs_cmd_load(int argc, char **argv) {
    int errs = 0;
    while (argc >= 2) {
        FILE *f;
        tfs_log_info("Importando %s...", argv[1]);
        if ((f = fopen(argv[1], "r")) == NULL) {
            errs++;
            printf("\n");
            tfs_log_err("Não foi possível importar %s!\n", argv[1]);
            argc--;
            argv++;
            continue;
        }
        struct tfs_strstack_t stack = tfs_strstack__init(30);
        char c;
        while (!feof(f)) {
            c = fgetc(f);
            if (c == '\n') {
                char *path = tfs_strstack__unwrap(&stack);
                /* printf("ADD %s\n", path); */
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
        free(stack.raw);
        printf("\n");
    }
    return errs;
}

int tfs_cmd_enable_dir_check(int argc, char **argv) {
    check_dirs = 1;
    return 0;
}

int tfs_cmd_disable_dir_check(int argc, char **argv) {
    check_dirs = 0;
    return 0;
}

int tfs_cmd_help(int argc, char **argv) {
    tfs_log_info("TFS - Tree File System by Lucas59356 <lucas59356@gmail.com>\n");
    tfs_log_info("COMANDOS\n");
    tfs_log_info("\tmkdir args... - Cria pastas\n");
    tfs_log_info("\ttouch args... - Cria arquivos\n");
    tfs_log_info("\tls arg        - Lista os conteúdos de uma pasta\n");
    tfs_log_info("\tdir arg       - Alias para ls\n");
    tfs_log_info("\ttree arg      - Lista todos os elementos filhos de arg\n");
    tfs_log_info("\tcd arg        - Muda o caminho atual para arg\n");
    tfs_log_info("\tload args...  - Carrega uma lista de caminhos e aplica no caminho atual. Comando padrão na inicialização\n");
    tfs_log_info("\tedc           - Ativa checagem de arquivos e pastas usando o critério do ponto (padrão)\n");
    tfs_log_info("\tddc           - Desativa checagem de arquivos e pastas usando o critério do ponto\n");
    tfs_log_info("\thelp          - Mostra isso xD\n");
    tfs_log_info("\texit          - Sai do programa\n");
    return 0;
}

struct tfs_command cmds[] = {
    {"mkdir", tfs_cmd_mkdir},
    {"touch", tfs_cmd_touch},
    {"ls", tfs_cmd_ls},
    {"dir", tfs_cmd_ls},
    {"tree", tfs_cmd_tree},
    {"cd", tfs_cmd_cd},
    {"load", tfs_cmd_load},
    {"edc", tfs_cmd_enable_dir_check},
    {"ddc", tfs_cmd_disable_dir_check},
    {"help", tfs_cmd_help},
    {NULL, NULL}
};
struct tfs_args_t tfs_repl_handle() {
    struct tfs_strstack_t strstack = tfs_strstack__init(10);
    struct tfs_args_t args = tfs_args_init();
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
            tfs_args__push(&args, tfs_strstack__unwrap(&strstack));
            strstack = tfs_strstack__init(10);
        } else {
            tfs_strstack__push(&strstack, c);
        }
    }
    tfs_args__push(&args, tfs_strstack__unwrap(&strstack));
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

void tfs_command_handle(struct tfs_args_t args) {
        for (int i = 0; cmds[i].function != NULL; i++) {
            if (!strcmp(args.argv[0], cmds[i].name)) {
                if (cmds[i].function(args.argc, args.argv))
                    /* tfs_log_warn("Comando terminado com erro\n"); */
                return;
            }
        }
        tfs_log_err("Comando não encontrado\n");
}

int running = 1;

// https://github.com/lucasew/allegro_blasteroids/blob/master/src/util_signal.c
#include <signal.h>
int catch_signal(int sig, void (*handler)(int)) {
#ifndef WIN32
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, 0); // 0 == NULL
#else
    return signal(SIGINT, handler) ? 0: 1;
#endif
}

void tfs_stop(int sig) {
    tfs_log_info("Saindo...");
    running = 0;
}

int main(int argc, char **argv) {
    catch_signal(SIGTERM, tfs_stop); // Parar o programa quando receber um sinal do sistema e não ter vazamento de memoria
    catch_signal(SIGINT, tfs_stop);
    tfs_init();
    tfs_cmd_load(argc, argv);
    printf("TFS - Tree File System by Lucas59356 <lucas59356@gmail.com>.\n");
    tfs_log_info("Digite help para ajuda.\n");
    while (running && !feof(stdin)) {
        print_cwd(*tfs_get_cwd());
        printf(" $ -> ");
        struct tfs_args_t args = tfs_repl_handle();
        if (!strcmp(args.argv[0], "exit")) {
            tfs_args__destroy(args);
            break;
        }
        if (!args.argc)
            goto endloop;
        tfs_command_handle(args);
endloop:
        tfs_args__destroy(args);
    }
    tfs_destroy();
}
