#====================================================================
# nvm source files;
# --------------------------------------------------------------------

nvmfile.o: nvmfile.c nvm.h memory.h error.h
nvmpeek.o: nvmpeek.c nvm.h format.h memory.h
nvmspan.o: nvmspan.c nvm.h memory.h error.h 
chknvm.o: chknvm.c getoptv.h putoptv.h flags.h error.h int6k.h nvm.h 
chknvm2.o: chknvm2.c getoptv.h memory.h flags.h error.h files.h sdram.h nvm.h 
nvmsplit.o: nvmsplit.c getoptv.h putoptv.h version.h memory.h flags.h error.h files.h nvm.h 
nvmmerge.o: nvmmerge.c getoptv.h putoptv.h version.h memory.h flags.h error.h files.h nvm.h 

# ====================================================================
# nvm header files;
# --------------------------------------------------------------------

nvm.h: types.h
	touch ../nvm/${@}

