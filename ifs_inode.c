#include <ifs_inode.h>

static const struct address_space_operations ifs_aops = {
	.readpage=ifs_readpage,
};

int ifs_readpage(struct file *fp, struct page *page){
	printk("%s called\n",__func__);
        return 0;
}

struct inode *ifs_new_inode(struct super_block *sb){
	struct inode *ifs_ino=new_inode(sb);
	if(IS_ERR(ifs_ino)){
		printk("dentry err: %ld\n",PTR_ERR(ifs_ino));
		printk("mount dentry is NULL\n");
	}
	if(ifs_ino){
		ifs_ino->i_ino = get_next_ino();
		inode_init_owner(ifs_ino, NULL, S_IFDIR | 0755);
		printk("ifs_ino->i_no: %lu\n", ifs_ino->i_ino);
		return ifs_ino;
	}
	return NULL;
}
