#ifneq ($(strip $(TOOLPREFIX)),)
ifeq ($(BOARD_TYPE),amcc)
CROSS:=powerpc-apm-linux-gnu-
else ifeq ($(BOARD_TYPE),armada370)
CROSS:=arm-marvell-linux-gnueabi-
else
CROSS:=mips-linux-
endif
#endif
CFLAGS:=-Wall -Wno-unused -g -I.
CLIBS:= -L../../auteos/src/src/ -lauteos
LDFLAG:=
TARGET:=pro_ctl_util
CC:=$(CROSS)gcc
RM:=rm -rf 


ifdef APM82181
	CFLAGS+=-DAPM82181
endif	
ifdef ARMADA370
	CFLAGS+=-DARMADA370
endif
ifdef FLASHSIZE_8M
	CFLAGS+=-DFLASHSIZE_8M
endif	
ifdef AQ2000N
	CFLAGS+=-DAQ2000N
endif	
ifdef AQ2000SNH
	CFLAGS+=-DAQ2000SNH
endif
ifdef PC018
	CFLAGS+=-DPC018
endif
ifdef WNAP210
	CFLAGS+=-DWNAP210	
endif
ifdef WNAP350
	CFLAGS+=-DWNAP350	
endif
ifdef WNAP550
	CFLAGS+=-DWNAP550
endif
ifdef XJAP93
	CFLAGS+=-DXJAP93	
endif
ifdef XJAP93_02
	CFLAGS+=-DXJAP93_02	
endif
ifdef AP2400
	CFLAGS+=-DAP2400
endif	
ifdef XJPB44_02
	CFLAGS+=-DXJPB44_02
endif
ifdef TDPB44_02
	CFLAGS+=-DTDPB44_02
endif
ifdef XHAP93
	CFLAGS+=-DXHAP93
endif
ifdef XHPB44
	CFLAGS+=-DXHPB44
endif
ifdef XH9344
	CFLAGS+=-DXH9344
endif
ifdef XJ9344
	CFLAGS+=-DXJ9344
endif
ifdef AQ2000_X
	CFLAGS+=-DAQ2000_X
endif
#add start for lte-fi by liuhj//20130709
ifdef LTEFI
	CFLAGS+=-DLTEFI
endif
ifdef LTEFI_V2
	CFLAGS+=-DLTEFI_V2
endif
#add end for lte-fi by liuhj  //20130709
#wangyu add for ae5000
ifdef AE5000ENT
	CFLAGS+=-DAE5000ENT
endif
#wangyu add end
all:clean
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LFLAGS) $(CLIBS) -o pro_ctl_util pro_ctl_util_ap83.c
	$(CC) $(CFLAGS) $(LFLAGS) -o crc crc.c
clean:
	$(RM) $(TARGET)

