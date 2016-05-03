#!/bin/sh
# file: ram/ram.sh 

# ====================================================================
# programs;
# --------------------------------------------------------------------

gcc -Wall -o config2cfg config2cfg.c 
gcc -Wall -o sdram sdram.c

# ====================================================================
# functions;
# --------------------------------------------------------------------

gcc -Wall -c nvrampeek.c 
gcc -Wall -c nvram.c 
gcc -Wall -c sdramfile.c 
gcc -Wall -c sdrampeek.c 
gcc -Wall -c sdramtext.c 
gcc -Wall -c nvram.c 
gcc -Wall -c nvrampeek.c 
gcc -Wall -c sdramfile.c 
gcc -Wall -c sdramfileA.c 
gcc -Wall -c sdrampeek.c 

