#!/bin/sh
# file: pib/pib.sh

# ====================================================================
# 
# --------------------------------------------------------------------

gcc -Wall -o chkpib chkpib.c
gcc -Wall -o psin psin.c
gcc -Wall -o psout psout.c
gcc -Wall -o pskey pskey.c
gcc -Wall -o pxe pxe.c
gcc -Wall -o modpib modpib.c
gcc -Wall -o mrgpib mrgpib.c
gcc -Wall -o setpib setpib.c
gcc -Wall -o pibrump pibrump.c
gcc -Wall -o pibruin pibruin.c

# ====================================================================
# 
# --------------------------------------------------------------------

gcc -Wall -c pibfile.c
gcc -Wall -c pibpeek.c
gcc -Wall -c pibsize.c
gcc -Wall -c piblock.c

