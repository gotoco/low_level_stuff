KDIR := /home/hjacker/olimex/A20/linux-sunxi #HERE your kernel dir in example
PWD := $(shell pwd)

obj-m += rootkitmod.o
obj-m += rootkitmod_time.o
obj-m += rootkitmod_wrong.o
obj-m += rootkitmod_time_usp.o

all:
	make -C $(KDIR) \
                SUBDIRS=$(shell pwd) modules

clean:
	make -C $(KDIR) \
                SUBDIRS=$(shell pwd) clean
