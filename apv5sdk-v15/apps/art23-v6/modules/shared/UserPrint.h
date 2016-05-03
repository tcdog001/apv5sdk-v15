#include <stdio.h>

#include "Socket.h"

//
// print a message for the user
//
extern void UserPrintIt(char *buffer);

//
// print a message for the user
//
extern void UserPrint(char *format, ...);


//
// print user messages to the console
//
extern void UserPrintConsole(int onoff);

//
// print user messages to the specified file.
// the file must be opened before calling this function.
// if file==0, messages are no longer added to the file
//
extern void UserPrintFile(FILE *file);

//
// print user messages to the specified socket.
// the socket must be opened before calling this function.
// if socket==0, messages are no longer added to the file
//
extern void UserPrintSocket(struct _Socket *socket);

//
// print user messages with an application defined function
//
extern void UserPrintFunction(void (*print)(char *buffer));

