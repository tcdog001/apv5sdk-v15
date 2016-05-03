#!/bin/sh
# file: int6000-utils.sh

# this script is for developers; it does a remote export or checkout
# and compile of the toolkit from CVS;

# ====================================================================
# environment variables;
# --------------------------------------------------------------------

. /etc/environment

# ====================================================================
# local variables;
# --------------------------------------------------------------------

PACKAGE=int6000-utils-linux

# ====================================================================
# package;
# --------------------------------------------------------------------

echo -n "package [${PACKAGE}]: "; read
if [ ! -z ${REPLY} ]; then
	PACKAGE=${REPLY}
fi

# ====================================================================
#
# --------------------------------------------------------------------

rm -fr ${PACKAGE}       
# cvs export -r HEAD ${PACKAGE}
cvs checkout ${PACKAGE}
if [ -d ${PACKAGE} ]; then
	cd ${PACKAGE}
	make install 1> install.log 2> install.err
	make manuals 1> manuals.log 2> manuals.err
	make network
	cd ..
fi

# ====================================================================
# discard package;
# --------------------------------------------------------------------

echo -n "discard [yes/no]: "; read
if [ "${REPLY}" == "yes" ]; then
 	rm -fr ${PACKAGE}	
fi

# ====================================================================
#  
# --------------------------------------------------------------------

echo done.
