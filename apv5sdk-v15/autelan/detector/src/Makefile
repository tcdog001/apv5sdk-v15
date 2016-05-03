## detector makefile
# Supply your own C cross-compiler; I recommend the one from the OpenWRT buildroot
# Also requires a libpcap to link with, use libpcap.a for static, .so for shared
#CC=~/buildroot/staging_dir_mipsel/bin/mipsel-linux-gcc
#CC=/opt/toolchains/4.1.0-uclibc-0.9.28/bin/mipsel-linux-gcc
CC = ${CC}
#LIBS=libpcap.a

CCOPTS=$(COPTS)
INCLUDE=/opt/toolchains/4.1.0-uclibc-0.9.28/includ
SOURCES=detector.c 
OBJS=detector.o  

TARGET=detector

.PHONY : ALL
ALL : TARGET 

.PHONY : TARGET
TARGET: ${OBJS}
	${CC} ${CCOPTS} -I${INCLUDE} -o ${TARGET} ${OBJS} -L/BUILDDIR/gcc-4.3.3/build_mips/staging_dir/lib -lpthread
#${LIBS}
	

.PHONY : detector.o
detector.o: detector.c
	${CC} ${CCOPTS} -I${INCLUDE} -c ${SOURCES} -L/BUILDDIR/gcc-4.3.3/build_mips/staging_dir/lib -lpthread
	
#.PHONY : wl_access.o
#wl_access.o: wl_access.c
#	${CC} ${CCOPTS} -I${INCLUDE} -c ${SOURCES}
	
#.PHONY : channelhopper.o
#channelhopper.o: channelhopper.c
#	${CC} ${CCOPTS} -I${INCLUDE} -c ${SOURCES}
	
#.PHONY : nvram.o
#nvram.o: nvram.c
#	${CC} ${CCOPTS} -I${INCLUDE} -c ${SOURCES}
	
remake:
	touch detector.c 

clean:
	rm -f *.o *.c~ *.h~ detector 
