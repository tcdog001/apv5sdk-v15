/** @file cert.c
  * @brief This file contains functions for cert.
 *
 *  Copyright (C) 2001-2008, Iwncomm Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cert_info.h"
#include "auth.h"
#include "proc.h"
#include "debug.h"

/* local funciton declare */

static const WOID gWOID[WAPI_OID_NUMBER] = 
{
    /* OIDName          OIDLen      ParLen      OID     Parameter*/
    {WAPI_ECDSA_OID, 8, 11, {0x2A, 0x81, 0x1C, 0xD7, 0x63, 0x01, 0x01, 0x01}, {0x06, 0x09, 0x2A, 0x81, 0x1C, 0xD7, 0x63, 0x01, 0x01, 0x02, 0x01}}
};

static const WOID gPubKeyOID[WAPI_OID_NUMBER] = 
{
    {ECDSA_ECDH_OID, 7, 11, {0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01}, {0x06, 0x09, 0x2A, 0x81, 0x1C, 0xD7, 0x63, 0x01, 0x01, 0x02, 0x01}}
};

static int ParseLength(unsigned char **pBuf, unsigned char *pMax, unsigned int *pLen);
static int ParseSequence(unsigned char **pBuffer, unsigned char *pMax, int *pClass, int *pTag, unsigned int *pLength, unsigned char *pbIsConstruct);
static int ParseOID(unsigned char **pBuffer, unsigned char *pMax, unsigned char *pszString, unsigned int *pStrLen, unsigned int *pParLen);
static int ParseString(unsigned char **pBuffer, unsigned char *pMax, unsigned char *pszString, unsigned int *pStrLen);
static int ParseName(unsigned char **pBuffer, unsigned char *pMax, unsigned char **pszString, unsigned int *pStrLen);
static int ParseValidity(unsigned char **pBuffer, unsigned char *pMax, pov_x_t* pValid);
static int Parse_certfile(const char *cert_file_name, struct cert_obj_st_t *cert_obj);
static int Parse_private_key(const void *buffer, int bufferlen, struct key_bin_t *keypair);
static int ParsePubKey(unsigned char **pBuffer, unsigned char *pMax, unsigned char **pPubKey, unsigned int *pLen);
static unsigned char hlpCheckOIDAndParam(unsigned char *pOID, unsigned int dwOIDLen, unsigned char* pParam, unsigned int dwParamLen, unsigned char bIsPubKey);
static int Base64Dec(unsigned char *buf,const unsigned char*text,int size);
static int Base64_strip(const unsigned char *in, unsigned int inlen, unsigned char *out );
static unsigned char GetBase64Value(unsigned char ch);
static int getchartype_base64(unsigned char b);
static const unsigned char* findmark_mem(const unsigned char* src, int lsrc, const  char* mark, int lmark);
static unsigned char *get_pos_from_certfile(const unsigned char *src, int len, 
		const  char *start_strs, const  char *end_stars,
		unsigned char **ppos, int *plen);
static  int x509_read_cert_file(const char *cert_file_name, item_data_t *cert_file_buffer);
static int x509_get_pubkey(struct cert_bin_t *cert_st, item_data_t *publickey);
static int x509_get_subject_der(struct cert_bin_t *cert_st, item_data_t *subject);
static int x509_get_issuer_der(struct cert_bin_t *cert_st, item_data_t *issuer);
static int x509_get_serial_der(struct cert_bin_t *cert_st, item_data_t *serial);
static int x509_get_cert_sign(struct cert_bin_t *cert_st, unsigned char *out);
static int x509_get_cert_sign_inlen(struct cert_bin_t *cert_st);


static int ParseLength(unsigned char ** pBuffer, unsigned char* pMax, unsigned int* pLen)
{
    unsigned char *p = *pBuffer;
    unsigned int l = 0, n = 0;
    if (*p & 0x80)
    {
        n = *p++ & 0x7f;
        while (n-- && p < pMax)
        {
            l <<= 8;
            l |= *p++;
        }
    }
    else
    {
        l = *p++;
    }
    *pLen = l;
    *pBuffer = p;
    return 0;
}

static int ParseSequence(unsigned char **pBuffer, unsigned char *pMax, int *pClass, int *pTag, unsigned int *pLength, unsigned char *pbIsConstruct)
{
    unsigned char *p = *pBuffer;
    int c, t;
    unsigned char bCt;
    unsigned int tl = 0;
    c = *p & V_ASN1_PRIVATE;
    t = *p & V_ASN1_PRIMITIVE_TAG;
    bCt = *p & V_ASN1_CONSTRUCTED;
    p++;
	if (t == V_ASN1_PRIMITIVE_TAG) 
    {
        t = 0;
        do
        {
            t <<= 7;
            t |= *p & 0x7f;
        } while( (*(p++) & 0x80) && p < pMax);
    }
    if (ParseLength(&p, pMax, &tl))
    {
        return 1;
    }
    *pBuffer = p;
    if (pClass)
    {
        *pClass = c;
    }
    if (pTag)
    {
        *pTag = t;
    }
    if (pbIsConstruct)
    {
        *pbIsConstruct = bCt;
    }
    if (pLength)
    {
        *pLength = tl;
    }
    return 0;
}

