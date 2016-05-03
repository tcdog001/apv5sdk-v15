


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "smatch.h"
#include "UserPrint.h"
#include "Field.h"
#include "ParameterSelect.h"
#include "Card.h"
#include "Sticky.h"

static struct _Sticky *_Shead, *_Stail, *_Scurrent;

//
// execute all of the sticky register and field writes
//
int StickyExecute()
{
	struct _Sticky *s;
	int count;

	count=0;
	for(s=_Shead; s!=0; s=s->next)
	{
		UserPrint("sticky %08x[%d,%d] <- %08x\n",s->address,s->high,s->low,s->value);
		MyFieldWrite(s->address,s->low,s->high,s->value);
		count++;
	}
	return count;
}

//
// return the values of the first sticky thing on the list
// return value is 0 if successful, non zero if not
//
extern int StickyHead(unsigned int *address, int *low, int *high, unsigned int *value)
{	
	_Scurrent=_Shead;
	if(_Scurrent!=0)
	{
		*address=_Scurrent->address;
		*low=_Scurrent->low;
		*high=_Scurrent->high;
		*value=_Scurrent->value;
		return 0;
	}
	return -1;
}

//
// return the values of the next sticky thing on the list
// return value is 0 if successful, non zero if not
//
extern int StickyNext(unsigned int *address, int *low, int *high, unsigned int *value)
{
	if(_Scurrent!=0)
	{
		_Scurrent=_Scurrent->next;
	}
	if(_Scurrent!=0)
	{
		*address=_Scurrent->address;
		*low=_Scurrent->low;
		*high=_Scurrent->high;
		*value=_Scurrent->value;
		return 0;
	}
	return -1;
}


//
// clear the list of sticky registers
//
int StickyClear()
{
	struct _Sticky *s, *snext;
	int count=0;

	for(s=_Shead; s!=0; s=snext)
	{
		snext=s->next;
		free(s);
		count++;
	}
	_Shead=0;
	_Stail=0;
	_Scurrent=0;
	return count;
}

//
// find a sticky register on the list
//
struct _Sticky *StickyInternalFind(unsigned int address, int low, int high)
{
	struct _Sticky *s;

	for(s=_Stail; s!=0; s=s->prev)
	{
		if(s->address==address && s->low==low && s->high==high)
		{
			return s;
		}
	}
	return 0;
}

//
// clear one register from the list
//
int StickyInternalClear(unsigned int address, int low, int high)
{
	struct _Sticky *s;

	s=StickyInternalFind(address,low,high);
	if(s!=0)
	{
		//
		// adjust pointers to take this entry off the list
		//
		if(s==_Shead)
		{
			_Shead=s->next;
		}
		if(s==_Stail)
		{
			_Stail=s->prev;
		}
		if(s==_Scurrent)
		{
			_Scurrent=s->next;
		}
		if(s->prev!=0)
		{
			s->prev->next=s->next;
		}
		if(s->next!=0)
		{
			s->next->prev=s->prev;
		}
		free(s);
		return 0;
	}
	return -1;
}

//
// add a sticky register and value to the list
//
int StickyInternalAdd(unsigned int address, int low, int high, unsigned int value)
{
	struct _Sticky *s;

#ifdef OVERWRITESTICKY
	s=StickyInternalFind(address,low,high);
	if(s==0)
#endif
	{
		//
		// make a new structure
		//
		s=(struct _Sticky *)malloc(sizeof(struct _Sticky));
		//
		// tack it on the end of the list
		//
		if(s!=0)
		{
			if(_Shead==0)
			{
				s->next=0;
				s->prev=0;
				_Shead=s;
				_Stail=s;
			}
			else
			{
				s->next=0;
				s->prev=_Stail;
				if(s->prev!=0)
				{
					s->prev->next=s;
				}
				_Stail=s;
			}
		}
		else
		{
			UserPrint("cant save sticky register %x:%d:%d -< %x\n",address,high,low,value);
			return -1;
		}
	}
	if(s!=0)
	{
		s->address=address;
		s->low=low;
		s->high=high;
		s->value=value;
	}
	return 0;
}

//
// add a sticky register and value to the list
//
int StickyInternalChange(unsigned int address, int low, int high, unsigned int value)
{
	struct _Sticky *s;

	s=StickyInternalFind(address,low,high);
	if(s==0)
	{
		//
		// make a new structure
		//
		s=(struct _Sticky *)malloc(sizeof(struct _Sticky));
		//
		// tack it on the end of the list
		//
		if(s!=0)
		{
			if(_Shead==0)
			{
				s->next=0;
				s->prev=0;
				_Shead=s;
				_Stail=s;
			}
			else
			{
				s->next=0;
				s->prev=_Stail;
				if(s->prev!=0)
				{
					s->prev->next=s;
				}
				_Stail=s;
			}
		}
		else
		{
			UserPrint("cant save sticky register %x:%d:%d -< %x\n",address,high,low,value);
			return -1;
		}
	}
	if(s!=0)
	{
		s->address=address;
		s->low=low;
		s->high=high;
		s->value=value;
	}
	return 0;
}

//
// find a sticky register on the list
//
struct _Sticky *StickyRegisterFind(unsigned int address)
{
	return StickyInternalFind(address,0,31);
}

//
// clear one register from the list
//
int StickyRegisterClear(unsigned int address)
{
	return StickyInternalClear(address,0,31);
}

//
// add a sticky register and value to the list
//
int StickyRegisterAdd(unsigned int address, unsigned int value)
{
	return StickyInternalAdd(address,0,31,value);
}

//
// find a sticky register on the list
//
struct _Sticky *StickyFieldFind(char *name)
{
	unsigned int address;
	int low, high;

    if(FieldFind(name, &address, &low, &high))
	{
		return StickyInternalFind(address,low,high);
	}
	return 0;
}

//
// clear one field from the list
//
int StickyFieldClear(char *name)
{
	unsigned int address;
	int low, high;

    if(FieldFind(name, &address, &low, &high))
	{
		return StickyInternalClear(address,low,high);
	}
	return -1;
}

//
// add one sticky field and value to the list
//
int StickyFieldAdd(char *name, unsigned int value)
{
	unsigned int address;
	int low, high;

    if(FieldFind(name, &address, &low, &high))
	{
		return StickyInternalAdd(address,low,high,value);
	}
	return -1;
}



