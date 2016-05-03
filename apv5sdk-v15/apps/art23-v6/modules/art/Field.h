


struct _Field
{
	char *registerName;
	char *fieldName;
	unsigned int address;
	char low;
	char high;
};


extern void FieldSelect(struct _Field *field, int nfield);


extern int FieldFindByAddress(unsigned int address, int low, int high, char **registerName, char **fieldName);


extern int FieldFind(char *name, unsigned int *address, int *low, int *high);


extern int FieldWrite(char *name, unsigned int value);


extern int FieldRead(char *name, unsigned int *value);


extern int FieldList(char *pattern, void (*print)(char *name, unsigned int address, int low, int high));
