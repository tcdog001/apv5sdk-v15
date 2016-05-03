


extern void ParseErrorInit();



extern int ParseMacAddress(char *buffer, unsigned char *cmac);


extern int ParseUnsigned(int input, char *name, int max, unsigned int *value);

extern int ParseUnsignedList(int input, char *name, unsigned int *value, struct _ParameterList *list);

extern int ParseInteger(int input, char *name, int max, int *value);

extern int ParseIntegerList(int input, char *name, int *value, struct _ParameterList *list);

extern int ParseFloat(int input, char *name, int max, float *value);

extern int ParseFloatList(int input, char *name, float *value, struct _ParameterList *list);

extern int ParseDouble(int input, char *name, int max, double *value);

extern int ParseDoubleList(int input, char *name, double *value, struct _ParameterList *list);

extern int ParseHex(int input, char *name, int max, unsigned int *value);

extern int ParseHexList(int input, char *name, unsigned int *value, struct _ParameterList *list);



extern int ParseStringAndSetRates(int input, char *name, int max, int *value);



extern void ParseParameterReplacement(char * (*f)(char *name, char *buffer, int max));


