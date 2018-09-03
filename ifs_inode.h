#ifndef INODE_H
#define INODE_H
#include <linux/fs.h>


typedef struct ifs_inode{

} ifs_ionde_t;

struct inode *ifs_new_inode(struct super_block *sb);

#endif
