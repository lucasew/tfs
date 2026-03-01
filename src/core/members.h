#ifndef _TFS_MEMBERS_H
#define _TFS_MEMBERS_H

#include "defs.h"

struct tfs_node_t **tfs_members__append(struct tfs_node_t **node, char *name);
void tfs_members__destroy(struct tfs_members_t **root);
int tfs_members_is_file(struct tfs_members_t *obj);

#endif
