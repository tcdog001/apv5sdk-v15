#
# Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
# Copyright (c) 2002-2005 Atheros Communications, Inc.
# All rights reserved.
#
DEPTH=	../../../..

ATHDIR=	${DEPTH}/lmac/ath_dev
HALDIR=	${DEPTH}/hal
LINUX_HALDIR=	${DEPTH}/hal/linux
LINUX_INCLUDE=	${DEPTH}/os/linux/include
LINUX_INCLUDE1=	${DEPTH}/include
LINUX_INCLUDE2=	${DEPTH}/asf/include
LINUX_INCLUDE3=	${DEPTH}/adf/include/os
LINUX_INCLUDE4=	${DEPTH}/adf/os/linux

#zhaoyang1 transplant from 717
ifeq ($(BOARD_TYPE),amcc)
include $(obj)/${DEPTH}/os/linux/Makefile.inc

endif
#zhaoyang1 transplant end
ifneq ($(strip $(TOOLPREFIX)),)
CROSS:=$(TOOLPREFIX)
else
CROSS= ${DEPTH}/../build/gcc-3.4.4-2.16.1/build_mips/bin/mips-linux-uclibc-
endif

CC = $(CROSS)gcc
#zhaoyang1 transplant from 717
ifneq ($(BOARD_TYPE),amcc)
CC = mips-linux-uclibc-gcc
endif
#zhaoyang1 transplant end
#CC = gcc
#zhaoyang1 transplant from 717
ifneq ($(BOARD_TYPE),amcc)
CFLAGS=	-I. -I ${LINUX_INCLUDE4} -I ${LINUX_INCLUDE3} -I ${LINUX_INCLUDE2} -I ${LINUX_INCLUDE1} -I${LINUX_INCLUDE} -I${HALDIR} -I${LINUX_HALDIR} -I${ATHDIR} -g
else
INCS+= -I$(ATHEROSPATH) -I${HAL} -I${HAL}/${OS} -I${ATH_HAL} -I${obj}/${DEPTH} -I${COMMON} -I${ATH_RATE} -I${ATH} -I${WLAN}
CFLAGS= ${INCS} ${COPTS}
endif
#zhaoyang1 transplant end

ALL=	eeprom rfgain ani txpow reg dumpregs

all:	${ALL}

eeprom: eeprom.c
	${CC} -o eeprom ${CFLAGS} eeprom.c
rfgain: rfgain.o diag.o
	${CC} -o rfgain ${CFLAGS} rfgain.o diag.o
ani:    ani.o diag.o
	${CC} -o ani ${CFLAGS} ani.o diag.o
txpow:  txpow.c
	${CC} -o txpow ${CFLAGS} txpow.c
reg:	reg.c
	${CC} -o reg ${CFLAGS} reg.c
dumpregs: dumpregs.o dumpregs_5212.o dumpregs_5416.o dumpregs_9300.o
	${CC} -o dumpregs ${CFLAGS} dumpregs.o \
		dumpregs_5212.o dumpregs_5416.o dumpregs_9300.o

install: ${ALL}
	for i in ${ALL}; do \
		rm -f /usr/local/bin/$$i; \
		cp $$i /usr/local/bin/$$i; \
	done

clean:
	rm -f ${ALL} *.o core
