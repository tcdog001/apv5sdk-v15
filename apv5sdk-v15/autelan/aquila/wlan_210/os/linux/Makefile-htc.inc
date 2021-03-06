#
# (C) Copyright 2009 Atheros Communications, Inc.
# All rights reserved.
#

export BUILD_ADF=0

TFTPPATH := /tftpboot/wlan

#
# Include files needed for HTC/WMI/HIF
#
ifeq ($(ATH_SUPPORT_HTC),1)
include $(obj)/$(DEPTH)/htc/HTCCaps.inc

# Include path
INCS = -include $(COMINC)/compat.h -I$(COMINC) -I$(WLAN_TOP)/os/$(OS)/include -I$(WLAN_TOP)/os/$(OS)/src -I$(KERNELPATH)/include
INCS += -I$(WLAN_TOP)/asf/include
INCS += -I$(HTC_INC) -I${ADF_ADF_INC} -I${ADF_LNX_OS_INC} -I${ADF_LNX_NBUF_INC} -I${ADF_LNX_INC}  -I${ADF_LNX_NET_INC} -I${HIF_INC}  
ifeq ($(ATH_SUPPORT_TX99),1)
INCS += -I$(TX99)
EXTRA_CFLAGS+= -DATH_TX99_DIAG=1
endif

#EXTRA_CFLAGS+= -DATH_MAGPIE
EXTRA_CFLAGS+= -DATH_SUPPORT_HTC=1
EXTRA_CFLAGS+= -DATH_CWMIN_WORKAROUND=1
EXTRA_CFLAGS+= -DA_TYPE_IN_ADF
EXTRA_CFLAGS+= -DATH_WINHTC_L   # tempary for ATH_WINHTC sync
EXTRA_CFLAGS+= -DATH_TEMP_I386  # remove jumpstar register callback
# target-host simulate
#EXTRA_CFLAGS+= -DATH_SINGLE_CPU
endif

ifeq ($(strip ${BUS}),USB)
BUSNAME=usb
COPTS += -DATH_USB
EXTRA_CFLAGS += -DMAGPIE_HIF_USB

# tempary for usbdrv.c
INCS += -I$(obj)/$(DEPTH)/htc/hif/usb
endif


ifeq ($(strip ${BUS}),GMAC)
BUSNAME=gmac
COPTS += -DATH_USB -DATH_GMAC -DUMAC_SUPPORT_NAWDS=1
EXTRA_CFLAGS += -DMAGPIE_HIF_GMAC  -DHTC_HOST_CREDIT_DIST=1 -DATH_HTC_TX_SCHED=1 -DENCAP_OFFLOAD -DATH_HTC_SG_SUPPORT=1 -DWMI_RETRY
COPTS :=$(patsubst -DATH_AMSDU=1,, $(COPTS))
endif

ifeq ($(ENABLE_MTU_LIMIT),1)
EXTRA_CFLAGS += -DLIMIT_MTU_SIZE
endif    


ifeq ($(strip ${BUS}),PCI)
BUSNAME=pci
COPTS += -DATH_USB
EXTRA_CFLAGS += -DMAGPIE_HIF_PCI
endif
