#!/bin/sh
# file: nda/nda.sh

# ====================================================================
# programs;
# --------------------------------------------------------------------

gcc -Wall -Wextra -Wno-unused-parameter -o ampinit ampinit.c
gcc -Wall -Wextra -Wno-unused-parameter -o plcinit plcinit.c
gcc -Wall -Wextra -Wno-unused-parameter -o plcecho plcecho.c
gcc -Wall -Wextra -Wno-unused-parameter -o plcmcinfo plcmcinfo.c
gcc -Wall -Wextra -Wno-unused-parameter -o plcmod plcmod.c
gcc -Wall -Wextra -Wno-unused-parameter -o psgen psgen.c -lm
gcc -Wall -Wextra -Wno-unused-parameter -o int6kecho int6kecho.c 
gcc -Wall -Wextra -Wno-unused-parameter -o int6kp int6kp.c 
gcc -Wall -Wextra -Wno-unused-parameter -o int6kprobe int6kprobe.c 

# ====================================================================
# functions; 
# --------------------------------------------------------------------

gcc -Wall -Wextra -Wno-unused-parameter -c AccessLevel.c
gcc -Wall -Wextra -Wno-unused-parameter -c AccessLevelPTS.c
gcc -Wall -Wextra -Wno-unused-parameter -c EraseFlashMemory.c
gcc -Wall -Wextra -Wno-unused-parameter -c EraseFlashMemory1.c
gcc -Wall -Wextra -Wno-unused-parameter -c EraseFlashMemory2.c
gcc -Wall -Wextra -Wno-unused-parameter -c EraseFlashSector.c
gcc -Wall -Wextra -Wno-unused-parameter -c FlashPTS.c
gcc -Wall -Wextra -Wno-unused-parameter -c Loopback.c
gcc -Wall -Wextra -Wno-unused-parameter -c ModuleErase.c
gcc -Wall -Wextra -Wno-unused-parameter -c Monitor.c
gcc -Wall -Wextra -Wno-unused-parameter -c MulticastInfo1.c
gcc -Wall -Wextra -Wno-unused-parameter -c MulticastInfo2.c
gcc -Wall -Wextra -Wno-unused-parameter -c NetworkProbe.c 
gcc -Wall -Wextra -Wno-unused-parameter -c psgen_util.c
gcc -Wall -Wextra -Wno-unused-parameter -c ReadFlashMemory.c
gcc -Wall -Wextra -Wno-unused-parameter -c ReadFlashMemory1.c
gcc -Wall -Wextra -Wno-unused-parameter -c ReadFlashMemory2.c
gcc -Wall -Wextra -Wno-unused-parameter -c ReadFlashFirmware.c
gcc -Wall -Wextra -Wno-unused-parameter -c ReadFlashParameters.c
gcc -Wall -Wextra -Wno-unused-parameter -c RandomAddress.c
gcc -Wall -Wextra -Wno-unused-parameter -c Sniffer.c

# ====================================================================
# cleanse; 
# --------------------------------------------------------------------

rm -f *.o

