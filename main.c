#include "./tree.c"
#include "./tree_prettyprint.c"

int main() {
    struct tfs_node_t *root = NULL;
    tfs_node_mkdir(&root, &root, "/etc/init.d/eoq.conf");
    tfs_node__print(root, 0);
    tfs_node_mkdir(&root, &root, "/etc/trab/eoq.conf");
    tfs_node__print(root, 0);
    tfs_node_mkdir(&root, &root, "/etc/kappa/eoq.conf");
    tfs_node__print(root, 0);
    tfs_node_mkdir(&root, &root, "/etc/ImageMagick-7/mime.xml");
    tfs_node__print(root, 0);
    tfs_node_mkdir(&root, &root, "/tmp/eoq");
    tfs_node__print(root, 0);
    tfs_node_mkdir(&root, &root, "/media/dados/Lucas");
    tfs_node_mkdir(&root, &root, "/media/dados/Jogos/Counter-Strike Xtreme V6/");
    tfs_node_mkdir(&root, &root, "/media/dados/Jogos/FlatOut 2");
    tfs_node_mkdir(&root, &root, "/media/dados/Jogos/GTA San Andreas");
    tfs_node__print(root, 0);
    struct tfs_node_t **ret = tfs_node_chdir(&root, "media/dados/Jogos");
    assert(ret);
    assert(!tfs_node_chdir(&root, "/root"));
    assert(tfs_node_chdir(&root, "/media") == tfs_node_chdir(&root, "media")); // O / não pode dar diferença, de onde ele parte depende do root
    tfs_node__print(*ret, 0);
    struct tfs_node_t **up = tfs_node_chdir(ret, "../../../etc");
    assert(*up);
    tfs_node__print(*up, 0);
    tfs_node__destroy(&root);
}
