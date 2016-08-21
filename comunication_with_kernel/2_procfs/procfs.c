#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gotoc.io");
MODULE_DESCRIPTION("Simple procfs usage");

static struct proc_dir_entry *my_dir, *my_file;

static char my_data[256];

static 
int my_proc_show(struct seq_file *sf, void *v)
{
        seq_printf(sf, "%s \n", my_data);
	return 0;
}

static 
int my_proc_open(struct inode *inode, struct file *file)
{
	printk("#: my_proc open data:: %s\n", my_data);
	return single_open(file, my_proc_show, PDE_DATA(inode));
}

static
int my_write(struct file *seq, const char *data, size_t len)
{
	int i;
	for(i=0; i<len-1 && i<sizeof(my_data)-1; i++)
		my_data[i] = *(char *)(data+i);
	my_data[i]= '\0';

	return -1;
}

static 
const struct file_operations my_proc_fops = {
        .open           = my_proc_open,
        .read           = seq_read,
	.write 		= my_write,
	.llseek         = seq_lseek,
        .release        = single_release,
};

static 
int __init my_module_init(void)
{
	printk("#: init my procfs module! \n");
	/* create a directory */
	my_dir = proc_mkdir("my_test", NULL);
        if(my_dir == NULL)
               goto error; 
        
        /* create a file and initialize file ops*/
        my_file = proc_create_data("my_file", 0644, my_dir, &my_proc_fops, my_data);
        if(NULL == my_file) {
		remove_proc_entry("my_test", NULL);
		goto error;	
	}
	strcpy(my_data, "initial content \n");
        return 0;
error:
	return -ENOMEM;
}
module_init(my_module_init);

static 
void __exit my_module_exit(void)
{
	printk("#: cleanup after my module \n");
	remove_proc_entry("my_file", my_dir);
	remove_proc_entry("my_test", NULL);
}
module_exit(my_module_exit);

