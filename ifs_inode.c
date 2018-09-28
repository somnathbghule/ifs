#include <ifs_inode.h>
#include <ifs.h>

//int (*writepage)(struct page *page, struct writeback_control *wbc);

int ifs_writepage(struct page *page, struct writeback_control *wbc){
	DPRINTK("%s called\n",__func__);
	return 0;
}

int ifs_readpage(struct file *fp, struct page *page){
	DPRINTK("%s called\n",__func__);
        return 0;
}

static const struct address_space_operations ifs_aops = {
	.readpage=ifs_readpage,
	.writepage=ifs_writepage,
};


//int (*setattr) (struct dentry *, struct iattr *);
int ifs_setattr(struct dentry *dentry, struct iattr *iattr){
        struct inode *inode = d_inode(dentry);
        int error;

	DPRINTK("%s\n",__func__);	
        error = setattr_prepare(dentry, iattr);
        if (error)          
                return error;
  
        if (iattr->ia_valid & ATTR_SIZE)
                truncate_setsize(inode, iattr->ia_size);
        setattr_copy(inode, iattr);
        mark_inode_dirty(inode);
	return 0;
}

//int (*mknod) (struct inode *,struct dentry *,umode_t,dev_t)
int ifs_mknod(struct inode *dir,struct dentry *dentry,umode_t mode,dev_t dev){
	DPRINTK("%s\n",__func__);	
	return 0;
}

//int (*create) (struct inode *,struct dentry *, umode_t, bool);

int ifs_create(struct inode *dir,struct dentry *dentry, umode_t mode, bool unknown){
	struct inode *inode=ifs_new_inode(dir->i_sb, dir,mode | S_IFREG);
	DPRINTK("%s called\n",__func__);	
	//struct inode *ifs_new_inode(struct super_block *sb, struct inode *dir,int mode);
	DPRINTK("d_name: %s\n",dentry->d_name.name);
	if(inode){
		d_instantiate(dentry, inode);
		dget(dentry);
		dir->i_mtime = dir->i_ctime = current_time(dir);
	}
	return 0;
}

//int (*getattr) (const struct path *, struct kstat *, u32, unsigned int);
int ifs_getattr(const struct path *path, struct kstat *kstat, u32 offset, unsigned int flags){
	struct inode *inode = d_inode(path->dentry);
	DPRINTK("%s called\n",__func__);	
	generic_fillattr(inode, kstat);
	kstat->blocks = inode->i_mapping->nrpages << (PAGE_SHIFT - 9);
	return 0;

}
struct dentry * ifs_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags){
	DPRINTK("%s called\n",__func__);	
	d_add(dentry, NULL);
	return NULL;
}
//int (*unlink) (struct inode *,struct dentry *);

int ifs_unlink (struct inode *dir,struct dentry *dentry){
	DPRINTK("%s called\n",__func__);	
	dput(dentry);
	return 0;
}

struct inode_operations ifs_inode_operations={
	.create=ifs_create,
	.unlink=ifs_unlink,
	.setattr=ifs_setattr,
	.getattr=ifs_getattr,
	.lookup=ifs_lookup,
	.mknod=ifs_mknod,
};

//int (*iterate) (struct file *, struct dir_context *);
int ifs_iterate (struct file *filp, struct dir_context *ctx){
	DPRINTK("%s called\n",__func__);
	return 0;
}

struct file_operations ifs_inode_dir_operations={
	.read			= generic_read_dir,
	.iterate		= dcache_readdir,
	.open           	= dcache_dir_open,
        .release        	= dcache_dir_close,

};

//ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
ssize_t ifs_write(struct file *file, const char __user *buf, size_t size, loff_t *offset){
	DPRINTK("%s called\n",__func__);
	return 0;
}

struct file_operations ifs_file_operations={
	.open  = generic_file_open,
	.write = ifs_write,
	 //.read_iter              = generic_file_read_iter,
        //.write_iter             = generic_file_write_iter,
        //.splice_read            = generic_file_splice_read,
        .llseek                 = generic_file_llseek,

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
		ifs_ino->i_mapping->a_ops = &ifs_aops;
		inode_init_owner(ifs_ino, dir, mode); // RP: why needed? [Init uid,gid,mode for new inode according to posix standards ]
		switch(S_IFMT & mode){
			case S_IFDIR:
				ifs_ino->i_op=&ifs_inode_operations;
				ifs_ino->i_fop=&ifs_inode_dir_operations;
				//ifs_ino->i_fop=&simple_dir_operations;
				DPRINTK("inode FMT is directory\n");
				ifs_ino->i_size=PAGE_SIZE;
				inc_nlink(ifs_ino);
				break;		
			case S_IFREG:
				ifs_ino->i_op=&ifs_inode_operations;
				ifs_ino->i_fop=&ifs_file_operations;
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