static int ParseOID(unsigned char **pBuffer, unsigned char *pMax, unsigned char *pszString, unsigned int *pStrLen, unsigned int *pParLen)
{
    unsigned char *p = *pBuffer, *pbak;
    unsigned int len;
    if (*p != V_ASN1_OBJECT)
    {
        return 1;
    }
    p++;
	if (ParseLength(&p, pMax, &len))
    {
        return 1;
    }
    *pStrLen = len;
    pbak = p;
    if (pParLen)
    {
        p += len;
        if (*p == V_ASN1_NULL)
        {
            *pParLen = 2;
        }
        else
        {
            p++;
            if (ParseLength(&p, pMax, pParLen))
            {
                return 1;
            }
            *pParLen += (unsigned int)(p - pbak - len);
        }
    }
    if (pszString)
    {
        memcpy(pszString, pbak, len);
        *pBuffer = pbak + len;
    }
    return 0;
}

static int ParseString(unsigned char **pBuffer, unsigned char *pMax, unsigned char *pszString, unsigned int *pStrLen)
{
    unsigned char* p = *pBuffer;
    unsigned char type = *p++;
    unsigned int len;
    if (ParseLength(&p, pMax, &len))
    {
        return 1;
    }
    if (pszString == NULL || *pStrLen < len)
    {
        *pStrLen = len;
        return 0;
    }
    switch(type)
    {
    case V_ASN1_UTF8STRING:
        break;
    case V_ASN1_BMPSTRING:
        break;
    case V_ASN1_UNIVERSALSTRING:
        break;
    case V_ASN1_PRINTABLESTRING:
        break;
    case V_ASN1_BIT_STRING:
        break;
    case V_ASN1_OCTET_STRING:
        break;
    default:
        return 1;
        break;
    }
    memcpy(pszString, p, len);
    *pStrLen = len;
    *pBuffer = p+len;
    return 0;
}

static int ParseName(unsigned char **pBuffer, unsigned char *pMax, unsigned char **pszString, unsigned int *pStrLen)
{
	unsigned char *p = NULL;
	unsigned char  *pp = NULL;
	unsigned int allLen = 0;
	int rv = 0;

	do{	
		if(!pBuffer){
			break;
		}		
		p = *pBuffer;
		
		
		if (ParseSequence(&p, pMax, NULL, NULL, &allLen, NULL))	{
			break;
		}
		
		p += allLen;
		allLen = (unsigned int)(p - *pBuffer);

		if(pszString){
			pp = *pszString;
			if(!pp){
				pp = (unsigned char*)get_buffer(allLen);
			}
			if(pp){
				memcpy(pp, *pBuffer, allLen);
				*pszString = pp;
				*pStrLen = allLen;
			}else{
				break;
			}
		}
		*pBuffer = p;
		*pStrLen = allLen;
		rv = 0;
	}while(0);	
	return rv;
}

