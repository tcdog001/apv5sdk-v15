#!/usr/bin/make -f
# file: int6000-utils-linux/Makefile
# Published 2007 by Intellon Corporation for customer use;

# ====================================================================
# symbols;
# --------------------------------------------------------------------

PACKAGE=$(shell basename "${CURDIR}")
ARCHIVE=../LTK
FTPSITE=/home/ftp/intellon/software/int6000-utils
WINSITE=/home/ftp/windows/toolkit/int6000-utils
FOLDERS=int6k pib crypt nvm node hpav scan coqos ram mme efsu pts cgi VisualStudioNET int6k2 tools
#FOLDERS=int6k pib hpav #pts cgi VisualStudioNET crypt int6k2 tools

# ====================================================================
# targets;
# --------------------------------------------------------------------

.PHONY: all compile compact scripts manuals install uninstall clean fresh 
.PHONY: archive package library windows upgrade release

all compile compact install scripts manuals uninstall clean fresh:
	for folder in ${FOLDERS}; do ${MAKE} -C $${folder} ${@}; done
package: release archive windows 
	install -m 0755 -o root -g root -d ${FTPSITE} 
	install -m 0755 -o root -g root ../${PACKAGE}.tar.gz  ${FTPSITE}
	install -m 0755 -o root -g root ../${PACKAGE}.tar.bz2 ${FTPSITE}
	install -m 0755 -o root -g root README ${FTPSITE}/_README.txt
	crlf -w ${FTPSITE}/_README.txt
	rm -f ${FTPSITE}/_README.txt.[0-9][0-9][0-9]
release:
	rm -fr Eclipse install.* *.log t.c
	rm -fr */*.o */*.[0-9][0-9][0-9]
	rm -fr VisualStudioNET/*/*.txt
	rm -fr VisualStudioNET/*/*.cpp
	rm -fr VisualStudioNET/*/*.vcproj.[0-9][0-9][0-9]
	rm -fr VisualStudioNET/*/Release/*.obj 
	rm -fr VisualStudioNET/*/Release/*.htm 
	rm -fr VisualStudioNET/*/Release/*.pdb 
	rm -fr VisualStudioNET/*/Release/*.idb 
	rm -fr VisualStudioNET/*/Debug
	chmod -fR 0444 */*.c */*.h */*.txt */*.mak */*.xsd */*.xsl */*.xml */*.css */*.html */*.png 
	chmod -fR 0755 */Makefile */*.sh
	chown -fR root:root * 
archive: 
	if [ -d ${ARCHIVE} ]; then tar -vzcf ${ARCHIVE}/${PACKAGE}.tar.gz  -C ${ARCHIVE} ../${PACKAGE}; fi
	tar -vzcf ../${PACKAGE}.tar.gz  -C .. ${PACKAGE} 
	tar -vjcf ../${PACKAGE}.tar.bz2 -C .. ${PACKAGE} 
windows:
	install -m 0755 -o root -g root -d ${WINSITE}
	install -m 0755 -o root -g root VisualStudioNET/*.msi ${WINSITE}
network:
	install -m 0755 -o root -g root int6000-utils.sh ..
