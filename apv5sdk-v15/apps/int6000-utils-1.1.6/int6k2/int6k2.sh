#!/bin/sh
# file: int6k2/int6k2.sh

# ====================================================================
# compile programs;
# --------------------------------------------------------------------

gcc -Wall -o int6k2 int6k2.c

# ====================================================================
# compile functions;
# --------------------------------------------------------------------

gcc -Wall -c FactoryDefaults2.c
gcc -Wall -c HostActionWait2.c
gcc -Wall -c Identity2.c
gcc -Wall -c MfgString2.c
gcc -Wall -c NetworkInfo2.c
gcc -Wall -c RdModWrPib2.c
gcc -Wall -c ReadMAC2.c
gcc -Wall -c ReadMME2.c
gcc -Wall -c ReadPIB2.c
gcc -Wall -c ResetDevice2.c
gcc -Wall -c SetKey2.c
gcc -Wall -c SetSDRAM2.c
gcc -Wall -c Upgrade2.c
gcc -Wall -c VersionInfo2.c

