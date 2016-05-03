#!/bin/sh
# file: hpav/hpav.sh

# ====================================================================
# compile tools in this folder;
# --------------------------------------------------------------------

gcc -Wall -o arpc arpc.c
gcc -Wall -o hpav hpav.c
gcc -Wall -o hpavd hpavd.c

