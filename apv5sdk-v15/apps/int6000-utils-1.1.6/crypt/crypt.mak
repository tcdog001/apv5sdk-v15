# ====================================================================
# files;
# --------------------------------------------------------------------

hpavkey.o: hpavkey.c getoptv.h putoptv.h version.h number.h types.h flags.h error.h HPAVKey.h SHA256.h 
hpavkeys.o: hpavkeys.c getoptv.h putoptv.h version.h number.h types.h flags.h error.h HPAVKey.h SHA256.h 
HPAVKey.o: HPAVKey.c HPAVKeyDAK.c HPAVKeyNMK.c HPAVKeyNID.c HPAVKeySHA.c HPAVKey.h SHA256.h
HPAVKeyDAK.o: HPAVKeyDAK.c HPAVKey.h
HPAVKeyNMK.o: HPAVKeyNMK.c HPAVKey.h
HPAVKeyNID.o: HPAVKeyNID.c HPAVKey.h
HPAVKeySHA.o: HPAVKeySHA.c HPAVKey.h
keys.o: keys.c HPAVKey.h
MACPasswords.o: MACPasswords.c types.h flags.h keys.h
rkey.o: rkey.c getoptv.h putoptv.h memory.h files.h HPAVKey.h SHA256.h 
mac2pw.o: mac2pw.c getoptv.h putoptv.h memory.h number.h types.h flags.h error.h keys.h 
mac2pwd.o: mac2pwd.c getoptv.h putoptv.h memory.h number.h types.h flags.h error.h keys.h 

# ====================================================================
# files;
# --------------------------------------------------------------------

SHA256.o: SHA256.c SHA256Reset.c SHA256Write.c SHA256Block.c SHA256Fetch.c SHA256.h
SHA256Fetch.o: SHA256Fetch.c SHA256.h
SHA256Write.o: SHA256Write.c SHA256.h
SHA256Block.o: SHA256Block.c SHA256.h
SHA256Reset.o: SHA256Reset.c SHA256.h
SHA256Print.o: SHA256Print.c SHA256.h number.h
SHA256Match.o: SHA256Match.c SHA256.h number.h
SHA256Ident.o: SHA256Ident.c SHA256.h 

# ====================================================================
# files;
# --------------------------------------------------------------------

HPAVKey.h:
	touch ../crypt/${@}
keys.h:
	touch ../crypt/${@}
rfc3174.h:
	touch ../crypt/${@}
SHA256.h:
	touch ../crypt/${@}

