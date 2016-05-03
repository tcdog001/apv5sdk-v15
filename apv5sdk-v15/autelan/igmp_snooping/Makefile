
ROOTFSDIR := /home/shark/builddir/rootfs

PKG_NAME := igmp-snooping
PKG_RELEASEi := 1.0
PKG_BUILD_DIR := src

CAVIUM_CPPFLAGS ?= -I$(OCTEON_ROOT)/tools/mips64-octeon-linux-gnu/sys-root/usr/include
CAVIUM_LDFLAGS ?= -L$(OCTEON_ROOT)/tools/mips64-octeon-linux-gnu/sys-root/usr/lib64 
EXTRA_LDFLAGS := 
CPPFLAGS := $(CAVIUM_CPPFLAGS)
LDFLAGS :=$(CAVIUM_LDFLAGS) -lpthread

TARGET_CROSS := mips64-octeon-linux-gnu-
CC := $(TARGET_CROSS)gcc
LD := $(TARGET_CROSS)ld
AR := $(TARGET_CROSS)ar

CP := cp -fpR

.PHONY: all
all: compile install

.PHONY: compile
compile:
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CPPFLAGS="$(CPPFLAGS)" \
		LDFLAGS="$(LDFLAGS)" \
		EXTRA_CPPFLAGS="$(EXTRA_CPPFLAGS)" \
		EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" \
		CC=$(CC) \
		AR=$(AR)

.PHONY: install
install:
	sudo $(CP) $(PKG_BUILD_DIR)/igmpsnoop $(ROOTFSDIR)/usr/sbin	
	sudo $(CP) $(PKG_BUILD_DIR)/mldsnoop $(ROOTFSDIR)/usr/sbin	
	
.PHONY: clean
clean:
	$(MAKE) -C $(PKG_BUILD_DIR) clean
