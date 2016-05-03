LDFLAGS=-s

all:
	rm -f xmlconf
	${CC} ${LDFLAGS} -Wall xmlconf.c -o xmlconf ../libxml/lib/libmxml.a -I../libxml/include

clean:
	rm -f xmlconf
