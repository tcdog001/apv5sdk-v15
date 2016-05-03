
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename:  downloadmanager.h
* description:  the header file for the declaration of download function
* 
*
* 
************************************************************************************/



#ifndef _DOWNLOADMANAGER_H
#define _DOWNLOADMANAGER_H

double download_funct(const char *_url, 
                    const char *_targetfilename, 
                    const char *_user,
                    const char *_passwd,
                    const char *_proxyhost,
                    const unsigned int _proxyport,
                    const char *_proxyuserid, 
                    const char *_proxyuserpwd);

#endif // _DOWNLOADMANAGER_H
