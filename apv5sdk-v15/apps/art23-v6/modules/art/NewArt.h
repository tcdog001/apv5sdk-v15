
extern int CommandRead();


extern int CommandNext(char *command, int max, int *client);

extern void  UserPrintSocketSet(int client);

extern int SendIt(int client, char *buffer);


extern int SendDebug(int client, char *buffer);


extern int SendDone(int client);


extern int SendOk(int client);


extern int SendOn(int client);


extern int SendOff(int client);


extern int SendError(int client,char *message);


void NewArt(unsigned int instance, int port, char *filename);
