# 
# Makes spectraldaemon tools
#

ifeq ($(obj),)
obj := .
endif


# General path related variables
# XXX : Review and purge
#

SPECTRAL_INC := $(ATHEROSPATH)/lmac/spectral
COMMON_INC := ./common
DEPTH := ../../drivers/wlan
BINDIR= /usr/local/bin
HAL_INC := ../../drivers/wlan/hal/
CLASSIFIER_SRC := ./common

include $(obj)/${DEPTH}/os/linux/Makefile.inc


# Spectral
ifneq ($(strip ${ATH_SUPPORT_SPECTRAL}),0)
ALL+= spectraldaemon spectraldemo
SPECTRAL_TOOLS+= spectraldaemon spectraldemo
endif

all: ${ALL}

INCS+= -I$(ATHEROSPATH) -I${HAL} -I${HAL}/${OS} -I${ATH_HAL} -I${obj}/${DEPTH} 
INCS+= -I${HAL_INC} -I${COMMON} -I${ATH} -I${WLAN} -I${COMMON_INC} -I${SPECTRAL_INC}
CFLAGS= ${INCS} ${COPTS}

ifeq (${BUILD_STATIC}, y)
LDFLAGS= -static
endif

ifeq (${BUILD_DEBUG}, y)
CFLAGS+= -g
endif

ifeq (${BUILD_OPTIMIZED}, y)
CFLAGS += -Os -pipe -mips32r2 -mtune=mips32r2 -funit-at-a-time
endif

spectraldemo: spectraldemo.c daemon.c ${CLASSIFIER_SRC}/classifier.c
	${CC} -o spectraldemo ${CFLAGS} ${LDFLAGS} spectraldemo.c daemon.c ${CLASSIFIER_SRC}/classifier.c $(CLASSIFIER_SRC)/classifier_init.c -lm -static
spectraldaemon: daemon.c $(CLASSIFIER_SRC)/classifier_init.c ${CLASSIFIER_SRC}/classifier.c spectral_samp.c spectral_test_server.c spectral_parse_req.c spectral_parse_data.c spectral_build_rsp.c spectral_daemon_eacs.c spectral_save_samp_msgs.c spectral_debug.c spectral_interf.c
	${CC} -o spectraldaemon ${CFLAGS} ${LDFLAGS} daemon.c $(CLASSIFIER_SRC)/classifier_init.c ${CLASSIFIER_SRC}/classifier.c spectral_samp.c spectral_test_server.c spectral_parse_req.c spectral_parse_data.c spectral_build_rsp.c spectral_daemon_eacs.c spectral_save_samp_msgs.c spectral_interf.c spectral_debug.c -lm -static 
clean:
	@rm -rf spectraldaemon spectraldemo
 
