#ifndef INODE_H
#define INODE_H
#include <linux/fs.h>
#include <linux/highmem.h>
#include <linux/pagemap.h>


typedef struct ifs_inode{

} ifs_ionde_t;

struct inode *ifs_new_inode(struct super_block *sb, struct inode *inode,int mode);
int ifs_readpage(struct file *fp, struct page *pg);

#endif
