#include <ifs_inode.h>

static const struct address_space_operations ifs_aops = {
	.readpage=ifs_readpage,
};

int ifs_readpage(struct file *fp, struct page *page){
        clear_highpage(page);
        flush_dcache_page(page);
        SetPageUptodate(page);
        unlock_page(page);
        return 0;
}

struct inode *ifs_new_inode(struct super_block *sb){
	struct inode *ifs_ino=new_inode(sb);
	ifs_ino->i_mapping->a_ops=&ifs_aops;
	if(ifs_ino){
		return ifs_ino;
	}
	return NULL;
}
