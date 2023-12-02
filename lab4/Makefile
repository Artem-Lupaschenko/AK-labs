obj-m += hello1.o hello2.o

KDIR ?= /lib/modules/`uname -r`/build
INC_DIR := $(PWD)/inc

ifeq ($(KERNELRELEASE),)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) CCFLAGS-y=-I$(INC_DIR)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
else
# kbuild part of makefile
ccflags-y += -I$(INC_DIR)
endif
