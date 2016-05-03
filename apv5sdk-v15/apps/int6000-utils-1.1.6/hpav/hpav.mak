# file: hpav/hpav.mak

# ====================================================================
# hpav source files;
# --------------------------------------------------------------------

hpav.o: hpav.c getoptv.h putoptv.h memory.h number.h types.h flags.h error.h intellon.h homeplug.h message.h problem.h channel.h mme.h 
hpavd.o: hpavd.c getoptv.h putoptv.h memory.h number.h types.h flags.h error.h intellon.h homeplug.h message.h problem.h channel.h mme.h 
arpc.o: arpc.c getoptv.h putoptv.h memory.h types.h flags.h files.h error.h channel.h 

# ====================================================================
# hpav header files;
# --------------------------------------------------------------------

