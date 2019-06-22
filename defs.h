#ifndef TREEFS_DEFS
#define TREEFS_DEFS

struct tfs_app_ctx_t {
    struct tfs_node_t **root; // Raiz do bgl
    struct tfs_node_t **cwd; // Onde estou? Mudo isso com cd
};

// node é como se fosse a pasta, members são os membros do node
struct tfs_node_t {
    struct tfs_members_t **children;
    struct tfs_node_t **father;
    struct tfs_members_t *node_of;
};

struct tfs_members_t {
    struct tfs_members_t *next;
    char *name;
    struct tfs_node_t *child;
};


#endif
