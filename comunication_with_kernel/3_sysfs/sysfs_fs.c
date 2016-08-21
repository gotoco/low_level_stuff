#include<linux/module.h>
#include<linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gotoc.io");
MODULE_DESCRIPTION("Demonstration of file operations on kobjects");

extern struct kobject *fs_kobj;
 
static struct kobject my_kobj;

static 
struct attribute my_read_entry = { .name = "read_table", .mode = S_IRUGO | S_IWUSR };

static 
struct attribute my_write_entry = {.name = "write_table", .mode = S_IRUGO | S_IWUSR };

static 
struct attribute *default_attrs[] = {
        &my_read_entry,
        &my_write_entry,
        NULL,
};

ssize_t my_show(struct kobject *kobj, struct attribute *attr, char *buffer)
{
	printk(KERN_INFO "#: my_show! \n");

	return 0;
}

ssize_t my_store(struct kobject *kobj, struct attribute *attr, const char *buffer, size_t size)
{
	printk(KERN_INFO "#: my store! \n");

	return size;
}


static struct sysfs_ops my_sysfs_ops = {
        .show = my_show,
        .store = my_store,
};

static struct kobj_type my_type = {
        .sysfs_ops = &my_sysfs_ops,
        .default_attrs = default_attrs,
};

struct sysfs_ops my_op;

static int __init sysfsexample_module_init(void)
{
	memset(&my_kobj, 0x00, sizeof(my_kobj)); 
	kobject_init_and_add(&my_kobj, &my_type, fs_kobj, "%s", "my_test");

	return 0;
}
module_init(sysfsexample_module_init);

static void __exit sysfsexample_module_exit(void)
{
	kobject_put(&my_kobj);
}
module_exit(sysfsexample_module_exit);
