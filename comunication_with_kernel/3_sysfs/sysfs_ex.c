#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>

#include <linux/types.h>
#include <linux/list.h>
#include <linux/sysfs.h>
#include <linux/compiler.h>
#include <linux/spinlock.h>
#include <linux/kref.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <asm/atomic.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gotoc.io");
MODULE_DESCRIPTION("Create new fs using kobjects");

extern struct kobject *fs_kobj;

static struct kobject my_kobj;

struct my_attr {
        struct attribute attr;
	int value;
};

static struct my_attr my_first = {
	.attr.name="first",
	.attr.mode = 0644,
	.value = 1,
};

static struct my_attr my_second = {
	.attr.name="second",
	.attr.mode = 0644,
	.value = 2,
};

static struct attribute * myattr[] = {
	&my_first.attr,
	&my_second.attr,
	NULL
};

static 
ssize_t default_show(struct kobject *kobj, struct attribute *attr,
                              char *buf)
{
	struct my_attr *a = container_of(attr, struct my_attr, attr);
	printk("#: show for kobject: %s, attr: %s \n", kobj->name, a->attr.name);
	return scnprintf(buf, PAGE_SIZE, "%d\n", a->value);
}

static 
ssize_t default_store(struct kobject *kobj, struct attribute *attr,
                               const char *buf, size_t len)
{
	struct my_attr *a = container_of(attr, struct my_attr, attr);
	printk("#: store for kobject: %s, attr: %s \n", kobj->name, a->attr.name);
       	sscanf(buf, "%d", &a->value);
	return sizeof(int);
}

static struct sysfs_ops myops = {
	.show = default_show,
	.store = default_store,
};

static struct kobj_type my_type = {
	.sysfs_ops = &myops,
	.default_attrs = myattr,
};

static 
int __init sysfsexample_module_init(void)
{
	int err;
        
	memset(&my_kobj, 0x00, sizeof(my_kobj));
	err = kobject_init_and_add(&my_kobj, &my_type, fs_kobj, "%s", "my_test");

	return err;
}
module_init(sysfsexample_module_init);

static 
void __exit sysfsexample_module_exit(void)
{
	kobject_put(&my_kobj);
}
module_exit(sysfsexample_module_exit);
