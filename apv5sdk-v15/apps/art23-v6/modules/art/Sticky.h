


struct _Sticky
{
	unsigned int address;
	unsigned int value;
	int low;
	int high;
	struct _Sticky *next;
	struct _Sticky *prev;
};

//
// execute all of the sticky register and field writes
//
extern int StickyExecute();

//
// clear the list of sticky registers
//
extern int StickyClear();

//
// find a sticky register on the list
//
extern struct _Sticky *StickyInternalFind(unsigned int address, int low, int high);

//
// clear one register from the list
//
extern int StickyInternalClear(unsigned int address, int low, int high);

//
// change the value of a sticky register, create it if it does not exist
//
extern int StickyInternalChange(unsigned int address, int low, int high, unsigned int value);

//
// add a sticky register and value to the list
//
extern int StickyInternalAdd(unsigned int address, int low, int high, unsigned int value);

//
// find a sticky register on the list
//
extern struct _Sticky *StickyRegisterFind(unsigned int address);

//
// clear one register from the list
//
extern int StickyRegisterClear(unsigned int address);

//
// add a sticky register and value to the list
//
extern int StickyRegisterAdd(unsigned int address, unsigned int value);

//
// find a sticky register on the list
//
extern struct _Sticky *StickyFieldFind(char *name);

//
// clear one field from the list
//
extern int StickyFieldClear(char *name);

//
// add one sticky field and value to the list
//
extern int StickyFieldAdd(char *name, unsigned int value);

//
// return the values of the first sticky thing on the list
// return value is 0 if successful, non zero if not
//
extern int StickyHead(unsigned int *address, int *low, int *high, unsigned int *value);

//
// return the values of the next sticky thing on the list
// return value is 0 if successful, non zero if not
//
extern int StickyNext(unsigned int *address, int *low, int *high, unsigned int *value);







