KDIR := #HERE your kernel dir in example /home/gotoc/olimex/A20/linux-sunxi
PWD := $(shell pwd)

obj-m += rootkitmod.o
obj-m += rootkitmod_time.o
obj-m += rootkitmod_time_usp.o
obj-m += app.o
obj-m += tasklet.o

all:
	make -C $(KDIR) \
                SUBDIRS=$(shell pwd) modules

clean:
	make -C $(KDIR) \
                SUBDIRS=$(shell pwd) clean
