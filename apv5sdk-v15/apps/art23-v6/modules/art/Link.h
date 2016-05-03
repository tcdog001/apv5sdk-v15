

extern void CarrierTransmit(int client);

extern void LinkReceive(int client);

extern void LinkTransmit(int client);

extern int ParseMacAddress(char *text, unsigned char *mac);

extern void LinkTransmitPAPD(int chainNum );
extern void LinkTransmitPAPDWarmUp(int txchain);
extern void ForceLinkReset();

extern void LinkParameterSplice(struct _ParameterList *list);

