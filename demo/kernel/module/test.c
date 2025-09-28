#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static const char device_name[] = "hello_driver";

int my_open(struct inode *, struct file *);
int my_release(struct inode *, struct file *);
ssize_t my_read(struct file *, char __user *, size_t , loff_t *);
ssize_t my_write(struct file *, const char __user *, size_t , loff_t *);

struct file_operations fops = {
	.owner=THIS_MODULE,
	.read=my_read,
	.write=my_write,
	.release=my_release,
	.open=my_open,
};

int my_open(struct inode *inode, struct file *file)
{
	printk("my_open() execute successfully!\n");
	return 0;
}

int my_release(struct inode *inode, struct file *file)
{
	printk("my_release() execute successfully!\n");
	return 0;
}

ssize_t my_read (struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	int ret = 0;
	printk("my_read() execute successfully!\n");
	return ret;
}

ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	int ret = 0;
	printk("my_write() execute successfully!\n");
	return ret;
}

static struct miscdevice cdfinger_dev = {
    .minor = 255,
    .name = "test_node",
    .fops = &fops,
};

int __init my_init(void)
{
	if (misc_register(&cdfinger_dev) < 0) {
		printk( KERN_NOTICE "hello-driver: reg misc drv failed\n" );
		return -1;
	}
	printk( KERN_NOTICE "hello-driver: my_init() is called.\n" );

	return 0;	
}

void __exit my_exit(void)
{
	printk( KERN_NOTICE "hello-driver: my_exit() is called\n" );

	misc_deregister(&cdfinger_dev);
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL"); 
