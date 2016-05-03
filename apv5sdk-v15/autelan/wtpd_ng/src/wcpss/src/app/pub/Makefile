
CC = mips-linux-gnu-gcc

#LDFLAGS = -lssl -lcrypto -lpthread -ldl -D_REENTRANT
#LDFLAGS = ./static/libssl.a ./static/libcrypto.a -lpthread -ldl -D_REENTRANT

CFLAGS = -Wall -g -O0 -D_REENTRANT -DCW_NO_DTLS -DCW_WID -I. -I../wid
CFLAGS += -I$(ACCAPI_DIR)

# Memory leak
#LDFLAGS += ../dmalloc-5.5.0/libdmallocth.a
#CFLAGS += -DDMALLOC

# Capwap Debugging
#CFLAGS += -DCW_DEBUGGING



RM = /bin/rm -f 

PUB_LIB = libwcpsspub.a

PUB_OBJS =  CWCommon.o CWConfigFile.o CWErrorHandling.o CWList.o CWLog.o CWSafeList.o CWNetwork.o CWProtocol.o CWRandom.o CWStevens.o CWThread.o CWBinding.o CWSecurity.o CWOpenSSLBio.o timerlib.o CWThread_wid.o

PUB_SRCS = $(PUB_OBJS:.o=.c) 



.PHONY: all 

all: $(PUB_OBJS) 
	$(AR) -r $(PUB_LIB) $(PUB_OBJS)


.PHONY: clean

clean: 
	$(RM) $(PUB_LIB) $(PUB_OBJS) 


