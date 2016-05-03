/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "memory_glibc.h"



void * autelan_malloc (size_t size)
{
    return malloc (size);
}

void autelan_free (void *ptr)
{
    return free (ptr);
}

void * autelan_realloc (void *ptr, size_t newsize)
{
    return realloc (ptr, newsize);
}
void * autelan_calloc (size_t count, size_t eltsize)
{
    return calloc (count, eltsize);
}

void * autelan_memalign (size_t boundary, size_t size)
{
    return (void *)memalign (boundary, size);
}