static int ParseValidity(unsigned char **pBuffer, unsigned char *pMax, pov_x_t* pValid)
{
    unsigned char *p = *pBuffer;
    unsigned int len;
    unsigned char bType;
    if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL))
    {
        return 1;
    }
	if (*p != V_ASN1_UTCTIME && *p != V_ASN1_GENERALIZEDTIME)
	{
		return 1;
	}
    if (ParseSequence(&p, pMax, NULL, NULL, &len, NULL))
    {
        return 1;
    }

	if (pValid)
	{
	    memcpy(pValid->Xnot_before, p, len);
	}
    p += len;
    if (*p != V_ASN1_UTCTIME &&
        *p != V_ASN1_GENERALIZEDTIME)
    {
        return 1;
    }
    bType = *p;
    if (ParseSequence(&p, pMax, NULL, NULL, &len, NULL))
    {
        return 1;
    }
	if (pValid)
	{
		memcpy(pValid->Xnot_after, p, len);
		pValid->Length = len;
	}
    *pBuffer = p + len;
    return 0;
}
static int Parse_certfile(const char *cert_file_name, struct cert_obj_st_t *cert_obj)
{
	unsigned char *pos = NULL;
	item_data_t cert_file_buffer;
	int cert_len = 0;
	int len = 0;
	int rv = -1;
	
	DPrintf("[WAPID]:: Parse certificate file %s\n", cert_file_name);

	memset(&cert_file_buffer, 0 , sizeof(cert_file_buffer));	
	cert_file_buffer.length = 4096;
	cert_file_buffer.data = (unsigned char*)get_buffer(cert_file_buffer.length);
	

	do{
		cert_len = x509_read_cert_file(cert_file_name, &cert_file_buffer);

		if(cert_len <= 0) 	break;
		//DPrintf("certificate file Base64:\n%s", cert_file_buffer.data);
		DPrintf("[WAPID]:: Parse AS certificate\n");
		if(get_pos_from_certfile(cert_file_buffer.data, cert_len, 
			PEM_STRING_AS_X509_CERTB, PEM_STRING_AS_X509_CERTE,
			&pos, &len) == NULL){
			break;
		}
		len = Base64_strip(pos, len, pos);
		if(len == 0)
			break;
		//wapi_str_dump("AS certificate Base64", pos, len);
		
		len = Base64Dec(pos, pos, len);

		//wapi_hex_dump("AS certificate Der", pos, len);

		cert_obj->asu_cert_st.data = (unsigned char*)get_buffer(len);
		if(!cert_obj->asu_cert_st.data)
			break;

		memcpy(cert_obj->asu_cert_st.data, pos, len); 
		cert_obj->asu_cert_st.length = len;
		//wapi_hex_dump("AS certificate", cert_obj->asu_cert_st.data, cert_obj->asu_cert_st.length);

		DPrintf("[WAPID]:: Parse USER certificate\n");
		if(get_pos_from_certfile(cert_file_buffer.data, cert_len, 
			PEM_STRING_USER_X509_CERTB, PEM_STRING_USER_X509_CERTE,
			&pos, &len) == NULL){
			break;
		}
		len = Base64_strip(pos, len, pos);
		if(len == 0)
			break;
		//wapi_str_dump("USER certificate Base64", pos, len);
		
		len = Base64Dec(pos, pos, len);

		//wapi_hex_dump("USER certificate Der", pos, len);

		cert_obj->user_cert_st.data = (unsigned char*)get_buffer(len);
		if(!cert_obj->user_cert_st.data)
			break;

		memcpy(cert_obj->user_cert_st.data, pos, len); 
		cert_obj->user_cert_st.length = len;

		DPrintf("[WAPID]:: Parse USER Private key pairs certificate\n");
		if(get_pos_from_certfile(cert_file_buffer.data, cert_len, 
			PEM_STRING_PRIKEYB, PEM_STRING_PRIKEYE,
			&pos, &len) == NULL){
			break;
		}
		len = Base64_strip(pos, len, pos);
		if(len == 0)
			break;
		//wapi_str_dump("USER Private key pairs Base64", pos, len);
		
		len = Base64Dec(pos, pos, len);

		//wapi_hex_dump("USER Private key pairs Der", pos, len);
		if(Parse_private_key(pos, len, &cert_obj->user_key) !=0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		wapi_hex_dump("USER Private key value", cert_obj->user_key.privatekey.data, cert_obj->user_key.privatekey.length);
		wapi_hex_dump("USER Public key value", cert_obj->user_key.publickey.data, cert_obj->user_key.publickey.length);
		rv = 0;
	}while(0);
	if(rv !=0){
		free_item(&cert_obj->user_key.privatekey, 0);
		free_item(&cert_obj->user_key.publickey, 0);
	}
	free_item(&cert_file_buffer, 0);
	return rv;
}

static int Parse_ca_certfile(const char *cert_file_name, struct cert_obj_st_t *cert_obj)
{
	unsigned char *pos = NULL;
	item_data_t cert_file_buffer;
	int cert_len = 0;
	int len = 0;
	int rv = -1;
	
	DPrintf("[WAPID]:: Parse CA from certificate file %s\n", cert_file_name);

	memset(&cert_file_buffer, 0 , sizeof(cert_file_buffer));	
	cert_file_buffer.length = 4096;
	cert_file_buffer.data = (unsigned char*)get_buffer(cert_file_buffer.length);
	

	do{
		cert_len = x509_read_cert_file(cert_file_name, &cert_file_buffer);

		if(cert_len <= 0) 	break;
		DPrintf("certificate file length %d, Base64:\n%s", cert_len, cert_file_buffer.data);
		DPrintf("[WAPID]:: Parse CA certificate\n");
		if(get_pos_from_certfile(cert_file_buffer.data, cert_len, 
			PEM_STRING_CA_X509_CERTB, PEM_STRING_CA_X509_CERTE,
			&pos, &len) == NULL){
			break;
		}
		len = Base64_strip(pos, len, pos);
		if(len == 0) {
			DPrintf("%s: base64 strip failed.\n", __func__);
            break;
        }
		wapi_str_dump("CA certificate Base64", pos, len);
		
		len = Base64Dec(pos, pos, len);

		wapi_hex_dump("CA certificate Der", pos, len);

		cert_obj->ca_cert_st.data = (unsigned char*)get_buffer(len);
		if(!cert_obj->ca_cert_st.data)
			break;

		memcpy(cert_obj->ca_cert_st.data, pos, len); 
		cert_obj->ca_cert_st.length = len;
		wapi_hex_dump("CA certificate", cert_obj->ca_cert_st.data, cert_obj->ca_cert_st.length);

		rv = 0;
	}while(0);
	free_item(&cert_file_buffer, 0);
	return rv;
}


static int Parse_private_key(const void *buffer, int bufferlen, struct key_bin_t *keypair)
{
	int rv = -1;
	unsigned char id = 0;
	unsigned char len = 0;
	unsigned char *pos =(unsigned char*) buffer;
		
	do{
		if((!pos) ||(bufferlen <=5)){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		/*step sequese id*/
		pos++; bufferlen--;
		len = *pos;  
		if(len != bufferlen-1) {
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			DPrintf("len = %d, Bufferlen=%d\n", len, bufferlen);
			break;
		}
		/*step sequese len*/
		pos++;bufferlen--;
		/*step Version ID*/
		pos++;bufferlen--;
		len = *pos;
		/*step Version len, value*/
		pos++; bufferlen--;
		pos++;bufferlen--;
		if(bufferlen <=0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		/*Parse privakey value*/
		id = *pos;
		pos++;bufferlen--;
		if(bufferlen <=0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		len = *pos;
		pos++;bufferlen--;

		if(bufferlen <=0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		bufferlen -= len;
		if(bufferlen <=0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		
		keypair->privatekey.data = (unsigned char*)get_buffer(len);
		if(!keypair->privatekey.data){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}

		memcpy(keypair->privatekey.data, pos, len);
		keypair->privatekey.length = len;
		/*step privatekey value*/
		pos+=len;

		/*step Context ID(Algorithm OID), len, value*/
		id = *pos;
		pos++; bufferlen--;
		if(bufferlen <=0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		len = *pos; 
		pos++;bufferlen -= len+1;
		if(bufferlen <=0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		pos+= len;	
		/*step Context ID(publikey value) len*/
		bufferlen -=2;
		if(bufferlen <= 0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		pos++; 
		len = *pos; pos++;

		/*step BTstring ID(publickey value)*/
		bufferlen -=2;
		if(bufferlen -2 <= 0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		pos++; /*BTS ID*/
		len = *pos; pos++; /*BTS len*/

		if((pos[0] != 0x0) ||(pos[1] != 0x04)){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}

		bufferlen -= len;
		
		if(bufferlen  != 0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}

		keypair->publickey.data = (unsigned char*)get_buffer(len-1);
		if(!keypair->publickey.data){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		
		memcpy(keypair->publickey.data, pos+1, len-1);
		keypair->publickey.length = len-1;
		rv = 0;
	}while(0);
	return rv;
}

static int ParsePubKey(unsigned char **pBuffer, unsigned char *pMax, unsigned char **pPubKey, unsigned int *pLen)
{
	#define TMP_BUF 100
	unsigned char *p = NULL;
	unsigned char *pp = NULL;
	unsigned int len = 0, ParLen;
	unsigned char pTmp[TMP_BUF] = {0};
	int rv = 0;

	do{
		if(!pBuffer){
			break;
		}
		p = *pBuffer;

		if(!pPubKey){
			pp = *pPubKey;
		}
		
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		if (ParseSequence(&p, pMax, NULL, NULL, &len, NULL)){
			break;
		}
		
		len = TMP_BUF;
		if (ParseOID(&p, pMax, pTmp, &len, &ParLen)){
			break;
		}
		if (hlpCheckOIDAndParam(pTmp, len, p, ParLen, 1)){
			break;
		}
		
		p += ParLen;   
		if ((*p & V_ASN1_PRIMITIVE_TAG) != V_ASN1_BIT_STRING){
			break;
		}
		
		if (ParseString(&p, pMax, NULL, &len) || len != PUBKEY_LEN + 1 + 1){
			break;
		}

		if (ParseString(&p, pMax, pTmp, &len)){
			break;
		}
		if (pTmp[1] != 0x04 || pTmp[0] != 0){
			break;
		}

		if(pPubKey){
			if(!pp){
				pp = (unsigned char*)get_buffer(len-1);
			}
			if(!pp){
				break;
			}else{
				 memcpy(pp, &pTmp[1], len-1);
			 	*pPubKey = pp;
			 	*pLen = len -1;
			}
		}
		rv = 0;
	}while(0);	
	return rv;
}

static unsigned char hlpCheckOIDAndParam(unsigned char *pOID, unsigned int dwOIDLen, unsigned char* pParam, unsigned int dwParamLen, unsigned char bIsPubKey)
{
    int i;
    const WOID* pWD = bIsPubKey ? &gPubKeyOID[0] : &gWOID[0];
    if (!pOID && !pParam)
    {
        return 1;
    }

    for(i = 0; i < WAPI_OID_NUMBER; i++)
    {
        if ( (pOID == NULL || (pWD[i].usOIDLen == dwOIDLen &&
                        memcmp(pWD[i].bOID, pOID, dwOIDLen) == 0) )
              &&
              (pParam == NULL || (pWD[i].usParLen == dwParamLen &&
                         memcmp(pWD[i].bParameter, pParam, dwParamLen) == 0) )
            )
        {
            return 1;
        }
    }
    return 0;
}

static unsigned char GetBase64Value(unsigned char ch)
{
    if ((ch >= 'A') && (ch <= 'Z')) 
        return ch - 'A'; 
    if ((ch >= 'a') && (ch <= 'z')) 
        return ch - 'a' + 26; 
    if ((ch >= '0') && (ch <= '9')) 
        return ch - '0' + 52; 
    switch (ch) 
	{ 
    case '+': 
        return 62; 
    case '/': 
        return 63; 
    case '=': /* base64 padding */ 
        return 0; 
    default: 
        return 0; 
	} 
}


static int Base64Dec(unsigned char *buf,const unsigned char*text,int size)
{
	unsigned char chunk[4];
	int parsenum=0;
	unsigned char* p = buf;

	if(size%4)
		return -1;

	while(size>0)
	{
		chunk[0] = GetBase64Value(text[0]); 
		chunk[1] = GetBase64Value(text[1]); 
		chunk[2] = GetBase64Value(text[2]); 
		chunk[3] = GetBase64Value(text[3]); 
		
		*buf++ = (chunk[0] << 2) | (chunk[1] >> 4); 
		*buf++ = (chunk[1] << 4) | (chunk[2] >> 2); 
		*buf++ = (chunk[2] << 6) | (chunk[3]);
		
		text+=4;
		size-=4;
		parsenum+=3;
	}

	if (0x30 == p[0])
	{
		if (0x82 == p[1])
		{
			parsenum = (p[2]<<8) + p[3] + 4;
		}
		else
		{
			parsenum = p[1] + 2;
		}
	}

	return parsenum;
}

/*ret:0-char of base64,1-\n or \r,-1-unknow char*/
static int getchartype_base64(unsigned char b)
{
	if (	(b>='A'&&b<='Z')
		||	(b>='a'&&b<='z')
		||	(b>='0'&&b<='9')
		||	'+'==b || '/'==b || '='==b)
	{
		return 0;
	}
	else if ('\r'==b || '\n'==b)
	{
		return 1;
	}
	return -1;
}


/*find mark in src*/
static const unsigned char* findmark_mem(const unsigned char* src, int lsrc, const  char* mark, int lmark)
{
	const unsigned char* p = src;
	const unsigned char* pe = src+lsrc;
	if (NULL==src || NULL==mark || lsrc<0 || lmark<0 || lsrc<lmark)
	{
		return NULL;
	}
	pe -= lmark;
	for (; p<=pe; p++)
	{
		if (0 == memcmp(p, mark, lmark))
		{
			return p;
		}
	}
	return NULL;
}

static int Base64_strip(const unsigned char *in, unsigned int inlen, unsigned char *out )
{
	unsigned char *p = (unsigned char*)in;
	int c = 0;
	for (; p<in+inlen; p++){
		int t = getchartype_base64(*p);
		if (0 == t)
		{
			out[c++] = *p;
		}
	}
	return c;
}


/* ---------------------------------------------------------------------------------------
 * [Name]      get_prikey_from_cert
 * [Function]   get information from cert buffer with flag
 * [Input]       const unsigned char *src_cert
 *                  const unsigned char *start_flag
 *                  const unsigned char *end_flag
 * [Output]     int * len 
 * [Return]     unsigned char * 
 *                            NULL  fail
                               !0     success (private key value)                                   
 * [Limitation] NULL
 * ---------------------------------------------------------------------------------------
 */

static unsigned char *get_pos_from_certfile(const unsigned char *src, int len, 
		const  char *start_strs, const  char *end_stars,
		unsigned char **ppos, int *plen)
{
	const unsigned char *p = src;
	const unsigned char *ps  = NULL;
	const unsigned char *pe  = NULL;
	int l0 = 0;
	int l1 = 0;
	//int c = 0;
	unsigned char *pos = NULL;
	
	do{	
		if ((!src)|| (!start_strs) || (!end_stars))
		{
			break;
		}

		l0 = strlen(start_strs);
		l1 = strlen(end_stars);
		
		ps = findmark_mem(p, len, start_strs, l0);
		pe = findmark_mem(p, len, end_stars, l1);
		
        printf("%s: l0=%d, l1=%d, from %p to %p\n", __func__, l0, l1, ps, pe);

		if ((!ps) || (!pe) || (ps+l0 >= pe)){
			break;
		}
		pos = (unsigned char*)ps+l0;
		*plen = pe-ps-l0;
		
	}while(0);
	
	*ppos = pos;

	return pos;
}

static  int x509_read_cert_file(const char *cert_file_name, item_data_t *cert_file_buffer)
{
	int rv = 0;
	FILE* fp = fopen((const char*)cert_file_name, "rb");

	do{
		if (fp == NULL){
			break;
		}
		
		rv = fread(cert_file_buffer->data, 1, cert_file_buffer->length, fp);
		
		fclose(fp);

	}while(0);

	return rv;
}


static int x509_get_pubkey(struct cert_bin_t *cert_st, item_data_t *publickey)
{
	unsigned int tmp;
	unsigned int parLen = 0;
	unsigned char tmpOID[100];
	unsigned char *p = NULL;
	unsigned char *pMax = NULL;
	unsigned char *pBAK = NULL;
	int rv = -1;

	if ((!cert_st) ||(!cert_st->data) || (!publickey)){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return rv; 
	}

	p = cert_st->data;
	pMax = p + cert_st->length;

	do{
		/* tbsCertificate */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL))	{
			break;
		}

		/* version */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL))	{
			break;
		}

		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}

		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL))	{
			break;
		}
		 /* only support V3 */
		if (*p != V_X509_V3) {
			break;
		}
		p += tmp;
		/* sn */
		pBAK = p;
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL))	{
			break;
		}


		p += tmp;
		if ((unsigned int)(p - pBAK) > 0xff){
			break;
		}

		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}

		tmp = sizeof(tmpOID);
		if (ParseOID(&p, pMax, tmpOID, &tmp, &parLen)){
			break;
		}

		if (p + parLen > pMax){
			break;
		}

		if (hlpCheckOIDAndParam(tmpOID, tmp, NULL, 0, 0)){
			break;
		}

		p += parLen;
		/* Issuer */
		if (ParseName(&p, pMax, NULL, &tmp)){
			break;
		}

		/* validity */
		if (ParseValidity(&p, pMax, NULL)){
			break;
		}

		/* subject */
		if (ParseName(&p, pMax, NULL, &tmp)){
			break;
		}

		/* pubkey der */

		if (ParsePubKey(&p, pMax, &publickey->data, &publickey->length)){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		rv = 0;
	} while(0);
	return rv;
}

static int x509_get_subject_der(struct cert_bin_t *cert_st, item_data_t *subject)
{
	unsigned int tmp;
	unsigned int parLen = 0;
	unsigned char tmpOID[100];
	unsigned char *p = NULL;
	unsigned char *pMax = NULL;
	unsigned char *pBAK = NULL;


	int rv  = -1;

	if ((!cert_st) || (!cert_st->data) ||(!subject)){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return rv;
	}

	p = cert_st->data;
	pMax = p + cert_st->length;

	do
	{
		/* tbsCertificate */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		/* version */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			break;
		}
		/* only support V3 */
		if (*p != V_X509_V3) {
			break;
		}
		
		p += tmp;
		/* SN */
		pBAK = p;
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			break;
		}
		p += tmp;
		if ((unsigned int)(p - pBAK) > 0xff){
			break;
		}

		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		
		tmp = sizeof(tmpOID);
		if (ParseOID(&p, pMax, tmpOID, &tmp, &parLen)){
			break;
		}
		if (p + parLen > pMax){
			break;
		}
		if (hlpCheckOIDAndParam(tmpOID, tmp, NULL, 0, 0)){
			break;
		}

		p += parLen;
		/* Issuer */
		if (ParseName(&p, pMax, NULL, &tmp)){
			break;
		}
		/* validity */
		if (ParseValidity(&p, pMax, NULL)){
			break;
		}

		/* subject */
		if (ParseName(&p, pMax, &subject->data, &subject->length)){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		rv = 0;
	} while(0);
	return rv;
}


static int x509_get_issuer_der(struct cert_bin_t *cert_st, item_data_t *issuer)
{
	unsigned int tmp;
	unsigned int parLen = 0;
	unsigned char tmpOID[100];
	unsigned char *p       = NULL;
	unsigned char *pMax = NULL;
	unsigned char *pBAK = NULL;
	int rv  = -1;


	if ((!cert_st) ||(!cert_st->data) || (!issuer)){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return rv;
	}

	p = cert_st->data;
	pMax = p + cert_st->length;


	do{
		/* tbsCertificate */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		/* version */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			break;
		} 
		/* only support V3 */
		if (*p != V_X509_V3){
			break;
		}
		p += tmp;
		/* SN */
		pBAK = p;
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			break;
		}
		p += tmp;
		if ((unsigned int)(p - pBAK) > 0xff){
			break;
		}

		/* signature algorithm */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		tmp = sizeof(tmpOID);
		if (ParseOID(&p, pMax, tmpOID, &tmp, &parLen)){
			break;
		}
		if (p + parLen > pMax){
			break;
		}
		if (hlpCheckOIDAndParam(tmpOID, tmp, NULL, 0, 0)){
			break;
		}

		p += parLen;
		/* Issuer */
		if (ParseName(&p, pMax, &issuer->data, &issuer->length))	{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		rv = 0;
	} while(0);
	return rv;
}


