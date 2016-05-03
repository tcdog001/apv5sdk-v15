EXECNAME = libapnplib.so
SRCDIRS = .

CFLAGS += -fPIC
CFLAGS += -shared
CFLAGS += -Wall

SRCS = $(wildcard *.c)

OBJS = $(SRCS:.c=.o)

device : $(SRCS)
	$(CC) $(CFLAGS)  $^ -o $(EXECNAME)
	
.PHONY : clean
clean :
	-rm $(EXECNAME)
