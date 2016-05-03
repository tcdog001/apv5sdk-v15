
obj-m	= capwap_split_fast.o

ifdef PC018
	EXTRA_CFLAGS+=-DPC018
endif
ifdef APM82181
	EXTRA_CFLAGS+=-DAPM82181
endif
all:
	
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules
	
clean:
	$(RM) *.o *.ko
