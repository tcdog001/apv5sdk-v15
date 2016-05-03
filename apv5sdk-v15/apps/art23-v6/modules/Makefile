export KDIR=$(PWD)/../../../linux/kernels/powerpc-linux-2.6.36
export KERNELPATH=$(PWD)/../../../linux/kernels/powerpc-linux-2.6.36

export POWERPC=1;

all:
	cd ${PWD}/driver/linux && \
	make -f makefile.artmod clean && \
	make -f makefile.artmod all POWERPC=1
	cd ${PWD}/art && \
	make -f makefile.nart POWERPC=1
clean:
	cd ${PWD}/driver/linux && \
	make -f makefile.artmod clean
	cd ${PWD}/art && \
	make -f makefile.nart clean
