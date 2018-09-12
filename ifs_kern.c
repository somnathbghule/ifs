#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <ifs_inode.h>
#include <linux/slab.h>
#include <ifs.h>
#include <linux/statfs.h>

/*

# create and mount a 100 megabyte (100M) temporary memory filesystem
	sudo mount -osize=100m ifs /mnt -t ifs
*/

int ifs_statfs(struct dentry *dentry, struct kstatfs *buf){
	DPRINTK("%s called\n",__func__);
	buf->f_bsize=PAGE_SIZE; // RP: what if other fields are not filled, they should be zero.
	return 0;
}
//int (*show_stats)(struct seq_file *, struct dentry *);

int ifs_show_stats(struct seq_file *seqf, struct dentry *dentry){
	DPRINTK("%s called\n",__func__);
	return 0;
}
struct super_operations ifs_super_operations={
	.statfs=ifs_statfs,
	.show_stats=ifs_show_stats,
	.drop_inode=generic_delete_inode, // RP: alloc_inode or new_inode sops would be there, look at that
};

//int (*fill_super)(struct super_block *, void *, int)

int ifs_fill_super(struct super_block *sb, void *data, int silent){
	int status=0;
	struct inode *root_inode=NULL;
	sb->s_blocksize		= PAGE_SIZE;
	sb->s_blocksize_bits    = PAGE_SHIFT;
	sb->s_maxbytes          = MAX_LFS_FILESIZE; // RP: wrong way to use., it should be your own design decision
	sb->s_op		= &ifs_super_operations;
        sb->s_magic             = IFS_MAGIC;
	sb->s_time_gran         = 1; // should know what does it mean, if you  use code
	
	root_inode=ifs_new_inode(sb, NULL,S_IFDIR|0755);
	if(!root_inode){
		status=-1; // RP: cannot return random error values
		goto out;
	}
	sb->s_root = d_make_root(root_inode);
	if (!sb->s_root){
		status=-ENOMEM; // RP: does d_make_root always fail only with ENOMEM?
		goto out;		
	}
	//DPRINTK("data: %s\n",(char *)data);
	DPRINTK("EOF ifs_fill_super\n");
out:
	return status;
}


//struct dentry *(*mount) (struct file_system_type *, int, const char *, void *);
//struct dentry *mount_nodev(struct file_system_type *fs_type, int flags, void *data, int (*fill_super)(struct super_block *, void *, int))
struct dentry *ifs_mount(struct file_system_type *fstype, int mount_flags, const char *dev_name, void *data){
	struct dentry *dt=NULL;
	DPRINTK("%s called\n", __func__);
	dt=mount_nodev(fstype, mount_flags, data, ifs_fill_super);
	if(IS_ERR(dt)){
		DPRINTK("dentry err: %ld\n",PTR_ERR(dt));
		DPRINTK("mount dentry is NULL\n");
	}
	DPRINTK("EOF ifs_mount\n");
	return dt; // RP: is it the right error to return
}
void ifs_kill_sb(struct super_block *sb){
	DPRINTK("%s called.\n",__func__);
	kill_litter_super(sb); // RP: whats does this API does and why not use other generic kill_super APIs?
}
struct file_system_type ifs_type={
	.name="ifs",
	.fs_flags=FS_USERNS_MOUNT, // RP: whats the use of this flag?
	.owner=THIS_MODULE,
	.mount=ifs_mount,
	.kill_sb=ifs_kill_sb,
};

//extern int register_filesystem(struct file_system_type *);
static int ifs_init(void){
	int ret=0;
	DPRINTK("%s called\n",__func__);
	ret=register_filesystem(&ifs_type);
	if(ret){
		DPRINTK("ifs registration failed\n");
	}
	return 0;
}
//extern int unregister_filesystem(struct file_system_type *);
static void ifs_exit(void){
	int ret=0;
	DPRINTK("%s called\n",__func__);
	ret=unregister_filesystem(&ifs_type);
	if(ret){
		DPRINTK("ifs unregister failed\n");
	}
}
MODULE_AUTHOR("Somnath");
MODULE_LICENSE("GPL");
module_init(ifs_init);
module_exit(ifs_exit);
