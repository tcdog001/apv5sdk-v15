obj-m := QCUSBNet.o
QCUSBNet-objs := GobiUSBNet.o QMIDevice.o QMI.o

#KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
default:
	$(MAKE) CROSS_COMPILE=$(TOOLPREFIX) -C $(KERNELPATH) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.* modules.order


