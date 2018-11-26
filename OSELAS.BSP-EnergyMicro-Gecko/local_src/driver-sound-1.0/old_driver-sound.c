/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/sched.h>

#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

#include "efm32gg.h"
/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */
 
 
 static char* sound_remap;
 static struct class *cl;
 static struct cdev my_cdev;
 static dev_t dev;
 static int error;
 
static int sound_open(struct inode *inode, struct file *filp);
static int sound_release(struct inode *inode, struct file *filp);
static int sound_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t sound_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

static struct file_operations sound_fops = 
{
	.owner = THIS_MODULE,
	.open = sound_open,
	.release = sound_release,
	.read = sound_read,
	.write = sound_write
};

static int __init template_init(void)
{
	
	//0x40004000 = DAC0_BASE2
	request_mem_region(0x40004000, 0x024, "Sound");
	sound_remap = ioremap_nocache(0x40004000, 0x024);
	
	
	//Enable DAC0_CH0CTRL (?)
	sound_remap[8] = 1;
	
	//Enable DAC0_CH1CTRL (?)
	sound_remap[12] = 1;
	
	error  = alloc_chrdev_region(&dev, 0, 1, "Sound");
	
	if(error < 0){
		printk("ERROR Allocating character device in sound driver");
	}	
	
	cdev_init(&my_cdev, &sound_fops);
	
	error = cdev_add(&my_cdev, dev, 1);
	if (error < 0)
	{
		printk("ERROR Adding character device?\n");
	}
	
	cl = class_create(THIS_MODULE, "sound");
	device_create(cl, NULL, dev, NULL, "sound");
	
	printk("Hello World, here is your module speaking\n");
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
	device_destroy(cl, dev);
	class_destroy(cl);

	printk("Short life for a small module...\n");
}

static int sound_open(struct inode *inode, struct file *filp)
{
	return 0;
}
static int sound_release(struct inode *inode, struct file *filp)
{
	return 0;
}
static int sound_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	
	return 0;
}

static ssize_t sound_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	return 0;
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");