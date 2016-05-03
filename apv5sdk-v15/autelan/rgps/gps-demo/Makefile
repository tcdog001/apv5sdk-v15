INSTALL_ROOT = $(shell pwd)/install
BIN_INSTALL_DIR = /bin

#export INSTALL_ROOT
INSTALL_ROOT=./install
all: gps

install: all
	cp -fr gps $(INSTALL_ROOT)$(BIN_INSTALL_DIR)

QMILIB = -L$(INSTALL_ROOT)/lib -lqmi -lqmiservices -lqmi_client_qmux -lqmiidl -lqcci_legacy -lpthread  -lgps

DEPEND_FILES = nas.o wan.o
DEPEND_FILES += wncqmi.o wncqmi_utils.o wncqmiwds.o wncqminas.o wncqmidms.o 
DEPEND_FILES += gps.o

GPS_DEPEND_FILES = nas.o wan.o wncqmi.o wncqmi_utils.o wncqmiwds.o wncqminas.o wncqmidms.o

IFLAGS 	+= -I.. -I./inc -I./install/include/qmi
LDFLAGS	+= $(QMILIB) 

#CROSS = /opt/atheros-920405/build_mips/staging_dir/usr/bin/mips-linux-uclibc-

CC = $(CROSS)gcc
CXX = $(CROSS)g++
LD = $(CROSS)ld
AR = $(CROSS)ar
STRIP = $(CROSS)strip

gps: libgps Makefile $(DEPEND_FILES) gpsmain.o 
	$(CC) -o gps $(CFLAGS) -I./inc gpsmain.o gps.o $(LDFLAGS)  -L.

libgps:
	$(AR) rcs libgps.a $(GPS_DEPEND_FILES)

clean: 
	rm -f gps
	rm -fr gps.o gpsmain.o
	rm -f libgps.a

.c.o:
	$(CC) -c -o $@ $(DFLAGS) $(CFLAGS) $(IFLAGS) $<

