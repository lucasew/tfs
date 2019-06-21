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
    print_node(root, 0);
}
