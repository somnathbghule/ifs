#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

//extern int register_filesystem(struct file_system_type *);
//extern int unregister_filesystem(struct file_system_type *);
//struct dentry *(*mount) (struct file_system_type *, int, const char *, void *);
//struct dentry *mount_nodev(struct file_system_type *fs_type, int flags, void *data, int (*fill_super)(struct super_block *, void *, int))

/*

# create and mount a 100 megabyte (100M) temporary memory filesystem
	sudo mount -osize=100m ifs /mnt -t ifs
*/
struct dentry *ifs_mount(struct file_system_type *fstype, int mount_flags, const char *dev_name, void *data){
	printk("%s called\n", __func__);
	return mount_nodev(fstype, mount_flags, data, NULL);
}
struct file_system_type ifs_type={
	.name="ifs",
	.fs_flags=FS_BINARY_MOUNTDATA,
	.owner=THIS_MODULE,
	.mount=ifs_mount,
};

static int ifs_init(void){
	int ret=0;
	printk(KERN_INFO"%s called\n",__func__);
	ret=register_filesystem(&ifs_type);
	if(ret){
		printk("ifs registration failed\n");
	}
	return 0;
}
static void ifs_exit(void){
	int ret=0;
	printk(KERN_INFO"%s called\n",__func__);
	unregister_filesystem(&ifs_type);
	if(ret){
		printk("ifs unregister failed\n");
	}
}
MODULE_AUTHOR("Somnath");
MODULE_LICENSE("GPL");
module_init(ifs_init);
module_exit(ifs_exit);
