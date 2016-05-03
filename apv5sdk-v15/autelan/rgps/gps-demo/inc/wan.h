#ifndef SMS_H
#define SMS_H


/*WNC_Sunglow 20111112 add for auto connect.*/
int initNetwork();
void start_network();
int stop_network();

#ifdef DMS_SUPPORT

/*WNC_ZYX 20110712 add for init DMS response functions.*/
int initDMSResp(void);
int initUIMResp(void);

#endif

#endif

