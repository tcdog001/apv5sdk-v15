
include ../Rules.elinux
LIB=libiwnwai_asue
OBJSDIR=obj
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(OBJSDIR)/%.o,$(SRCS))
CFLAGS += -Wall -O2 
all:$(LIB).a

$(OBJSDIR)/%.o : %.c
	mkdir -p $(OBJSDIR)
	$(CC) $(CFLAGS) -o $@ -c $< 

$(LIB).a: $(OBJS)
	$(AR) rcv $@ $(OBJS) 
	$(RANLIB) $@
	mkdir -p ../LIB/
	cp $(LIB).a ../LIB/
	#$(CC) $(CFLAGS)  -shared -o $(LIB).so -Wl -fPIC,-soname=$(LIB).so -Wl,-Bsymbolic -Wl,--whole-archive $(LIB).a -Wl,--no-whole-archive -L. 

clean:
	rm -rf *.o *.a $(OBJSDIR)/*.o $(ARMPATH)/*.o $(ARMPATH)/*.o *.a *.so
	
