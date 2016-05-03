#
# Copyright (c) 2004 Atheros Communications, Inc.
# All rights reserved.
#
# $Id: //depot/sw/releases/Aquila_9.2.0/apps/freeRadius/Makefile#1 $
# $File: //depot/sw/releases/Aquila_9.2.0/apps/freeRadius/Makefile $
# $Author: cholden $
# $DateTime: 2010/07/04 08:49:38 $
# $Change: 715594 $
#

TARFILE=freeradius-1.0.0.tar.gz
DIR=freeradius-1.0.0

.untar: 
	@tar zxf ${TARFILE}
	@touch .untar

.config: .untar
	@(cd ${DIR}; ./configure --localstatedir=/var --sysconfdir=/etc)
	@touch .config

build: .config
	@(cd ${DIR}; make)	

all: build
	@echo "Done"

clean:
	@rm -rf ${DIR} *~ .untar .config 
