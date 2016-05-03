lc_ether-objs +=  qmi_oper.o lc_cdc_ether.o 
obj-m +=  lc_ether.o

CROSS_COMPILE="$(TARGET_CROSS)" \
ARCH="$(LINUX_KARCH)" \
EXTRA_CFLAGS="$(BUILDFLAGS) $(INCS)"
ifdef AQ2000_X
	EXTRA_CFLAGS+=-DAQ2000_X
endif

all: 
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

clean:
	$(RM) *.o *.ko *~ core* .dep* .*.d .*.cmd *.mod.c *.a *.s .*.flags .tmp_versions Module.symvers Modules.symvers *.order -R

