
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: md5evp.c
* description:  implementation for gSOAP HTTP Content-MD5 digest EVP handler for httpmd5 plugin
* 
*
* 
************************************************************************************/




#include "md5evp.h"

int md5_handler(struct soap *soap, void **context, enum md5_action action, char *buf, size_t len)
{ EVP_MD_CTX *ctx;
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int size;
  switch (action)
  { case MD5_INIT:
#ifdef WITH_OPENSSL
      OpenSSL_add_all_digests();
#endif
      if (!*context)
      { *context = (void*)SOAP_MALLOC(soap, sizeof(EVP_MD_CTX));
        EVP_MD_CTX_init((EVP_MD_CTX*)*context);
      }
      ctx = (EVP_MD_CTX*)*context;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- MD5 Init %p\n", ctx));
      EVP_DigestInit(ctx, EVP_md5());
      break;
    case MD5_UPDATE:
      ctx = (EVP_MD_CTX*)*context;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- MD5 Update %p --\n", ctx));
      DBGMSG(TEST, buf, len);
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\n--"));
      EVP_DigestUpdate(ctx, (void*)buf, (unsigned int)len);
      break;
    case MD5_FINAL:
      ctx = (EVP_MD_CTX*)*context;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- MD5 Final %p --\n", ctx));
      EVP_DigestFinal(ctx, (unsigned char*)hash, &size);
      memcpy(buf, hash, 16);
      break;
    case MD5_DELETE:
      ctx = (EVP_MD_CTX*)*context;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- MD5 Delete %p --\n", ctx));
      if (ctx)
      { EVP_MD_CTX_cleanup(ctx);
        SOAP_FREE(soap, ctx);
      }
      *context = NULL;
  }
  return SOAP_OK;
}
