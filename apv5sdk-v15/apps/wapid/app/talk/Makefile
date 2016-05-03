CC = $(TOOLPREFIX)gcc
LD = $(TOOLPREFIX)ld
AR = $(TOOLPREFIX)ar
STRIP = $(TOOLPREFIX)strip

CFLAGS+=-I../../include -DDOT11_WAPI_MIB

all: clean wapitalk

wapitalk: wapitalk.o wapi_ctrl.o wapi_mib.o
	$(CC) $(CFLAGS) -o $@ $^ 
	$(STRIP) $@
	cp $@ $(INSTALL_ROOT)/sbin/.

wapitalk.o: wapitalk.c

wapi_ctrl.o: wapi_ctrl.c

clean:
	rm -rf *.o *.a wapitalk 


