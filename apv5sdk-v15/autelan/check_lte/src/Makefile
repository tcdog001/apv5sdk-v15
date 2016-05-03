obj-m    := acc_mod.o
acc_mod-objs := check_acc.o

ifeq ($(BOARD_TYPE),amcc)
CROSS:=powerpc-apm-linux-gnu-
else
CROSS:=mips-linux-
endif

EXTRA_CFLAGS:=-Wall -Wno-unused -g -I.
LDFLAG:=
#TARGET:=check_lte
CC:=$(CROSS)gcc
RM:=rm -rf 

ifdef LTEFI
	EXTRA_CFLAGS+=-DLTEFI
endif
ifdef LTEFI_V2
	EXTRA_CFLAGS+=-DLTEFI_V2
endif

all:clean
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules
	$(CC) $(EXTRA_CFLAGS) $(LFLAGS) -o check_lte check_lte.c -lpthread
	$(CC) $(EXTRA_CFLAGS) $(LFLAGS) -o acc_setget acc_setget.c
	$(CC) $(EXTRA_CFLAGS) $(LFLAGS) -o check_log check_log.c -lpthread
clean:
	$(RM) $(TARGET) *.o *.ko *.mod.c .*.cmd *.order *.symvers .tmp_versions
