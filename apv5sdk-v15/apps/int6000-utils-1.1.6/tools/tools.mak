# ====================================================================
# source files;
# --------------------------------------------------------------------

argv.o: argv.c
alert.o: alert.c error.h
assist.o: assist.c symbol.h
b64dump.o: b64dump.c base64.h types.h
bclose.o: bclose.c buffer.h types.h
beof.o: beof.c buffer.h types.h
bgetnexts.o: bgetnexts.c buffer.h types.h
bgeto.o: bgeto.c buffer.h types.h
bgets.o: bgets.c buffer.h types.h
bopen.o: bopen.c buffer.h types.h
buffer.o: buffer.c bclose.c bgets.c bopen.c bgeto.c beof.c bgetnexts.c buffer.h types.h
bytespec.o: bytespec.c memory.h number.h error.h
checkfilename.o: checkfilename.c 
checksum32.o: checksum32.c memory.h
checksum_32.o: checksum_32.c memory.h
dataspec.o: dataspec.c number.h error.h types.h
decdecode.o: decdecode.c memory.h number.h
decencode.o: decencode.c memory.h number.h
efreopen.o: efreopen.c error.h types.h
emalloc.o: emalloc.c error.h types.h
error.o: error.c error.h
expect.o: expect.c symbol.h
fdchecksum32.o: fdchecksum32.c memory.h
fdchecksum_32.o: fdchecksum_32.c memory.h
filepart.o: filepart.c files.h
getoptv.o: getoptv.c getoptv.h putoptv.h version.h
hexdecode.o: hexdecode.c number.h memory.h
hexencode.o: hexencode.c memory.h number.h
hexdump.o: hexdump.c number.h memory.h
hexin.o: hexin.c
hexview.o: hexview.c number.h memory.h
hexwrite.o: hexwrite.c number.h memory.h
memout.o: memout.c memory.h
hexstring.o: hexstring.c number.h
lookup.o: lookup.c symbol.h
reword.o: reword.c symbol.h
memincr.o: memincr.c memory.h
memdecr.o: memdecr.c memory.h
endian.o: memswap.c memory.h
markup.o: markup.c format.h
memswap.o: memswap.c memory.h
output.o: output.c format.h
putoptv.o: putoptv.c getoptv.h putoptv.h
regview.o: regview.c memory.h 
strdecr.o: strdecr.c memory.h
strincr.o: strincr.c memory.h
strfbits.o: strfbits.c format.h flags.h
synonym.o: synonym.c types.h
timer.o: timer.c getoptv.h putoptv.h version.h number.h types.h flags.h 
todigit.o: todigit.c number.h
typename.o: typename.c symbol.h
uintspec.o: uintspec.c number.h
version.o: version.c version.h

# ====================================================================
# program files;
# --------------------------------------------------------------------

fp.o: fp.c getoptv.h files.h error.h SHA256.h number.h types.h
odx.o: odx.c getoptv.h putoptv.h version.h files.h nodes.h 
grab.o: grab.c getoptv.h putoptv.h version.h number.h flags.h 
ds.o: ds.c getoptv.h files.h error.h SHA256.h 
odc.o: odc.c getoptv.h putoptv.h version.h files.h sizes.h 
odd.o: odd.c getoptv.h putoptv.h version.h files.h sizes.h 


# ====================================================================
# header files;
# --------------------------------------------------------------------

base64.h chars.h debug.h endian.h flags.h format.h getoptv.h pstdint.h putoptv.h sizes.h stdint.h types.h version.h:
	touch ../tools/${@}
buffer.h error.h files.h number.h symbol.h: types.h
	touch ../tools/${@}
memory.h: types.h endian.h
	touch ../tools/${@}
markup.h: version.h
	touch ../tools/${@}

