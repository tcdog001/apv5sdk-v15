
#include $(prefix)/Rules.elinux
CC = $(TOOLPREFIX)gcc
LD = $(TOOLPREFIX)ld
AR = $(TOOLPREFIX)ar
STRIP = $(TOOLPREFIX)strip

LIB=libsms4
OBJSDIR=.
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(OBJSDIR)/%.o,$(SRCS))
CFLAGS += -Wall -W -O2 
all: clean $(LIB).a

$(OBJSDIR)/%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

$(LIB).a: $(OBJS)
	$(AR) rcv $@ $(OBJS) 
#	$(RANLIB) $@
	cp $(LIB).a ../.
	#$(CC) $(CFLAGS)  -shared -o $(LIB).so -Wl -fPIC,-soname=$(LIB).so -Wl,-Bsymbolic -Wl,--whole-archive $(LIB).a -Wl,--no-whole-archive -L. 

clean:
	rm -rf *.o *.a $(OBJSDIR)/*.o 
	
