#include <ifs_inode.h>
#include <ifs.h>

static const struct address_space_operations ifs_aops = {
	.readpage=ifs_readpage,
};

int ifs_readpage(struct file *fp, struct page *page){
	DPRINTK("%s called\n",__func__);
        return 0;
}

//int (*setattr) (struct dentry *, struct iattr *);
int ifs_setattr(struct dentry *dentry, struct iattr *iattr){
	DPRINTK("%s\n",__func__);	
	return 0;
}

//int (*getattr) (const struct path *, struct kstat *, u32, unsigned int);
//int (*create) (struct inode *,struct dentry *, umode_t, bool);

int ifs_create(struct inode *inode,struct dentry *dentry, umode_t mode, bool unknown){
	DPRINTK("%s called\n",__func__);	
	return 0;
}

int ifs_getattr(const struct path *path, struct kstat *kstat, u32 offset, unsigned int flags){
	struct inode *inode = d_inode(path->dentry);
	DPRINTK("%s called\n",__func__);	
	generic_fillattr(inode, kstat);
	kstat->blocks = inode->i_mapping->nrpages << (PAGE_SHIFT - 9);
	DPRINTK("(S_IFMT & mode)== S_IFDIR :%d", ((S_IFMT & inode->i_mode)== S_IFDIR));
	return 0;

}
struct dentry * ifs_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags){
	DPRINTK("%s called\n",__func__);	
	return 0;
}
struct inode_operations ifs_inode_operations={
	.setattr=ifs_setattr,
	.getattr=ifs_getattr,
	.lookup=ifs_lookup,
};



struct file_operations ifs_inode_dir_operations={
	.read=generic_read_dir,
};

struct inode *ifs_new_inode(struct super_block *sb, struct inode *dir,int mode){
	struct inode *ifs_ino=new_inode(sb);
	DPRINTK("%s called\n",__func__);	
	if(IS_ERR(ifs_ino)){
		DPRINTK("dentry err: %ld\n",PTR_ERR(ifs_ino));
		DPRINTK("mount dentry is NULL\n");
		return NULL;
	}
	// RP: if no inode return from above, no need conditional code below ===> Resolved
	if(ifs_ino){
		ifs_ino->i_ino=get_next_ino();
		ifs_ino->i_atime=ifs_ino->i_mtime = ifs_ino->i_ctime = current_time(ifs_ino);
		inode_init_owner(ifs_ino, dir, mode); // RP: why needed? [Init uid,gid,mode for new inode according to posix standards ]
		switch(S_IFMT & mode){
			case S_IFDIR:
				ifs_ino->i_op=&ifs_inode_operations;
				ifs_ino->i_fop=&ifs_inode_dir_operations;
				DPRINTK("inode FMT is directory\n");
				ifs_ino->i_size=PAGE_SIZE;
				inc_nlink(ifs_ino);
				break;		
			default:
				DPRINTK("Not valid mode\n");
				break;
			
		}
		DPRINTK("ifs_ino->i_no: %lu\n", ifs_ino->i_ino);
		return ifs_ino;
	}
	return NULL;
}