static int x509_get_serial_der(struct cert_bin_t *cert_st, item_data_t *serial)
{
	unsigned int tmp;
	unsigned char *p = NULL;
	unsigned char *pMax = NULL;
	unsigned char *pBAK = NULL;
	int rv  = -1;


	if ((!cert_st) ||(!cert_st->data) || (!serial)){\
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return rv;
	}

	p = cert_st->data;
	pMax = p + cert_st->length;

	do{
		/* tbsCertificate */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		/* version */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		} 
		/* only support V3 */
		if (*p != V_X509_V3){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		p += tmp;
		/* SN */
		pBAK = p;
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL))
		{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		p += tmp;
		if ((unsigned int)(p - pBAK) > 0xff)
		{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		if(!serial->data){
			serial->length = p - pBAK;
			serial->data = (unsigned char*)get_buffer(serial->length);
			if(!serial->data){
				DPrintf("error at %s :%d\n", __func__, __LINE__);
				serial->length = 0;
				break;
			}
		}
		memcpy(serial->data, pBAK, p - pBAK);
		serial->length = p - pBAK;
		rv = 0;
	} while(0);
    return rv;
}



static int x509_get_cert_sign(struct cert_bin_t *cert_st, unsigned char *out)
{
	unsigned char *p       = NULL;
	unsigned char *pMax = NULL;
	unsigned int tmp = 0;
	int tmp_len = 0;
	int rv = -1;	

	if ((!cert_st) ||(!cert_st->data) ||(!out)){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return rv;
	}

	p = cert_st->data;
	pMax = p + cert_st->length;
 
	do
	{
		/* tbsCertificate */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			break;
		}
		/* skip the cert main informations */
		p += tmp;
		tmp = 0;

		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			break;
		}
		/* skip the sign arithmetic */
		p += tmp;
		tmp = 0;

		/* parse sign value -------start--------*/
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}

		/* skip the compress flag */
		p++;

		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}

		/* first parts */
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			break;
		}

		if (tmp > 0x18 && *p == 0x00){
			/* skip 0x00 */
			p++;
			tmp_len = (int)tmp - 1;
			memcpy(out, p, tmp_len);
		}
		else
		{
			if (tmp == 0x17)
			{	
				p--;
				tmp_len = (int)tmp + 1;
				memcpy(out, p, tmp_len);
				out[0] = 0x00;
			}
			else
			{
				tmp_len = (int)tmp;
				memcpy(out, p, tmp_len);
			}
		}
		p += tmp_len;
		tmp = 0;

		/* second parts */
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			break;
		}

		if (tmp > 0x18 && *p == 0x00){
			/* skip 0x00 */
			p++;
			memcpy(out+tmp_len, p, tmp - 1);
		}
		else{
			if (tmp == 0x17){	
				p--;
				memcpy(out+tmp_len, p, tmp + 1);
				out[tmp_len] = 0x00;
			}else{
				memcpy(out+tmp_len, p, tmp_len);
			}
		}
		/* parse sign value -------end--------*/

		/* sccess return */
		rv = 0;

	}while(0);

	return rv;
}

