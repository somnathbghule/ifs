#include <ifs_inode.h>

struct inode *ifs_new_inode(struct super_block *sb){
	struct inode *ifs_ino=new_inode(sb);

	if(ifs_ino){
		return ifs_ino;
	}
	return NULL;
}
