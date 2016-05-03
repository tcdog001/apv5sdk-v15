/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "system_management_glibc.h"

int autelan_gethostname (char *name, size_t size)
{
    return gethostname(name, size);
}

int autelan_sethostname (const char *name, size_t length)
{
    return sethostname(name, length);
}

int autelan_getdomainname (char *name, size_t length)
{
    return getdomainname(name, length);
}

int autelan_setdomainname (const char *name, size_t length)
{
    return setdomainname(name, length);
}

long int autelan_gethostid (void)
{
    return gethostid();
}

int autelan_sethostid (long int id)
{
    return sethostid(id);
}


FILE * autelan_setmntent (const char *file, const char *mode)
{
    return setmntent(file, mode);
}

int autelan_endmntent (FILE *stream)
{
    return endmntent(stream);
}

struct mntent * autelan_getmntent (FILE *stream)
{
    return getmntent(stream);
}

struct mntent * autelan_getmntent_r (FILE *stream, struct mntent *result, char *buffer, int bufsize)
{
    return getmntent_r(stream, result, buffer, bufsize);
}

int autelan_addmntent (FILE *stream, const struct mntent *mnt)
{
    return addmntent(stream, mnt);
}

char * autelan_hasmntopt (const struct mntent *mnt, const char *opt)
{
    return hasmntopt(mnt, opt);
}

int autelan_mount (const char *special_file, const char *dir, const char *fstype, unsigned long int options, const void *data)
{
    return mount(special_file, dir, fstype, options, data);
}

int autelan_umount2 (const char *file, int flags)
{
    return umount2(file, flags);
}

int autelan_umount (const char *file)
{
    return umount(file);
}

int autelan_sysctl (int *names, int nlen, void *oldval, size_t *oldlenp, void *newval, size_t newlen)
{
    return sysctl(names, nlen, oldval, oldlenp, newval, newlen);
}

