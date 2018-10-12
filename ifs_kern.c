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
	buf->f_type=0;
	buf->f_bsize=PAGE_SIZE; // RP: what if other fields are not filled, they should be zero. ===>> done
        buf->f_blocks=PAGE_SIZE;
        buf->f_bfree=0;
        buf->f_bavail=0;
        buf->f_files=0;
        buf->f_ffree=0;
        //buf->f_fsid=0;
        buf->f_namelen=0;
        buf->f_frsize=0;
        buf->f_flags=0;
        //buf->f_spare[4]=0;

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
	.drop_inode     = generic_delete_inode,
};

//int (*fill_super)(struct super_block *, void *, int)

int ifs_fill_super(struct super_block *sb, void *data, int silent){
	int status=0;
	struct inode *root_inode=NULL;
	sb->s_blocksize		= PAGE_SIZE;
	sb->s_blocksize_bits    = PAGE_SHIFT;
	sb->s_maxbytes          = MAX_IFS_FILESIZE; // RP: wrong way to use., it should be your own design decision ===>> designed
	sb->s_op		= &ifs_super_operations;
        sb->s_magic             = IFS_MAGIC;
	sb->s_time_gran         = 1; // should know what does it mean, if you  use code ===>> Granularity of c/m/atime in ns
	
	root_inode=ifs_new_inode(sb, NULL,S_IFDIR|0755);
	sb->s_root = d_make_root(root_inode);
	if (!sb->s_root){
		status=-ENOMEM; // RP: does d_make_root always fail only with ENOMEM? [ It returns %NULL if there is insufficient memory available. ]
		goto out;		
	}
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
		return NULL; //if error return error.
	}
	DPRINTK("EOF ifs_mount\n");
	return dt; // RP: is it the right error to return ===>> Resloved.
}
void ifs_kill_sb(struct super_block *sb){
	DPRINTK("%s called.\n",__func__);
	kill_litter_super(sb); // RP: whats does this API does and why not use other generic kill_super APIs? ===>> kill_litter_super() is for in memory fs.
}
struct file_system_type ifs_type={
	.name="ifs",
	.fs_flags=FS_USERNS_MOUNT, //RP: whats the use of this flag? ===>> fs is not disk based, allows the mountin inside unprivileged mount namespaces,can be mounted by userns root.
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
