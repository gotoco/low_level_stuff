# The simplest rootkit 

Simplest Rootkit is a example that show what kind of kernel programming techniques can be and also cannot be (because kernel is specific place) consider during developing (lets call it) rootkits :)

My goal was to show some entry point that will "do something" "be really easy" and can be further extended with some other functionalities. Because learning by doing is the best way to be hacker.

====

About content of this dir:

Makefile - simple build whole modules using make command, for ARM also can be builded using (for my A20 board): make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-

rootkitmod.c - simple kernel module that delete themself from modules list names after loading (it is invisible for lsmod)    
rootkitmod_time.c  - using simple timer in kernel module
rootkitmod_wrong.c - example what should not be doing in kernel/timer, DONT RUN ON YOUR WORKING MACHINE (can crash Your system, use for dev boards)
rootkitmod_time_usp.c - some functional example, that run after small delay user-space process, this file can be start point for writing Your funnt rootkits
