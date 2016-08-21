#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gotoc.io");
MODULE_DESCRIPTION("Registration of new bus my_new_bus");

/* 
 * The parameter value shows up in:
 * /sys/module/sysfs_bus/parameters/value and /sys/bus/my_new_bus/busval 
 */
static int example_value;
module_param_named(value, example_value, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(value, "a test value");

/* 
 * bus_show and bus_store is executed when it is read and written
 * respectively to /sys/bus/my_new_bus
 */
static int bus_value = 1;

static struct bus_type bus = {
	.name = "my_new_bus",
};

static 
ssize_t bus_show(struct bus_type *bus, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%d\n", bus_value);
}

static 
ssize_t bus_store(struct bus_type *bus, const char *buf, size_t count)
{
	sscanf(buf, "%d", &bus_value);
        return sizeof(int);
}
BUS_ATTR(busval, S_IRUGO | S_IWUSR, bus_show, bus_store);

static 
int __init sysfsexample_module_init(void)
{
	int ret = -1;
	
	ret = bus_register(&bus);
	if (ret < 0) {
             	printk(KERN_WARNING "#: sysfs: error register bus: %d\n", ret);
		return ret;
     	}

	ret = bus_create_file(&bus, &bus_attr_busval);
	if (ret < 0) {
		printk(KERN_WARNING "#: sysfs: error creating busfile\n");
		bus_unregister(&bus);	
	}

	if(example_value != 0)
		bus_value=example_value;

	return 0;
}
module_init(sysfsexample_module_init);

static 
void __exit sysfsexample_module_exit(void)
{
	bus_remove_file(&bus, &bus_attr_busval);
	bus_unregister(&bus);
}
module_exit(sysfsexample_module_exit);
