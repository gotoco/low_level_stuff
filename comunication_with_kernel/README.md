title: Communication with (linux) kernel
date:
categories:
- System Programming
tags:
- Kernel
- Linux
coverImage: http://res.cloudinary.com/gotocco/image/upload/v1467755373/radio-telescope-1031305_1920_jbhmgv.jpg
comments: false
---

## Introduction: Communication with linux kernel
The concept of operating systems is really old, one of the major advantages of using GPOS (general purpose operating system) is isolation.
Especially I left standalone adjective to suggest that this isolation is not related to just two components, but we can meet isolation on different levels and domains.
<!-- more -->

<!-- toc -->

### Isolation between kernel and user space
We can meet isolation when we are dealing with processes or memory. Also, isolation is used to separate place where programs are run (aka. user space) from a small area where hardware is a handle (aka. kernel space).
This separation mechanism makes user space independent from the kernel (and hardware), also kernel space can take advantage of this abstraction and don't care what is going on in user space but focus on its own tasks.
Everything will be found, except that when we need to make any communication with the external world (network, disk etc.) we have to back to basics of GNOS and be able to communicate between applications and physical devices.
In this article, we will go through most common interfaces that Linux kernel provides for user space to communicate with.    

### Parameters
Someone can ask, is true that we have to follow complicated way to be able interact with kernel?
Basically everything depends on module/driver. That is why need several different options to be able to meet requirements of driver.
We can compare this with running simple bash script called sample_script.sh, and we will add some additional parameters a, b

{% codeblock lang:bash %}
# run script with parameters a, b
#!/bin/bash
# for i in $*; do
#   echo $i
# done

:~/ $ sh sample_script.sh a b
{% endcodeblock %}

We can achieve the same thing with kernel:

{% codeblock lang:C %}
MODULE_DESCRIPTION("ECHO PARAMS");

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
{% endcodeblock %}

{% codeblock lang:bash %}
# insmod echo.ko cnt=2 args=a,b
# dmesg
# modinfo echo.ko
{% endcodeblock %}

Above examples are really similar from functional point of view. Both can return result based on input parameters list. Bash is a little bit cunning because it doesn't need names of parameters when we run it, while kernel does.
Although that our module was just print passed parameters, we already covered first way how we can send data to kernel.

