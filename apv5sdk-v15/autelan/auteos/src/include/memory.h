/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_MEMORY_H__
#define __AUTEOS_MEMORY_H__
/*****************************************************************/
/*---------------------------------------------------------*/
/*-------------------Low-Level I/O functions------------------*/
/*---------------------------------------------------------*/

/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "memory_glibc.h"
/*
* section 2:autelan api
*/
extern void * autelan_malloc (size_t size);
extern void autelan_free (void *ptr);
extern void * autelan_realloc (void *ptr, size_t newsize);
extern void * autelan_calloc (size_t count, size_t eltsize);
extern void * autelan_memalign (size_t boundary, size_t size);


/*
* section 3: Macro define
*/
/*Below are the functions that are forbidden by auteos*/
#define mallopt autelan_forbidden(mallopt)
#define mcheck autelan_forbidden(mcheck)
#define mallinfo autelan_forbidden(mallinfo)


/*Below are the functions that are replaced by auteos*/
#define malloc autelan_replaced(malloc)
#define free autelan_replaced(free)
#define realloc autelan_replaced(realloc)
#define calloc autelan_replaced(calloc)
#define memalign autelan_replaced(memalign)


#endif /* __AUTEOS_PIPE_AND_FIFO_GLIBC_H__ */
