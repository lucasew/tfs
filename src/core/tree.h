#ifndef _TFS_TREE_H
#define _TFS_TREE_H

#include "defs.h"

int tfs_is_depth_path(char *path);
struct tfs_node_t** tfs_node_chdir(struct tfs_node_t **cwd, char *path);
int tfs_node_mkdir(struct tfs_node_t** root, struct tfs_node_t** current, char *path);
void tfs_node__destroy(struct tfs_node_t** root);
int tfs_string_is_file(char *str);

#endif
