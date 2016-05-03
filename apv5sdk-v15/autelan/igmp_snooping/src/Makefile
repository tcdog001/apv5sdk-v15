#MAKEFILE for igmp_snoop

TARGETS := igmp_snoop

CC = gcc
CFLAGS = -g -c 
INCLUDES = 

target_OBJECTS = igmp_snoop_main.o igmp_snoop_com.o igmp_snoop_hw.o igmp_snoop_cli.o
LIBS = -l pthread
COMPILE = $(CC) $(CFLAGS)
LINK = $(COMPILE) -o $@

.SUFFIXES:
.SUFFIXES: .c .o 

all: $(TARGETS)

igmp_snoop:$(target_OBJECTS) smallkit 
	@rm -f $(TARGETS)
#	$(LINK) $(target_OBJECTS) $(LIBS)
	$(CC) -o $(TARGETS) $(CPPFLAGS) $(EXTRA_CPPFLAGS) $(target_OBJECTS) $(LDFLAGS) $(EXTRA_LDFLAGS) 
	@rm -f *.o
	echo "Compile complete!"
	
smallkit:   igmp_tmp.c
	$(CC) -o igmpsnoop igmp_tmp.c
	$(CC) -o mldsnoop mld_tmp.c

.c.o:
	source='$<' object='$@' \
	$(COMPILE) -c `test -f '$<' || echo '$(srcdir)/'`$<
	
clean:
	-rm -f *.a *.o
	-rm -f $(TARGETS)
	-rm -f igmpsnoop
	-rm -f mldsnoop


.PHONY: all clean
