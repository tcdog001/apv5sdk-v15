# file: coqos/coqos.mak

# ====================================================================
# files;
# --------------------------------------------------------------------

coqos_add.o: coqos_add.c getoptv.h memory.h number.h tools.h types.h flags.h files.h error.h int6k.h 
coqos_info.o: coqos_info.c getoptv.h memory.h number.h tools.h types.h flags.h files.h error.h int6k.h 
coqos_man.o: coqos_man.c getoptv.h memory.h number.h tools.h types.h flags.h files.h error.h int6k.h 
coqos_mod.o: coqos_mod.c getoptv.h memory.h number.h tools.h types.h flags.h files.h error.h int6k.h 
coqos_rel.o: coqos_rel.c getoptv.h memory.h number.h tools.h types.h flags.h files.h error.h int6k.h 

# ====================================================================
# files;
# --------------------------------------------------------------------

coqos.h: int6k.h memory.h error.h
	touch ../coqos/${@}

