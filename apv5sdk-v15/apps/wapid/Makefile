
#include $(prefix)/Rules.elinux
CC = $(TOOLPREFIX)gcc
LD = $(TOOLPREFIX)ld
AR = $(TOOLPREFIX)ar
STRIP = $(TOOLPREFIX)strip

LIBDIR=./lib
ECCLIB=-lecl
SMS4LIB=sms4
OBJSDIR=obj
APPDIR=./app
INCLUDE=-I./include -I./lib/ECC/ -I$(WLANINC)
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(OBJSDIR)/%.o,$(SRCS))
CFLAGS += -fsigned-char -W -Wall $(INCLUDE) -D$(TARGET) -DLSDK6 -DCGI -DCTRL_UNIX

GWTOP = ../..

ifndef TOPDIR
TOPDIR=$(GWTOP)
endif

ifndef ATHDIR
ATHDIR = \
	$(TOPDIR)/autelan/aquila/wlan
endif

CFLAGS += -I $(ATHDIR) -I $(ATHDIR)/os/linux/include -I $(ATHDIR)/include -DDOT11_WAPI_MIB
ifdef KERNELPATH
CFLAGS += -I $(KERNELPATH)
endif

all: clean wapilib wapid apps

WAPI_SUB_DIRS=./lib ./app
$(OBJSDIR)/%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 
wapid: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -L${LIBDIR} -l$(SMS4LIB) $(ECCLIB) $(DL)
	$(STRIP) $@
	cp $@ $(INSTALL_ROOT)/sbin/.
#	cp -f wapi.conf $(INSTALL_ROOT)/etc
wapilib:
	test -e $(OBJSDIR) || mkdir -p $(OBJSDIR)
	for dir in $(WAPI_SUB_DIRS) ; do (cd $$dir && $(MAKE) all) || exit 1; done
apps:
	for dir in $(APPDIR) ; do (cd $$dir && $(MAKE) all) || exit 1; done
clean:
	rm -rf *.o *.a $(OBJSDIR)/*.o wapid 
	for dir in $(WAPI_SUB_DIRS) ; do (cd $$dir && $(MAKE) clean) || exit 1; done
