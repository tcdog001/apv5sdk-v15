EXECNAME = apnpd
SRCDIRS = .

INCLUDE = ./include/upnp 
LDPATH = ./lib

CFLAGS += -O2 -w -g -Wall
CFLAGS += -pthread
#CFLAGS += -L./lib/ -lxml2
CFLAGS += -DHAVE_CONFIG_H

LDFLAGS += -lupnp
LDFLAGS += -lixml
LDFLAGS += -lthreadutil

LDFLAGS += -lxml2
#LDFLAGS += -lz
LDFLAGS += -lm
SRCS = $(wildcard *.c)

OBJS = $(SRCS:.c=.o)

device : $(OBJS)
	$(CC) $(CFLAGS) $^ -L $(LDPATH) $(LDFLAGS) -ldl -o $(EXECNAME)

.c.o:
	$(CC)  $(CFLAGS) -I $(INCLUDE) -c $< -o $@

%.d : %.c
	set -e; rm -f $@;\
	$(CC) -MM $(CFLAGS) $(INCLUDE) $< > $@.$$$$;\
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$;

.PHONY : clean
clean :
	-rm *.o $(EXECNAME)
