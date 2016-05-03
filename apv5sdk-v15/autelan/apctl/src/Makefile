
CC ?= gcc
CFLAGS += -I../includes -Wall
CFLAGS += $(EXTRA_CFLAGS)
LDFLAGS += -L../lib -liconv -lpthread -lm 

OBJS = global.o timerlib.o handle_err.o log.o NetWork.o messages.o APCTL.o 
SRCS = $(OBJS:.o=.c)
APP = apctl

all: clean usage $(APP)  end

usage:
	@echo "########################## Compiling AP module ###########################"
$(APP): $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) $(LDFLAGS) -o $(APP)
end:
	@echo "########################### Compiled AP module ###########################"

.PHONY: clean
clean:
	@echo "############################ clean AP module #############################"
	-rm -rf *.o
	-rm -rf *~
	-rm -rf $(APP)
