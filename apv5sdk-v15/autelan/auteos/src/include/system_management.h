/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_SYSTEM_MANAGEMENT_H__
#define __AUTEOS_SYSTEM_MANAGEMENT_H__
/******************************************************************************/
/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "system_management_glibc.h"
/******************************************************************************/

/*
* section 2:autelan api
*/
extern int      autelan_gethostname (char *name, size_t size);
extern int      autelan_sethostname (const char *name, size_t length);
extern int      autelan_getdomainname (char *name, size_t length);
extern int      autelan_setdomainname (const char *name, size_t length);
extern long int autelan_gethostid (void);
extern int      autelan_sethostid (long int id);
extern FILE *   autelan_setmntent (const char *file, const char *mode);
extern int      autelan_endmntent (FILE *stream);
extern struct mntent *  autelan_getmntent (FILE *stream);
extern struct mntent *  autelan_getmntent_r (FILE *stream, struct mntent *result, char *buffer, int bufsize);
extern int      autelan_addmntent (FILE *stream, const struct mntent *mnt);
extern char *   autelan_hasmntopt (const struct mntent *mnt, const char *opt);
extern int      autelan_mount (const char *special_file, const char *dir, const char *fstype, unsigned long int options, const void *data);
extern int      autelan_umount2 (const char *file, int flags);
extern int      autelan_umount (const char *file);
extern int      autelan_sysctl (int *names, int nlen, void *oldval, size_t *oldlenp, void *newval, size_t newlen);

/*
* section 3:forbidden all glibc api
*/
#define gethostname     autelan_replaced(gethostname)
#define sethostname     autelan_replaced(sethostname)
#define getdomainname   autelan_replaced(getdomainname)
#define setdomainname   autelan_replaced(setdomainname)
#define gethostid       autelan_replaced(gethostid)
#define sethostid       autelan_replaced(sethostid)
#define setfsent        autelan_forbidden(setfsent)
#define endfsent        autelan_forbidden(endfsent)
#define getfsent        autelan_forbidden(getfsent)
#define getfsspec       autelan_replaced(getfsspec)
#define getfsfile       autelan_forbidden(getfsfile)
#define setmntent       autelan_replaced(setmntent)
#define endmntent       autelan_replaced(endmntent)
#define getmntent       autelan_replaced(getmntent)
#define getmntent_r     autelan_replaced(getmntent_r)
#define addmntent       autelan_replaced(addmntent)
#define hasmntopt       autelan_replaced(hasmntopt)
#define mount           autelan_replaced(mount)
#define umount2         autelan_replaced(umount2)
#define umount          autelan_replaced(umount)
#define sysctl          autelan_replaced(sysctl)
/******************************************************************************/
#endif /* __AUTEOS_SYSTEM_MANAGEMENT_H__ */
