#
# Makefile	Main Makefile for the net-tools Package
#

CC = gcc
CFLAGS = -g

TARGET = ./dhcpsnooping

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
#$(CC) -c $(CFLAGS) $< -o $@

SOURCES = $(wildcard *.c)
	OBJS = $(patsubst %.c,%.o,$(SOURCES))

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lm -lpthread

	chmod a+x $(TARGET)

clean:
	rm -rf *.o dhcpsnooping

# End of Makefile.
