
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: tools.h
* description: the header file  for the ist (parameter, value) management
* 
*
* 
************************************************************************************/
#ifndef _TOOLS_H
#define _TOOLS_H

#include "const.h"



// add element at end of the list
//
LIST_PARAM_VALUE *add_in_queue(LIST_PARAM_VALUE *list, LIST_PARAM_VALUE *element);

// remove last element of the list
//
LIST_PARAM_VALUE *dequeue(LIST_PARAM_VALUE *list);

// destroy list
//
void free_queue(LIST_PARAM_VALUE *list);

// duplicate list
//
LIST_PARAM_VALUE *duplicate_queue(LIST_PARAM_VALUE *list);

// print the list
//
void print(LIST_PARAM_VALUE *list);


char *strExtractWord(char *_szString, char _sep, char *_szExtractedWord);

#endif // _TOOLS_H
