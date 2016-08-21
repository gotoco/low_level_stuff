obj-m += lm_param.o 
 
all:
	make -C /lib/modules/$(shell uname -r)/build \
		SUBDIRS=$(shell pwd) modules
 
clean:
	make -C /lib/modules/$(shell uname -r)/build \
		SUBDIRS=$(shell pwd) clean
