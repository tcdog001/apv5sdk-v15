
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename:  tools.c 
* description:  implementation for Tree functions on gSoap DOM  ,for parsing, adding node(s) and attribute(s) in DOM tree
* 
*
* 
************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tools.h"


/***************************************************************************/
/*** add element at end of the list                                      ***/
/***************************************************************************/

LIST_PARAM_VALUE *add_in_queue(LIST_PARAM_VALUE *list, LIST_PARAM_VALUE *element)
{
    LIST_PARAM_VALUE *ptr;

  // seek at end of the list
  ptr = list;
  if (ptr == NULL) return NULL;

  while(ptr->next != NULL)
  {
    ptr = ptr->next;
  }

  // add element at end of the list
  if (element != NULL)
  {
    element->next = NULL;
    ptr->next = element;
  }

  return ptr;
}


/***************************************************************************/
/*** remove last element of the list                                     ***/
/*** return removed element                                              ***/
/***************************************************************************/

LIST_PARAM_VALUE *dequeue(LIST_PARAM_VALUE *list)
{
	LIST_PARAM_VALUE *ptr;          // pointer on current element
	LIST_PARAM_VALUE *ptrPrevious;  // pointer on previous element

  // seek at end of the list
  ptr = list;
  if (ptr == NULL) return NULL;

  ptrPrevious = ptr;
  while(ptr->next != NULL)
  {
    ptrPrevious = ptr;
    ptr = ptr->next;
  }

  // remove last element from list
  ptrPrevious->next = NULL;

  return ptr;
}


/***************************************************************************/
/*** free the list                                                      ***/
/***************************************************************************/
void free_queue(LIST_PARAM_VALUE *list)
{
LIST_PARAM_VALUE *elt=NULL;

  while (elt != list)
  {
    elt = dequeue(list);
    if (elt != list)
    {
      free(elt);
      elt = NULL;
    }
  }

  if (elt == list)
  {
    free(list); // remove first element
    list = NULL;
  }
}


/***************************************************************************/
/*** duplicate the list                                                  ***/
/***************************************************************************/
LIST_PARAM_VALUE *duplicate_queue(LIST_PARAM_VALUE *list)
{
LIST_PARAM_VALUE *list_dest=NULL, *dest;
LIST_PARAM_VALUE *src=list;

  if (src == NULL) return NULL;

  // allocate first element of the new list
  dest = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
  list_dest = dest;
  memset(dest, 0x00, sizeof(LIST_PARAM_VALUE));

  strcpy(dest->szValue, src->szValue);
  strcpy(dest->szParameter, src->szParameter);

  while(src->next != NULL)
  {
    dest->next = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
    if (dest->next != NULL)
    {
      memset(dest->next, 0x00, sizeof(LIST_PARAM_VALUE));
      dest = dest->next;
      src = src->next;
      strcpy(dest->szValue, src->szValue);
      strcpy(dest->szParameter, src->szParameter);
    }
  }
  dest->next = NULL;

  return list_dest;
}


/***************************************************************************/
/*** print the list                                                      ***/
/***************************************************************************/

void print(LIST_PARAM_VALUE *list)
{
LIST_PARAM_VALUE *ptr;          // pointer on current element

  dprintf(LOG_LEVEL3, "LIST\n");
  ptr = list;
  if (ptr == NULL) return;

  dprintf(LOG_LEVEL3, "parameter: %s\t\tvalue: %s\n", ptr->szParameter, ptr->szValue);
  while(ptr->next != NULL)
  {
    ptr = ptr->next;
    dprintf(LOG_LEVEL3, "parameter: %s\t\tvalue: %s\n", ptr->szParameter, ptr->szValue);
  }
}



/***************************************************************************/
/*** extract word from string                                            ***/
/*** parameter values:                                                   ***/
/*** source string                                                       ***/
/*** separator                                                           ***/
/*** first word before separator (this string MUST be allocated)         ***/
/*** return value:                                                       ***/
/*** rest of the string                                                  ***/
/*** NULL else                                                           ***/
/***                                                                     ***/
/*** example:                                                            ***/
/*** char szSubString[ARGSIZE];                                          ***/
/***                                                                     ***/
/*** memset(szSubString, 0x00, ARGSIZE);                                 ***/
/*** ptr = strExtractWord((char *)_name, '.', szSubString);              ***/
/*** if (ptr != NULL)                                                    ***/
/*** {                                                                   ***/
/***   fprintf(stderr, "rest of string :%s\n", ptr);                     ***/
/*** }                                                                   ***/
/***************************************************************************/

char *strExtractWord(char *_szString, char _sep, char *_szExtractedWord)
{
char *ptr = _szString;
char *_szPartString=NULL;

  dprintf(LOG_LEVEL3, "_szString: %s\n", _szString);

  while ((*ptr != _sep) && (*ptr != '\0'))
  {
    ptr++;
  }

  if (*ptr != '\0')
  {
    _szPartString = ptr+1;
    strncpy(_szExtractedWord, _szString, ptr-_szString); //don't copy separator char
    dprintf(LOG_LEVEL3, "_szExtractedWord: %s\n", _szExtractedWord);
  }
  else
  {
    strcpy(_szExtractedWord, _szString);
    dprintf(LOG_LEVEL3, "_szExtractedWord: %s\n", _szExtractedWord);
  }

  return _szPartString;
}
