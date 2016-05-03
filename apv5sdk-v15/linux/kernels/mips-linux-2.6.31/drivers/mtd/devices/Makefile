#
# linux/drivers/devices/Makefile
#

obj-$(CONFIG_MTD_DOC2000)	+= doc2000.o
obj-$(CONFIG_MTD_DOC2001)	+= doc2001.o
obj-$(CONFIG_MTD_DOC2001PLUS)	+= doc2001plus.o
obj-$(CONFIG_MTD_DOCPROBE)	+= docprobe.o
obj-$(CONFIG_MTD_DOCECC)	+= docecc.o
obj-$(CONFIG_MTD_SLRAM)		+= slram.o
obj-$(CONFIG_MTD_PHRAM)		+= phram.o
obj-$(CONFIG_MTD_PMC551)	+= pmc551.o
obj-$(CONFIG_MTD_MS02NV)	+= ms02-nv.o
obj-$(CONFIG_MTD_MTDRAM)	+= mtdram.o
obj-$(CONFIG_MTD_LART)		+= lart.o
obj-$(CONFIG_MTD_BLOCK2MTD)	+= block2mtd.o
obj-$(CONFIG_MTD_DATAFLASH)	+= mtd_dataflash.o
obj-$(CONFIG_MTD_M25P80)	+= m25p80.o
obj-$(CONFIG_MTD_AR7240_SPI_FLASH)	+= ar7240_flash.o
obj-$(CONFIG_MTD_AR7100_SPI_FLASH)  += ar7100_flash.o
ifeq ($(ATH_SST_FLASH), y)
CFLAGS_ar7240_flash.o += -DATH_SST_FLASH=1
endif
obj-$(CONFIG_MTD_ATH_FLASH)	+= ath_flash.o
ifeq ($(ATH_SST_FLASH), y)
CFLAGS_ath_flash.o += -DATH_SST_FLASH=1
endif
#Autelan wangyu add the AQ_TYPE for XH9344 and XJ9344
ifeq ($(AQ_TYPE),XH9344)
EXTRA_CFLAGS += -DXH9344=1
endif
ifeq ($(AQ_TYPE),XJ9344)
EXTRA_CFLAGS += -DXJ9344=1
endif
#Autelan wangyu added end
ifeq ($(AQ_TYPE),AE5000ENT)
EXTRA_CFLAGS += -DAE5000ENT=1
endif
ifeq ($(OS_TYPE),AE1000_OS)
EXTRA_CFLAGS += -DAE1000_OS
endif
#zhangsiyu add the AQ_TYPE for CS_VIC_2000_C
ifeq ($(AQ_TYPE),CS_VIC_2000_C)
EXTRA_CFLAGS += -DCS_VIC_2000_C
endif
#zhangsiyu add end