static int x509_get_cert_sign_inlen(struct cert_bin_t *cert_st)
{
	unsigned char *p       = NULL;
	unsigned char *pMax = NULL;
	unsigned char *pBAK = NULL;
	unsigned int tmp;
	int rv = 0;	

	if ((!cert_st) ||(!cert_st->data)){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return rv; 
	}

	p = cert_st->data;
	pMax = p + cert_st->length;

	do{
		/* tbsCertificate */
		if (ParseSequence(&p, pMax, NULL, NULL, NULL, NULL)){
			break;
		}
		pBAK = p;	
		if (ParseSequence(&p, pMax, NULL, NULL, &tmp, NULL)){
			break;
		}
		/* sign(input) availability length */
		rv = ((int)tmp + (p - pBAK));
	}while(0);

	return rv;
}

void x509_free_obj_data(struct cert_obj_st_t *cert_obj)
{
	if(!cert_obj)
		return;
	free_item(&cert_obj->asu_cert_st, 0);
	free_item(&cert_obj->ca_cert_st, 0); /*三证书*/
	free_item(&cert_obj->user_cert_st, 0);
	free_item(&cert_obj->asu_publickey, 0);
	free_item(&cert_obj->ca_pubkey, 0); /*三证书*/
	free_item(&cert_obj->user_key.privatekey, 0);
	free_item(&cert_obj->user_key.publickey, 0);
}


