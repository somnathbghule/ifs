#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <ifs_inode.h>


/*

# create and mount a 100 megabyte (100M) temporary memory filesystem
	sudo mount -osize=100m ifs /mnt -t ifs
*/


//int (*fill_super)(struct super_block *, void *, int)

int ifs_fill_super(struct super_block *sb, void *data, int silent){
	int status=0;
	struct inode *root_inode=NULL;
	root_inode=ifs_new_inode(sb);
	if(!root_inode){
		status=-1;
		goto out;
	}
	sb->s_root = d_make_root(root_inode);
	if (!sb->s_root){
		status=-ENOMEM;
		goto out;		
	}
	printk("EOF ifs_fill_super\n");
out:
	return status;
}


//struct dentry *(*mount) (struct file_system_type *, int, const char *, void *);
//struct dentry *mount_nodev(struct file_system_type *fs_type, int flags, void *data, int (*fill_super)(struct super_block *, void *, int))
struct dentry *ifs_mount(struct file_system_type *fstype, int mount_flags, const char *dev_name, void *data){
	struct dentry *dt=NULL;
	printk("%s called\n", __func__);
	dt=mount_nodev(fstype, mount_flags, data, ifs_fill_super);
	if(!dt){
		printk("mount dentry is NULL\n");
	}
	printk("EOF ifs_mount\n");
	return dt;
}
struct file_system_type ifs_type={
	.name="ifs",
	.fs_flags=FS_BINARY_MOUNTDATA,
	.owner=THIS_MODULE,
	.mount=ifs_mount,
};

//extern int register_filesystem(struct file_system_type *);
static int ifs_init(void){
	int ret=0;
	printk(KERN_INFO"%s called\n",__func__);
	ret=register_filesystem(&ifs_type);
	if(ret){
		printk("ifs registration failed\n");
	}
	return 0;
}
//extern int unregister_filesystem(struct file_system_type *);
static void ifs_exit(void){
	int ret=0;
	printk(KERN_INFO"%s called\n",__func__);
	ret=unregister_filesystem(&ifs_type);
	if(ret){
		printk("ifs unregister failed\n");
	}
}
MODULE_AUTHOR("Somnath");
MODULE_LICENSE("GPL");
module_init(ifs_init);
module_exit(ifs_exit);