## File based interfaces
Despite that for common linux user is hard to imagine his OS without some of file systems, there are configurable and they can be disable on kernel configuration phase.
File based system that is present in RAM is abstraction layer that can be used to pass commands to kernel and vice versa.
Because of that from user space program can read and write values using read write calls. Usually one file handle single value, but it is not a rule.
By handling single value by single file structure of filesystem is similar to key-value storage.
Also some of linux filesystems that are mounted to root comes from UNIX-like File System Hierarchy (FSH)
In linux kernel we have plenty of different filesystems, but here we will try to describe most popular and widely used for communication with kernel.
To see more linux fs you can visit [documentation](http://lxr.free-electrons.com/source/Documentation/filesystems/)

----
  ### Procfs
Mounted in /proc Procfs is virtual file system which provide mainly information about processes, opened files, statistics and kernel itself (/proc/sys/).
Many monitor type programs like top provide informations by reading from this FS.
Because of destination /proc should be only used to handle information about processes, and usually is deprecated as a way to share other non-process specific information.

To ilustrate how procfs is working we will create new file in `/proc/my_test/my_file`. Example on [github file procfs.c]()
Files operation for /proc kernel are handle by provided procfs API, which is using directories and data to build file system.
To create directory we are using `proc_mkdir` and for data `proc_create_data`. For data entries we have to specify `struct file_operations`. This large structure defined in [`<include/fs.h>`](http://lxr.free-electrons.com/source/include/linux/fs.h#L1670) contain a lot of available file operations. We will not cover them all, as it is not necessary, but there are many possible approaches how these operation can be implemented. In general we should search for other ready API that we can reuse rather than implementing each function by our own. To illustrate it we write own simple implementation on `write` function but for `read` and `open` we will reuse [seq_file API](http://lxr.free-electrons.com/source/fs/seq_file.c#L160), because of that open and read functions are really simple.

{% codeblock lang:bash %}
# insmod procfs.ko
# # Check initial content
# cat /proc/my_test/my_file
initial content
# # Test write
# echo 12345 > /proc/my_test/my_file
# cat /proc/my_test/my_file
12345
{% endcodeblock %}


----
  ### Sysfs
As a /proc came as a part of UNIX convention /sys is linux specific extension to /proc. Sysfs is a way to expose connected devices to kernel and also allow to their modifications.
Program to manage devices like udev <ndev?> use /sys to access device driver to change parameters or read informations.
Reason why linux introduce this file system was to tidy /proc file system from mostly hardware/devices specific informations.

As other described FS it can be defined using CONFIG_SYSFS=y.

Note <modify?>
/sys exposes kernel data structures and control points, which implies that it might contain circular references,
where a directory links to an ancestor directory. As a result, a find command used on /sys might never terminate.

To demonstrate how to interact with sysfs we will run two exercises:
  1. New filesystem in /sys/fs/ [sysfs_ex.c](github.com)
  2. New bus in /sys/bus/ [sysfs_bus.c](github.com)



First exercise: kObjects - way to create own structures inside kernel.

Starting from the end: kobjects are just a way to implement inheritance for Linux systems and drivers, each driver related to sysfs should handle reference to kobject (you can think about it as a common ancestor) So we have kobject that should be embedded inside driver structure. From other side each kernel object after creation has representation in file structure based other kobjects that inherit from root kobject and their attributes that makes sysfs a filesystem-based view of kernel objects
Each kobject has its kobj_type and kobj_type describe file operation on kobject. Everything can be specified at creation time.
To create own sile system inside /sys/fs what we have to do is create another kernel object that will have parent sysfs kernel object, in result we got my_test folder inside /sys/fs/, to create files inside this folder kobject need attributes.
Here we create my_attr type and next two attributes first and second.
Writing code that works with kobjects often has the problem: given a struct kobject pointer or attributes, what is the pointer to the containing structure?
To ommit needs for any hacks we use container_of to get reference to current attribute and determine if we are running store/show for first or second.
After compilation we can check that everything is working using bash:

{% codeblock lang:bash %}
# ls /sys/fs/my_test/
first  second
# ls /sys/fs/my_test/first
/sys/fs/my_test/first
# cat /sys/fs/my_test/first
1
# echo 45 > /sys/fs/my_test/first
# cat /sys/fs/my_test/first
45
# cat /sys/fs/my_test/second
2
# echo 34 > /sys/fs/my_test/second
# cat /sys/fs/my_test/second
34
{% endcodeblock %}

In dmesg we can see who our module handle show/store operation for different attributes.

{% codeblock lang:bash %}
# dmesg
[...] #: show for kobject: my_test, attr: first
[...]  #: store for  kobject: my_test, attr: first
[...]  #: show for  kobject: my_test, attr: first
[...]  #: show for  kobject: my_test, attr: second
[...]  #: store for  kobject: my_test, attr: second
[...]  #: show for  kobject: my_test, attr: second
{% endcodeblock %}

----
  ### Configfs

Configfs is a ram-based filesystem that provides the converse of sysfs's functionality.  Where sysfs is a filesystem-based view of kernel objects, configfs is a filesystem-based manager of kernelobjects, or config_items.
Configfs by itself has similar concept that sysfs, some people can have question about the distinction between configfs and sysfs.
The reason why configfs was introduced was to provide easier interface to make configuration of kernel objects.
Usually when in Unix-like system we need to make configuration by default ioctl can be first guess. Unfortunately from user point of view ioctl
is not easy to interact, also from programmer point of view ioctl is not the easy way. More user friendly can be to use file system approach.
One main difference between these file systems is that using configfs allow user to create new kernel objects and manage them, while sysfs is
a representation of existing present kernel objects.

To see this difference lets imagine embedded system with PWM driver. We need to allow user to dynamically create configure and manage PWMs.

{% codeblock lang:bash %}
# GPIO pin number
GPIO = 123

# create configurable GPIO PWM
mkdir /config/gpio_pwm/$GPIO
echo 1 > /sys/class/pwm/gpio_pwm\:$GPIO/export

# configure for 10 ms with a 50% duty cycle - simple 100 hz square wave
echo 10000000 > /sys/class/pwm/gpio_pwm\:$GPIO/period_ns
echo 5000000 > /sys/class/pwm/gpio_pwm\:$GPIO/duty_ns

# start the PWM
echo 1 > /sys/class/pwm/gpio_pwm\:$GPIO/run

# stop the PWM
echo 0 > /sys/class/pwm/gpio_pwm\:$GPIO/run

# destroy the configurable GPIO PWM
echo 1 > /sys/class/pwm/gpio_pwm\:$GPIO/unexport
rm -rf /config/gpio_pwm/$GPIO
{% endcodeblock %}

As additional exercise to understand this fs we will provide stub driver that will be able to provide same API that we used in BASH



----
  ### Debigfs

<example> + opis

----
  ### Sysctl

----
  ### Relayfs

----
  ## Character Devices

----
  ## Socket Based Mechanisms

  There are different socket families which can be used to achieve this goal, below are presented the most useful from perspective of exchanging data with kernel:

  AF_INET: designed for network communication, but UDP sockets can also be used for the communication between a kernel module and the user space. The use of UDP sockets for node local communication involves a lot of overhead.

  AF_PACKET: allows the user to define all packet headers.

  AF_NETLINK (netlink sockets): They are especially designed for the communication between the kernel space and the user space. There are different netlink socket types currently implemented in the kernel, all of which deal with a specific subset of the networking part of the Linux kernel.

  AF_ALG:

  ### UDP Sockets

  ### Netlink Sockets


----
  ## Ioctl    

----
  ## Kernel System Calls
