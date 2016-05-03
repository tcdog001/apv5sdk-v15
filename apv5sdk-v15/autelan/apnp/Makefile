PKG := apnpd 
PKG_BUILD_DIR:=src/src
PKGLIB_BUILD_DIR:=src/apnplib

#command replace
CPDIR := cp -r 
CP := cp -fpR
MKDIR := mkdir -p
MV := mv

.PHONY: all
all: compile install 

.PHONY: compile
compile:
	$(MAKE) -C $(PKGLIB_BUILD_DIR) 
	$(MAKE) -C $(PKG_BUILD_DIR) 

.PHNOY: install
install:
	$(CP) $(PKG_BUILD_DIR)/apnpd $(ROOTFSDIR)/sbin/
	$(CP) $(PKGLIB_BUILD_DIR)/libapnplib.so $(ROOTFSDIR)/lib/
	
.PHONY: clean
clean:
	$(MAKE) -C $(PKG_BUILD_DIR) clean
	$(MAKE) -C $(PKGLIB_BUILD_DIR) clean
	