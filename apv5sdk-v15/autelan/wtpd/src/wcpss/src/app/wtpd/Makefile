#ifneq ($(strip $(TOOLPREFIX)),)
#CROSS:=$(TOOLPREFIX)
#endif
#CC = $(CROSS)gcc
#LDFLAGS = -L../pub -lwcpsspub -lpthread -ldl -D_REENTRANT
#LDFLAGS = -L../pub -lwcpsspub -lssl -lcrypto -lpthread -ldl -D_REENTRANT
#LDFLAGS = $(DTLSLDFLAGS) -L../pub -lwcpsspub -lpthread -ldl -D_REENTRANT
LDFLAGS = $(DTLSLDFLAGS) -L../pub -L../../../../../../auteos/src/src -lwcpsspub -lpthread -ldl -lauteos -D_REENTRANT
#LDFLAGS = ./static/libssl.a ./static/libcrypto.a -lpthread -ldl -D_REENTRANT


CFLAGS = $(DTLSCFLAGS) -Wall -g -O0 -D_REENTRANT  -I. -I../pub -DWRITE_STD_OUTPUT -I../../../../../../auteos/src/include $(EXTRA_CFLAGS)
#CFLAGS = $(DTLSCFLAGS) -Wall -g -O0 -D_REENTRANT  -I. -I../pub -DWRITE_STD_OUTPUT $(EXTRA_CFLAGS)
#CFLAGS = -Wall -g -O0 -D_REENTRANT -DCW_NO_DTLS  -I. -I../pub -DWRITE_STD_OUTPUT $(EXTRA_CFLAGS)
#-DCW_DEBUGGING

# Memory leak
#LDFLAGS += ../dmalloc-5.5.0/libdmallocth.a
#CFLAGS += -DDMALLOC

# Capwap Debugging
#CFLAGS += -DCW_DEBUGGING

RM = /bin/rm -f 

# list of generated object files for WTP. 
WTPD_OBJS = WTP.o WTPFrameReceive.o WTPConfigFile.o WTPProtocol.o WTPProtocol_User.o WTPDiscoveryState.o WTPJoinState.o WTPImageDataState.o WTPConfigureState.o WTPDataCheckState.o WTPRunState.o WTPRunStateCheck.o WTPRetransmission.o WTPSulkingState.o WTPBinding.o WTPDriverInteraction.o WTPSettingsFile.o 
 
WTPD_SRCS = $(WTPD_OBJS:.o=.c)

WTPD_DEPS := $(WTPD_OBJS:.o=.d)

# program executables. 
WTPD_APP = wtpd 

.PHONY: deps clean clean_libs libs

# top-level rule, to compile everything. 
all: pub clean $(WTPD_APP) install	
	@echo "##########################################"
	@echo "EXTRA_CFLAGS=${EXTRA_CFLAGS}"
	@echo "##########################################"
$(WTPD_APP): $(WTPD_OBJS) 
	$(CC) $(WTPD_OBJS) $(CFLAGS) $(LDFLAGS) -o $(WTPD_APP) 
install:
	cp wtpd ../../res
pub:
	cd ../pub && $(MAKE) -f Makefile_wtp
clean: 
	$(RM) $(WTPD_APP) $(WTPD_OBJS) $(WTPD_DEPS)

clean_deps:
	$(WTPD_DEPS)
	
deps: $(WTPD_SRC)
	$(CC) -MD -E -DWRITE_STD_OUTPUT $(WTPD_SRCS) $(CFLAGS) >/dev/null

-include $(WTPD_DEPS)
