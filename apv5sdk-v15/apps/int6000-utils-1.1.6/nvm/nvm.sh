#!/bin/sh
# file: nvm/nvm.sh

# ====================================================================
# programs;
# --------------------------------------------------------------------

gcc -Wall -o chknvm chknvm.c
gcc -Wall -o chknvm2 chknvm2.c
gcc -Wall -o nvmsplit nvmsplit.c
gcc -Wall -o nvmmerge nvmmerge.c

# ====================================================================
# functions;
# --------------------------------------------------------------------

gcc -Wall -c nvmfile.c
gcc -Wall -c nvmpeek.c
gcc -Wall -c nvmspan.c

