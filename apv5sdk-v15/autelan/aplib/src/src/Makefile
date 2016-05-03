LDFLAGS:=-shared -fPIC

all:
	rm -f *.a *.o *.so
	$(CC) -c  conf_file_opt.c -I$(STAGING_DIR)/usr/include/libmxml -I$(STAGING_DIR)/usr/include 
	$(CC) -c  ap_interface.c -I../include -I$(STAGING_DIR)/usr/include/libmxml -I$(STAGING_DIR)/usr/include/ 
	${CC} ${LDFLAGS} -Wall -o libapfunc.so ./*.o -L$(STAGING_DIR)/usr/lib -lmxml
#	$(AR) x $(STAGING_DIR)/usr/lib/libmxml.a
#	$(AR) cr libapfunc.a *.o

clean:
	rm -f *.a *.o *.so
