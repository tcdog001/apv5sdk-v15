
# Makefile for Ar7240 gigabit MAC and Phys
#



include ${ENETDIR}/Makefile.inc 
ifeq ($(ATH_GMAC_AR934x),1)

obj-m					:= athrs_gmac.o 
athrs_gmac-objs				:=  $(ACCEL-OBJS) $(PHY-OBJS) $(MAC-OBJS) athrs_mac.o athrs_mac_timer.o athrs_flowmac.o

else

obj-$(CONFIG_AG7240)			+= athrs_gmac.o
obj-phy-$(CONFIG_MACH_AR7240)		+= ag7240.o
obj-phy-$(CONFIG_AR7240_S26_PHY)	+= phys/ar7240_s26_phy.o
obj-phy-$(CONFIG_ATHRF1_PHY)            += phys/athrf1_phy.o
obj-phy-$(CONFIG_ATHRS27_PHY)           += phys/athrs27_phy.o
obj-phy-$(CONFIG_ATHRS16_PHY)		+= phys/athrs16_phy.o
obj-phy-$(CONFIG_AR7242_S16_PHY)	+= phys/athrs16_phy.o
obj-phy-$(CONFIG_AG7240_QOS)		+= athrs_qos.o vlan_igmp.o
obj-phy-$(CONFIG_AR7240_S26_VLAN_IGMP)	+= vlan_igmp.o
obj-phy-$(CONFIG_AR7240_S27_VLAN_IGMP)	+= vlan_igmp.o
obj-hw-nat-$(CONFIG_ATHRS_HW_NAT)	+= athrs_nf_nat.o athrs_hw_nat.o
obj-hw-acl-$(CONFIG_ATHRS_HW_ACL)	+= athrs_nf_acl.o athrs_hw_acl.o

obj-phy-$(CONFIG_MACH_AR934x) 	        += ag934x.o
obj-phy-$(CONFIG_MACH_HORNET)   	+= ag7240.o
obj-phy-$(CONFIG_AR8021_PHY)        	+= phys/ar8021_phy.o 
obj-phy-$(CONFIG_ATHRS_VIR_PHY)        	+= phys/athrs_vir_phy.o 

ifdef ATH_GMAC_TXQUEUELEN
EXTRA_CFLAGS = -DATH_GMAC_TXQUEUELEN=$$ATH_GMAC_TXQUEUELEN
else
# refer to ether_setup
EXTRA_CFLAGS = -DATH_GMAC_TXQUEUELEN=1000
endif


athrs_gmac-objs				:= $(obj-phy-y) athrs_mac.o athrs_mac_timer.o  athrs_phy_ctrl.o athrs_gmac_ctrl.o athrs_flowmac.o $(obj-hw-nat-y) $(obj-hw-acl-y)

endif

EXTRA_CFLAGS += -I ${ENETDIR}/include -I ${ENETDIR}/include/phys -I$(KERNELPATH)/arch/mips/include

ifdef FLOWMACDIR
EXTRA_CFLAGS+= -I ${FLOWMACDIR}
endif

ifndef NO_PUSH_BUTTON
export NO_PUSH_BUTTON=1
endif

ifeq ($(strip ${NO_PUSH_BUTTON}), 1)
EXTRA_CFLAGS+= -DNO_PUSH_BUTTON=1
else
EXTRA_CFLAGS+= -DNO_PUSH_BUTTON=0
endif

#wangyu add 2011-05-23
ifeq ($(BOARD_TYPE),db12x)
ifeq ($(AQ_TYPE),AQ2000_X)
EXTRA_CFLAGS += -DAQ2000_X
ifeq ($(SUB_TYPE),LTEFI)
EXTRA_CFLAGS += -DLTEFI
endif
ifeq ($(SUB_TYPE),AK2000_EN2T)
EXTRA_CFLAGS += -DAK2000_EN2T
endif
endif
#zhangsiyu add the AQ_TYPE for CS_VIC_2000_C
ifeq ($(AQ_TYPE),CS_VIC_2000_C)
EXTRA_CFLAGS += -DCS_VIC_2000_C
endif
#zhangsiyu add end
ifeq ($(AQ_TYPE),XH9344)
EXTRA_CFLAGS += -DXH9344=1
else
ifeq ($(AQ_TYPE),XJ9344)
EXTRA_CFLAGS += -DXJ9344=1
else
EXTRA_CFLAGS += -DPC018=1
endif
endif
endif
ifeq ($(SUB_TYPE),AE1000_N2)
EXTRA_CFLAGS += -DAE1000_N2
endif
#wangyu add end

#Begin:Add by zhangisyu for AE5000 transplantation 2012-11-22
ifeq ($(BOARD_TYPE),ap121-2.6.31)
ifeq ($(AQ_TYPE),AE5000ENT)
EXTRA_CFLAGS += -DAE5000ENT
endif
endif
#End:Add by zhangsiyu for AE5000 transplantation 2012-11-22

clean:
	rm -f *.o *.ko 
	rm -f phys/*.o *.ko
ifneq ($(ACCEL-OBJS),)
	rm -f hwaccels/*.o
endif


