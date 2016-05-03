#
# Makefile	Main Makefile for the net-tools Package
#

#CC = gcc
#CFLAGS = -g

ifdef LTEFI
	CFLAGS+=-DLTEFI
endif
ifdef LTEFI_V2
	CFLAGS+=-DLTEFI_V2
endif
TARGET = ./rgps

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
#$(CC) -c $(CFLAGS) $< -o $@

SOURCES = $(wildcard *.c)
	OBJS = $(patsubst %.c,%.o,$(SOURCES))

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lm -lpthread

	chmod a+x $(TARGET)

clean:
	rm -rf *.o rgps

# End of Makefile.
