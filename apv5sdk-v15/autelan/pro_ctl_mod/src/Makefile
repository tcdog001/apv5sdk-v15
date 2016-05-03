obj-m    := pro_ctl_mod.o
pro_ctl_mod-objs := pro_ctl_mod_ap83.o
CROSS_COMPILE="$(TARGET_CROSS)" \
ARCH="$(LINUX_KARCH)" \
EXTRA_CFLAGS="$(BUILDFLAGS) $(INCS)"
ifdef AQ2000_X
	EXTRA_CFLAGS+=-DAQ2000_X
endif
ifdef LTEFI
	EXTRA_CFLAGS+=-DLTEFI
endif
ifdef LTEFI_V2
	EXTRA_CFLAGS+=-DLTEFI_V2
endif
ifdef AK2000_EN2T
	EXTRA_CFLAGS+=-DAK2000_EN2T
endif
ifdef PC018
	EXTRA_CFLAGS+=-DPC018
endif
ifdef XJ9344
	EXTRA_CFLAGS+=-DXJ9344
endif
ifdef XH9344
	EXTRA_CFLAGS+=-DXH9344
endif
ifdef AQ2000N
	CFLAGS+=-DAQ2000N
endif
ifdef WNAP210
	CFLAGS+=-DWNAP210	
endif
ifdef WNAP350
	CFLAGS+=-DWNAP350
endif
ifdef WNAP550
	CFLAGS+=-DWNAP550
endif
ifdef XJAP93
	CFLAGS+=-DXJAP93
endif
ifdef XJAP93_02
	CFLAGS+=-DXJAP93_02	
endif
ifdef AP2400
	CFLAGS+=-DAP2400
endif
ifdef XJPB44_02
	CFLAGS+=-DXJPB44_02
endif
ifdef TDPB44_02
	CFLAGS+=-DTDPB44_02
endif
ifdef XHAP93
	CFLAGS+=-DXHAP93
endif
ifdef XHPB44
	CFLAGS+=-DXHPB44
endif
ifdef AE5000ENT
	 EXTRA_CFLAGS+=-DAE5000ENT
endif

all: 
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

#pro_ctl_mod.o: $(pro_ctl_mod-obj)
#	mips-linux-uclibc-ld $(LDOPTS) -o pro_ctl_mod.ko -r $(pro_ctl_mod-objs)
	
clean:
	$(RM) *.o *.ko
