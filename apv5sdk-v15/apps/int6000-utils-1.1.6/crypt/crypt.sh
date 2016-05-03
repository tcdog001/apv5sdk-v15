#!/bin/sh
# file: keys/keys.sh

# ====================================================================
# compile tools in this folder;
# --------------------------------------------------------------------

gcc -Wall -o hpavkey hpavkey.c
gcc -Wall -o hpavkeys hpavkeys.c
gcc -Wall -o mac2pw mac2pw.c
gcc -Wall -o mac2pw mac2pwd.c
gcc -Wall -o rkey rkey.c

# ====================================================================
# compile tools in this folder;
# --------------------------------------------------------------------

gcc -Wall -c HPAVKeyDAK.c
gcc -Wall -c HPAVKeyNID.c
gcc -Wall -c HPAVKeyNMK.c
gcc -Wall -c HPAVKeySHA.c
gcc -Wall -c HPAVKeyOut.c
gcc -Wall -c HPAVKeySpec.c
gcc -Wall -c MACPasswords.c
gcc -Wall -c SHA256Block.c
gcc -Wall -c SHA256Fetch.c
gcc -Wall -c SHA256Reset.c
gcc -Wall -c SHA256Write.c
gcc -Wall -c SHA256Print.c
gcc -Wall -c SHA256Ident.c
gcc -Wall -c SHA256Match.c
gcc -Wall -c keys.c

