
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: httpda.h
* description:  the header file for the gSOAP HTTP Digest Authentication plugin.Supports both Basic and Digest authentication.
* 
*
* 
************************************************************************************/

#ifndef HTTPDA_H
#define HTTPDA_H

#include "stdsoap2.h"
#include "md5evp.h" /* requires MD5 */
#include "threads.h" /* mutex for multi-threaded server implementations */

#define HTTP_DA_ID "HTTP-DA-1.0" /* plugin identification */

#define HTTP_DA_SESSION_TIMEOUT (600) /* sessions time out after ten minutes */

extern const char http_da_id[];

struct http_da_data
{
  int (*fposthdr)(struct soap*, const char*, const char*);
  int (*fparsehdr)(struct soap*, const char*, const char*);
  int (*fprepareinit)(struct soap*);
  int (*fpreparesend)(struct soap*, const char*, size_t);
  int (*fpreparerecv)(struct soap*, const char*, size_t);
  int (*fdisconnect)(struct soap*);
  void *context;	/* ptr to MD5 context for MD5 handler */
  char digest[16];	/* MD5 entity body digest */
  char *nonce;		/* client/server-side copy of server's nonce value */
  char *opaque;		/* client/server-side copy of server's opaque value */
  char *qop;		/* client/server-side copy of server's qop value(s) */
  char *alg;		/* client-side: server's algorithm value */
  unsigned long nc;	/* client-side: generated nonce count */
  char *ncount;		/* server-side: client's nonce count */
  char *cnonce;		/* server-side: client's nonce */
  char *response;	/* server-side: client's response digest key */
};

struct http_da_session
{
  struct http_da_session *next;
  time_t modified;
  char *realm;
  char *nonce;
  char *opaque;
  unsigned long nc;
};

int http_da(struct soap *soap, struct soap_plugin *p, void *arg);

int http_da_verify_post(struct soap *soap, char *passwd);
int http_da_verify_get(struct soap *soap, char *passwd);

struct http_da_info
{
  char *authrealm;
  char *userid;
  char *passwd;
  char *nonce;
  char *opaque;
  char *qop;
  char *alg;
};

void http_da_save(struct soap *soap, struct http_da_info *info, const char *realm, const char *userid, const char *passwd);
void http_da_restore(struct soap *soap, struct http_da_info *info);
void http_da_release(struct soap *soap, struct http_da_info *info);

#endif
