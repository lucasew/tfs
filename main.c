#include "./tree.c"
#include "./tree_prettyprint.c"

int main() {
    struct tfs_node_t *root = NULL;
    tfs_node_mkdir(&root, &root, "/etc/init.d/eoq.conf");
    print_node(root, 0);
    tfs_node_mkdir(&root, &root, "/etc/trab/eoq.conf");
    print_node(root, 0);
    tfs_node_mkdir(&root, &root, "/etc/kappa/eoq.conf");
    print_node(root, 0);
    tfs_node_mkdir(&root, &root, "/etc/ImageMagick-7/mime.xml");
    print_node(root, 0);
    tfs_node_mkdir(&root, &root, "/tmp/eoq");
    print_node(root, 0);
    tfs_node_mkdir(&root, &root, "/media/dados/Lucas");
    tfs_node_mkdir(&root, &root, "/media/dados/Jogos/Counter-Strike Xtreme V6/");
    tfs_node_mkdir(&root, &root, "/media/dados/Jogos/FlatOut 2");
    tfs_node_mkdir(&root, &root, "/media/dados/Jogos/GTA San Andreas");
    print_node(root, 0);
    struct tfs_node_t **ret = tfs_node_chdir(&root, "media/dados/Jogos");
    assert(ret);
    print_node(*ret, 0);
    print_node(*(*ret)->father, 0);
    tfs_node__destroy(&root);
}
