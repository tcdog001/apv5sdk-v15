# ====================================================================
# files;
# --------------------------------------------------------------------

anynic.o: anynic.c ether.h error.h
channel.o: channel.c channel.h
closechannel.o: closechannel.c channel.h types.h
getifname.o: getifname.c ether.h error.h types.h
gethwaddr.o: gethwaddr.c error.h ether.h types.h
openchannel.o: openchannel.c channel.h error.h ether.h types.h
pcapdevs.o: pcapdevs.c getoptv.h putoptv.h version.h flags.h error.h types.h memory.h ether.h
readpacket.o: readpacket.c channel.h memory.h error.h flags.h types.h
sendpacket.o: sendpacket.c channel.h memory.h flags.h types.h

