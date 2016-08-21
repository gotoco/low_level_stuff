KDIR := /usr/src/linux-headers-`uname -r`/
PWD := $(shell pwd)

obj-m += sysfs_ex.o 
obj-m += sysfs_bus.o

all:
	make -C $(KDIR) \
                SUBDIRS=$(shell pwd) modules

clean:
	make -C $(KDIR) \
                SUBDIRS=$(shell pwd) clean
