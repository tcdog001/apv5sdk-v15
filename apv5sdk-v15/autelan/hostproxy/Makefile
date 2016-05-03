#-------------------------------------------------------------------------------
#  Name:
#    Makefile
#
#  Description:    Makefile for ATparser
#
#   Usage:
#          make
#          make install
#          make clean
#   
#
# Copyright (c) 2010 by Longcheer, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------
#CFLAGS += -fprofile-arcs -ftest-coverage 
#CFLAGS += -std=c99
CFLAGS += -I$(LC_GENERL_INCLUDE_DIR)


LDFLAGS += -L$(LC_LIB_DIR)
LDFLAGS += -rdynamic
LDFLAGS += -lpthread \
			-ldl \
			-llctimer \
			-lcos \
			-lgcov \
			-llcdebug \
			-lsdkmem \
			-lsdkipc \
			-llcnv

exclude_dirs := CVS results


.PHONY: $(TARGET) install clean

#-------------------------------------------------------------------------------
# Main target
#-------------------------------------------------------------------------------
TARGET = hostproxyd
SOURCE = proxy.c
OBJECT = *.o

all: $(TARGET)
prep:

$(TARGET): $(OBJECT)
	@echo "building AT parser..."    
	${CC} $(OBJECT)  $(CFLAGS) -o $(TARGET) $(LDFLAGS)
$(OBJECT): $(SOURCE)	
	${CC} ${CFLAGS} -c $(SOURCE)

clean:
	@echo "cleaning AT parser..."    
	@-rm $(TARGET) $(OBJECT)

install: $(INST_DIRS)
	@echo "install AT parser..."
	cp -f hostproxyd $(INSTALL_ROOT)/usr/sbin/
