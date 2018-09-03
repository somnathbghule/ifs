#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

//extern int register_filesystem(struct file_system_type *);
//extern int unregister_filesystem(struct file_system_type *);

struct file_system_type ifs_type={
	.name="ifs",
	.owner=THIS_MODULE,
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
