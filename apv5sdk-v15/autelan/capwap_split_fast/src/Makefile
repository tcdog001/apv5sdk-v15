
obj-m	= capwap_split_fast.o

ifdef PC018
	EXTRA_CFLAGS+=-DPC018
#add start for lte-fi by liuhj//20130709
ifdef LTEFI
	EXTRA_CFLAGS+=-DLTEFI
endif
ifdef LTEFI_V2
	EXTRA_CFLAGS+=-DLTEFI_V2
endif
#add end for lte-fi by liuhj  //20130709
endif
ifdef APM82181
	EXTRA_CFLAGS+=-DAPM82181
endif
all:
	
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules
	
clean:
	$(RM) *.o *.ko
