# Makefile for adb
 
SRCS+= adb.c
SRCS+= adb_client.c
SRCS+= commandline.c
SRCS+= console.c
SRCS+= file_sync_client.c
SRCS+= fdevent.c
SRCS+= get_my_path_linux.c
SRCS+= services.c
SRCS+= sockets.c
SRCS+= transport.c
SRCS+= transport_local.c
SRCS+= transport_usb.c
SRCS+= usb_linux.c
SRCS+= usb_vendors.c
SRCS+= utils.c
#SRCS+= usb_linux_client.c
SRCS+= adb_auth_host.c
#SRCS+= adb_auth_client.c
#SRCS+= sysdeps_win32.c
 
VPATH+= ../libcutils
SRCS+= abort_socket.c
SRCS+= socket_inaddr_any_server.c
SRCS+= socket_local_client.c
SRCS+= socket_local_server.c
SRCS+= socket_loopback_client.c
SRCS+= socket_loopback_server.c
SRCS+= socket_network_client.c
SRCS+= load_file.c 
SRCS+= list.c
 
VPATH+= ../libzipfile
SRCS+= centraldir.c
SRCS+= zipfile.c

#VPATH+= ../libunz
#SRCS+= abort_socket.c
#SRCS+= socket_inaddr_any_server.c
#SRCS+= socket_local_client.c
#SRCS+= socket_local_server.c
#SRCS+= socket_loopback_client.c
#SRCS+= socket_loopback_server.c
#SRCS+= socket_network_client.c
 
CPPFLAGS+= -DADB_HOST=1
CPPFLAGS+= -DHAVE_FORKEXEC=1
CPPFLAGS+= -DHAVE_SYMLINKS
CPPFLAGS+= -DHAVE_TERMIO_H
CPPFLAGS+= -D_GNU_SOURCE
CPPFLAGS+= -D_XOPEN_SOURCE
CPPFLAGS+= -I.
CPPFLAGS+= -I../include
CPPFLAGS+= -I../libzipfile
#CPPFLAGS+= -I../mkbootimg
#CPPFLAGS+= -I../mincrypt
#CPPFLAGS+= -I../../../external/zlib
 
CFLAGS+= -O2 -Wno-unused-parameter
CFLAGS+= -DHAVE_BIG_ENDIAN 
LDFLAGS= 
PWD=$(shell pwd)
SSL_TREE= $(PWD)/../../../openssl/src
SSL_TREE1= $(PWD)/../../../zlib-1.2.3/src
#SSL_TREE= $(PWD)/../openssl/openssl-1.0.0f
#SSL_TREE1= $(PWD)/../zlib/zlib-1.2.5
LIBS= -lrt -lpthread -ldl -L ${SSL_TREE1} -lz -L ${SSL_TREE} -lcrypto -lssl
 
#CROSS= mips-linux-uclibc-
GCC= $(CC)
LD= $(CC)
 
OBJS= $(SRCS:.c=.o)
 
all: adb
 
adb: $(OBJS)
	$(LD) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)
 
clean:
	rm -rf $(OBJS) adb