static   struct cert_obj_st_t  *cert_objs[3];

void cert_obj_register( struct cert_obj_st_t *cert_obj)
{
	if(cert_obj->cert_type > 2)
	{
		DPrintf("%s: certificate %s has an invalid cert_index %u\n",
			__func__, cert_obj->cert_name,  cert_obj->cert_type);
		return;
	}


	if((cert_objs[cert_obj->cert_type] != NULL) && (cert_objs[cert_obj->cert_type] != cert_obj))
	{
		DPrintf("%s: certificate object %s registered with a different template\n",
			__func__, cert_obj->cert_name);
		return;
	}

	memset(&cert_obj->asu_cert_st, 0, sizeof(struct cert_bin_t));
	memset(&cert_obj->user_cert_st, 0, sizeof(struct cert_bin_t));
	memset(&cert_obj->user_key, 0, sizeof(struct key_bin_t));
	
	cert_objs[cert_obj->cert_type] = cert_obj;
	return;
}


void cert_obj_unregister(const struct cert_obj_st_t *cert_obj)
{
	int index = cert_obj->cert_type;
	
	if(index> 3)
	{
		DPrintf("%s: certificate %s has an invalid cert_index %u\n",
			__func__, cert_obj->cert_name,  index);
		return;
	}

	if ((cert_objs[index] != NULL) && (cert_objs[index] != cert_obj))
	{
		DPrintf("cert_obj address 	%p\n", cert_obj);
		DPrintf("obj address 		%p\n", cert_objs[index]);
		DPrintf("%s: certificate object %s registered with a different template\n",
			__func__, cert_obj->cert_name);
		return;
	}
	
	x509_free_obj_data(cert_objs[index]);
	cert_objs[index] = NULL;
}


