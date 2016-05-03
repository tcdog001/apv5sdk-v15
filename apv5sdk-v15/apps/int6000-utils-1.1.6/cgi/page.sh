#!/bin/sh
gcc -o ${1} ${1}.c
${1} > ${1}.html
crlf -w ${1}.html
