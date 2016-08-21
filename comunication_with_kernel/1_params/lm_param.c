#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("gotoc.io");
MODULE_DESCRIPTION("Simple echo functionality using module params");

static int cnt = 1;
module_param(cnt, int, 0);
MODULE_PARM_DESC(cnt, "Number of args");

#define MAX_ENTITIES 10
static char *args[MAX_ENTITIES] = { "params" };
module_param_array(args, charp, &cnt, 0644);
MODULE_PARM_DESC(args, "Params list.");

static 
void echo(void)
{
	int idx;
	printk(KERN_INFO "#: cnt: %d \n", cnt);
	for (idx = 0; idx < cnt; idx++)
		printk(KERN_INFO " %s; ",args[idx]);

	printk(KERN_INFO "\n");
}

static 
int __init echo_init(void)
{
	printk(KERN_INFO "#: Module init\n");
	echo();
	return 0;
}
module_init(echo_init);

static 
void __exit echo_exit(void)
{
	  printk(KERN_INFO "#: Module exit \n");
}
module_exit(echo_exit);
