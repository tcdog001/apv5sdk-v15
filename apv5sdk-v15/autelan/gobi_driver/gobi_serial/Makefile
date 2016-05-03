obj-m := GobiSerial.o
#GobiSerial-objs := usb-serial.o

#KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
#OUTPUTDIR=/lib/modules/`uname -r`/kernel/drivers/usb/serial/

all: clean
	$(MAKE) CROSS_COMPILE=$(TOOLPREFIX) -C $(KERNELPATH) SUBDIRS=$(PWD) modules

#install: all
#	mkdir -p $(OUTPUTDIR)
#	cp -f GobiSerial.ko $(OUTPUTDIR)
#	depmod

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.* modules.order
