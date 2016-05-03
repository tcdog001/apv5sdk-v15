
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: md5evp.h
* description:  the header file for function declaration of   MD5 handler
* 
*
* 
************************************************************************************/


#ifndef MD5EVP_H
#define MD5EVP_H

#include "stdsoap2.h"

#ifdef WITH_OPENSSL
#include <openssl/evp.h>
#endif

enum md5_action { MD5_INIT, MD5_UPDATE, MD5_FINAL, MD5_DELETE };

int md5_handler(struct soap *soap, void **context, enum md5_action action, char *buf, size_t len);

#endif