const struct cert_obj_st_t *get_cert_obj(int index)
{
	if(index < 3)
	{
		return cert_objs[index];
	}
	else
		return NULL;
}


int x509_verify_cert(struct cert_obj_st_t *cert_obj, struct cert_bin_t *cert, item_data_t *publickey)
{

	item_data_t sign_value;
	int  sign_inlen = 0;
	int  rv = -1;

	if((!cert_obj) || (cert_obj->verify == NULL)|| (!cert)||(!publickey) ){
		return -1;
	}
	memset(&sign_value, 0, sizeof(sign_value));

	if((!cert->data) || (!publickey->data)){
		return -1;
	}
	
	do{
		
		sign_value.data = (unsigned char*)get_buffer(publickey->length -1);

		if(!sign_value.data){
			break;
		}
		
		sign_value.length = publickey->length-1;
		if(x509_get_cert_sign(cert, sign_value.data) < 0){
			break;
		}

		sign_inlen = x509_get_cert_sign_inlen(cert);
		if(sign_inlen <= 0){
			break;
		}
		if ((*cert_obj->verify)(publickey->data, publickey->length, 
				cert->data+4, sign_inlen, sign_value.data, sign_value.length) <= 0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		
		rv = 0;
	}while(0);
	
	free_item(&sign_value, 0);
	
	return rv;
}

int load_ca_x509(struct ap_config *config, struct cert_obj_st_t *cert_obj)
{
    int rv = -1;
    do{
        if (Parse_ca_certfile(config->cert_name, cert_obj)!=0) {
            DPrintf("%s: Parse CA cert file failed.\n", __func__);
            break;
        }
  		if((*cert_obj->get_public_key)(&cert_obj->ca_cert_st, &cert_obj->ca_pubkey) != 0){
            DPrintf("%s: Get CA public key failed.\n", __func__);
    		break;
	    }
        rv = 0;
    }while(0);
	if(rv != 0){
		x509_free_obj_data(cert_obj);
	}
	return rv;
}

int load_x509(struct ap_config *config,  struct cert_obj_st_t *cert_obj)
{
	int rv = -1;
	unsigned char *pos = NULL;
	do{
		if(Parse_certfile(config->cert_name, cert_obj)!=0){
			break;
		}

		DPrintf("[WAPID]:: Parse certificate file success\n");
	    
	        if(config->cert_mode==2) {
	    		if((*cert_obj->get_public_key)(&cert_obj->ca_cert_st, &cert_obj->ca_pubkey) != 0){
		    		break;
			    }

	    		DPrintf("[WAPID]:: get CA publickey value success\n");
		    	pos = cert_obj->ca_cert_st.data;
			if(x509_verify_cert(cert_obj, &cert_obj->ca_cert_st, &cert_obj->ca_pubkey) != 0){
				DPrintf("error at %s :%d\n", __func__, __LINE__);
	    			break;
		    	}
			
			DPrintf("[WAPID]:: Verify CA certificate success\n");
		
	        }
 
    		if((*cert_obj->get_public_key)(&cert_obj->asu_cert_st, &cert_obj->asu_publickey) != 0){
	    		break;
		    }

    		DPrintf("[WAPID]:: get ASU publickey value success\n");
	    	pos = cert_obj->asu_cert_st.data;

	       if(config->cert_mode==1) 
		{			
			if(x509_verify_cert(cert_obj, &cert_obj->asu_cert_st, &cert_obj->asu_publickey) != 0){
				DPrintf("error at %s :%d\n", __func__, __LINE__);
	    			break;
		    	}
			DPrintf("[WAPID]:: Verify ASU certificate success\n");

			if(x509_verify_cert(cert_obj, &cert_obj->user_cert_st, &cert_obj->asu_publickey) != 0){
				DPrintf("error at %s :%d\n", __func__, __LINE__);
				break;
			}
			DPrintf("[WAPID]:: Verify user certificate success\n");
	       }

		if(config->cert_mode==2) 
		{			
			if(x509_verify_cert(cert_obj, &cert_obj->asu_cert_st, &cert_obj->ca_pubkey) != 0){
				DPrintf("error at %s :%d\n", __func__, __LINE__);
	    			break;
		    	}
			DPrintf("[WAPID]:: Verify ASU certificate success\n");
			
			if(x509_verify_cert(cert_obj, &cert_obj->user_cert_st, &cert_obj->ca_pubkey) != 0){
				DPrintf("error at %s :%d\n", __func__, __LINE__);
				break;
			}
			DPrintf("[WAPID]:: Verify user certificate success\n");
	       }
		
	
		if((*cert_obj->verify_key)(cert_obj->user_key.publickey.data,
				cert_obj->user_key.publickey.length,
				cert_obj->user_key.privatekey.data,
				cert_obj->user_key.privatekey.length) != 0){
			break;
		}
		
		DPrintf("[WAPID]:: Verify user key pairs success\n");	

		rv = 0;
	}while(0);
	if(rv != 0){
		x509_free_obj_data(cert_obj);
	}
	return rv;
}


int register_certificate(struct eloop_data *eloop)
{
	int res =  -1;
	if(!eloop){
		return res;
	}
	unsigned short  index = eloop->cert_info.config.used_cert; /*取证书类型号*/
	DPrintf("eloop.cert_info.config.used_cert = %d\n",index);	
#if 0	
	res = LOAD_CERT(CERT_TYPE[index], ap->config.cert[index], &cert_objs[index]);
#endif
	switch(index)
	{
		case 0:
		case 2:
		default:	
			break;
		case 1:
			/*释放证书对象中分配的资源*/
			x509_free_obj_data((struct cert_obj_st_t *)cert_objs[index]);
			/*三证书*/
			if(eloop->cert_info.config.cert_mode==2)
			{
			    res =load_ca_x509(&eloop->cert_info.config, cert_objs[index]);
			}
            
			/*加载X509证书*/
			res = load_x509(&eloop->cert_info.config, cert_objs[index]);
			break;
	}
	
	if(res == 0)
	{
		eloop->cert_info.ap_cert_obj= (struct cert_obj_st_t *)cert_objs[index];
	}
	return res;	
}



/*x509证书对象描述*/
static struct cert_obj_st_t  cert_obj_x509 = {
	.cert_type	  = CERT_OBJ_X509,
	.cert_name = "x509v3",
	
	.get_public_key	= x509_get_pubkey,
	.get_subject	= x509_get_subject_der,
	.get_issuer	= x509_get_issuer_der,
	.get_serial	= x509_get_serial_der,
	.verify_key = ecc192_verify_key_pair,
	.sign	= ecc192_sign,
	.verify = ecc192_verify,
	.dh = ecc192_ecdh,
	.genkey = ecc192_genkey_pair,
};
/*注销x509证书对象*/
void X509_exit()
{
	cert_obj_unregister(&cert_obj_x509);
}


/*注册x509证书对象*/
int X509_init()
{
	cert_obj_register(&cert_obj_x509);
	DPrintf("[WAPID]:: x.509 certificate object register success\n");
	return 0;
}




