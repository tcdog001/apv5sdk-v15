KDIR	:= $(KERNELPATH)
PWD	:= $(shell pwd)
ROOTDIR := $(PWD)/modules
ARC 	:=powerpc
#CROSS_CC :=armeb-linux-
CROSS_CC :=powerpc-apm-linux-gnu-
export KDIR
export ROOTDIR
export CROSS_CC

all: default

default:
	make ARCH=$(ARC) CROSS_COMPILE=$(CROSS_CC) -C $(KDIR) M=$(PWD)/modules modules

clean:
	cd modules && make clean
