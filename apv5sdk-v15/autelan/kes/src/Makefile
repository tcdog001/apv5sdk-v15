obj-m    := kes.o
kes_mod-objs := kes.o
CROSS_COMPILE="$(TARGET_CROSS)" \
ARCH="$(LINUX_KARCH)" \
EXTRA_CFLAGS="$(BUILDFLAGS) $(INCS)"
#CFLAGS_kes.o += -g

all: 
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

	
clean:
	$(RM) *.o *.ko
