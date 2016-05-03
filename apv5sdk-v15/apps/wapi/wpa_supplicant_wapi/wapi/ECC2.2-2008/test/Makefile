PROJECT=ecc_test
X86LIBS=ecc_x86c_1.6
 
GCC=gcc -O2

CFLAGS += -W -Wall 

OBJPATH=./

STRIPFLAGS=--strip-all --remove-section=.note --remove-section=.comment

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(OBJPATH)/%.o,$(SRCS))
STRIP=

all:pc
pc: $(PROJECT)

$(PROJECT):$(OBJS)
	$(GCC) $(CFLAGS) -o $@ $(OBJS) -I../ -L ../ -l$(X86LIBS)
$(OBJPATH)/%.o:%.c
	$(GCC) $(CFLAGS) -I../ -c -o $@ $< 
clean:
	rm -f $(OBJPATH)/*.o $(PROJECT) *.log
	rm -f $(ARMPATH)/*.o $(ARMPRJ)
	rm -f $(ARMTESTPATH)/*.o $(ARMPRJTEST)
