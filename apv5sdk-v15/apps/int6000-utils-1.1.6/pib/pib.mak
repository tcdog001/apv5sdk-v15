# ====================================================================
# programs; 
# --------------------------------------------------------------------

chkpib.o: chkpib.c getoptv.h putoptv.h version.h flags.h error.h int6k.h pib.h 
cmppib.o: cmppib.c getoptv.h putoptv.h version.h flags.h error.h int6k.h pib.h
getpib.o: getpib.c getoptv.h memory.h number.h error.h types.h flags.h files.h pib.h 
modpib.o: modpib.c getoptv.h putoptv.h version.h flags.h error.h pib.h HPAVKey.h 
mrgpib.o: mrgpib.c getoptv.h putoptv.h version.h flags.h error.h files.h pib.h 
pibrump.o: pibrump.c getoptv.h putoptv.h version.h flags.h error.h pib.h
pibruin.o: pibruin.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h rules.h pib.h 
psin.o: psin.c getoptv.h putoptv.h version.h number.h chars.h types.h flags.h error.h files.h HPAVKey.h pib.h 
pskey.o: pskey.c getoptv.h putoptv.h version.h number.h types.h flags.h error.h files.h HPAVKey.h pib.h 
psgraph.o: psgraph.c getoptv.h putoptv.h version.h number.h files.h 
psnotch.o: psnotch.c getoptv.h putoptv.h version.h error.h flags.h chars.h number.h 
psout.o: psout.c getoptv.h putoptv.h version.h number.h chars.h types.h flags.h error.h files.h pib.h 
pxe.o: pxe.c getoptv.h files.h number.h memory.h error.h nodes.h HPAVKey.h pib.h 
setpib.o: setpib.c getoptv.h putoptv.h memory.h error.h types.h flags.h files.h pib.h 

#====================================================================
# functions;
# --------------------------------------------------------------------

pibcodes.o: pibcodes.c pib.h
pibedit.o: pibedit.c int6k.h memory.h flags.h pib.h
pibfile.o: pibfile.c int6k.h memory.h flags.h pib.h
pibfileA.o: pibfileA.c int6k.h memory.h flags.h pib.h
pibpeek.o: pibpeek.c memory.h number.h HPAVKey.h pib.h
pibsize.o: pibsize.c pib.h
piblock.o: piblock.c pib.h error.h files.h

# ====================================================================
# pib header files;
# --------------------------------------------------------------------

pib.h: types.h memory.h HPAVKey.h endian.h
	touch ../pib/${@}

