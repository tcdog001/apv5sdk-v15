#!/bin/sh

# ====================================================================
#
# --------------------------------------------------------------------

gcc -Wall -o intellon intellon.c
gcc -Wall -o styles styles.c

# ====================================================================
#
# --------------------------------------------------------------------

gcc -Wall -c stylesheet.c
gcc -Wall -c DivisionHeader.c
gcc -Wall -c DivisionOption.c
gcc -Wall -c OptionTopology.c
gcc -Wall -c OptionIdentity.c
gcc -Wall -c OptionSecurity.c
gcc -Wall -c OptionPriority.c
gcc -Wall -c OptionFirmware.c
gcc -Wall -c DivisionDevice.c
gcc -Wall -c DivisionFooter.c

# ====================================================================
#
# --------------------------------------------------------------------

gcc -Wall -c session.c
gcc -Wall -c SessionLoad.c
gcc -Wall -c SessionSave.c
gcc -Wall -c SessionEdit.c
gcc -Wall -c SessionPeek.c
gcc -Wall -c SessionTask.c

