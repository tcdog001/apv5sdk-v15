/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "pipe_and_fifo_glibc.h"

int autelan_pipe (int filedes[2])
{
    return pipe(filedes);
}

FILE * autelan_popen (const char *command, const char *mode)
{
    return popen(command, mode);
}

int autelan_pclose (FILE *stream)
{
    return pclose(stream);
}

int autelan_mkfifo (const char *filename, __mode_t mode)
{
    return mkfifo(filename, mode);
}

