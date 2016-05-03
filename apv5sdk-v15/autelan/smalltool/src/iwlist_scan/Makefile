TARGET:=iwlist_scan
CC:=gcc
RM:=rm -rf 
CFLAGS:=  -I../../../auteos/src/include
CLIBS:=-L../../../auteos/src/src/ -lauteos
#-I/home/shark/new_cavium/cavium-sdk-v13/builddir/source/madwifi-trunk-r3314-wapi/src 

ifeq ($(PC018),1)
CFLAGS+= -DPC018=1
endif

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(EXTRA_CFLAGS) $(CLIBS) -o ${TARGET} ${TARGET}.c -lpthread
clean:
	$(RM) ${TARGET} ${SET_MAC} ${PACKET} ${SERVER} ${CLIENT} ${RATE_STAT} ${TTY_CRL_READ} ${TTY_CRL_WRITE} ${LOOPBACK} $(CPU_UTIL_RATE) $(CPU_UTIL_CHECK) ${WTPD_ALARM} 
