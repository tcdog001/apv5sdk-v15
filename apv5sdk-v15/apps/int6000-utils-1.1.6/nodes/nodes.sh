#!/bin/sh
# file: nodes/nodes.h

# ====================================================================
#
# --------------------------------------------------------------------

gcc -Wall -c reorder.c
gcc -Wall -c xmlattribute.c
gcc -Wall -c xmlcontent.c
gcc -Wall -c xmledit.c
gcc -Wall -c xmlfree.c
gcc -Wall -c xmlnode.c
gcc -Wall -c xmlopen.c
gcc -Wall -c xmlscan.c
gcc -Wall -c xmltree.c
gcc -Wall -c xmlvalue.c
gcc -Wall -c xmlschema.c

