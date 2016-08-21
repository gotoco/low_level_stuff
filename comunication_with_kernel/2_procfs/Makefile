KDIR := /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd)

obj-m += procfs.o

all:
	make -C $(KDIR) \
                SUBDIRS=$(shell pwd) modules

clean:
	make -C $(KDIR) \
                SUBDIRS=$(shell pwd) clean
