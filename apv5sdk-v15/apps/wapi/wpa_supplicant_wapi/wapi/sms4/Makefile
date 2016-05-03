
include ../Rules.elinux
LIB=libsms4
OBJSDIR=obj
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(OBJSDIR)/%.o,$(SRCS))
CFLAGS += -Wall -W -O2 
all:$(LIB).a

ifeq ($(TARGET),x86)
        CFLAGS += -DLE 

else ifeq ($(TARGET),ARM)
        CFLAGS += -DLE 

endif


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
	
