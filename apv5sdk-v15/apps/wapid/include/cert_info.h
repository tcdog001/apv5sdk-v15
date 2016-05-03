/** @file cert_info.h
 *  @brief This header file contains data structures and function declarations of cert
 *
 *  Copyright (C) 2001-2008, Iwncomm Ltd.
 */
 

#ifndef __CERT_INFO__H__
#define __CERT_INFO__H__

#include "structure.h"
#include "ecc_crypt.h"


#define V_X509_V1   0
#define V_X509_V2   1
#define V_X509_V3   2

#define V_ASN1_UNIVERSAL		0x00
#define	V_ASN1_APPLICATION		0x40
#define V_ASN1_CONTEXT_SPECIFIC		0x80
#define V_ASN1_PRIVATE			0xc0

#define V_ASN1_CONSTRUCTED		0x20
#define V_ASN1_PRIMITIVE_TAG		0x1f

#define V_ASN1_UNDEF			-1
#define V_ASN1_EOC			0
#define V_ASN1_BOOLEAN			1	/**/
#define V_ASN1_INTEGER			2
#define V_ASN1_BIT_STRING		3
#define V_ASN1_OCTET_STRING		4
#define V_ASN1_NULL			5
#define V_ASN1_OBJECT			6
#define V_ASN1_OBJECT_DESCRIPTOR	7
#define V_ASN1_EXTERNAL			8
#define V_ASN1_REAL			9
#define V_ASN1_ENUMERATED		10
#define V_ASN1_UTF8STRING		12
#define V_ASN1_SEQUENCE			16
#define V_ASN1_SET			17
#define V_ASN1_NUMERICSTRING		18	/**/
#define V_ASN1_PRINTABLESTRING		19
#define V_ASN1_T61STRING		20
#define V_ASN1_TELETEXSTRING		20	/* alias */
#define V_ASN1_VIDEOTEXSTRING		21	/**/
#define V_ASN1_IA5STRING		22
#define V_ASN1_UTCTIME			23
#define V_ASN1_GENERALIZEDTIME		24	/**/
#define V_ASN1_GRAPHICSTRING		25	/**/
#define V_ASN1_ISO64STRING		26	/**/
#define V_ASN1_VISIBLESTRING		26	/* alias */
#define V_ASN1_GENERALSTRING		27	/**/
#define V_ASN1_UNIVERSALSTRING		28	/**/
#define V_ASN1_BMPSTRING		30


#define WAPI_OID_NUMBER     1

/*ECDSA192+SHA256*/
#define WAPI_ECDSA_OID          "1.2.156.11235.1.1.1"
/*CURVE OID*/
#define WAPI_ECC_CURVE_OID      "1.2.156.11235.1.1.2.1"

#define ECDSA_ECDH_OID          "1.2.840.10045.2.1"


#define PEM_STRING_AS_X509_CERTB		"-----BEGIN ASU CERTIFICATE-----"
#define PEM_STRING_AS_X509_CERTE		"-----END ASU CERTIFICATE-----"

#define PEM_STRING_USER_X509_CERTB	"-----BEGIN USER CERTIFICATE-----"
#define PEM_STRING_USER_X509_CERTE	"-----END USER CERTIFICATE-----"

#define PEM_STRING_CA_X509_CERTB		"-----BEGIN CA CERTIFICATE-----"
#define PEM_STRING_CA_X509_CERTE		"-----END CA CERTIFICATE-----"

#define PEM_STRING_PRIKEYB			"-----BEGIN EC PRIVATE KEY-----"
#define PEM_STRING_PRIKEYE				"-----END EC PRIVATE KEY-----"

#define CERT_OBJ_NONE 0
#define  CERT_OBJ_X509 1

typedef struct _WOID 
{
    const char*     pszOIDName;
    unsigned short  usOIDLen;
    unsigned short  usParLen;
    unsigned short  bOID[MAX_BYTE_DATA_LEN];        
    unsigned short  bParameter[MAX_BYTE_DATA_LEN];  
} WOID, *PWOID;

#define X509_TIME_LEN       15
typedef struct _pov_x {
    struct  
    {
        unsigned int not_before;   
        unsigned int not_after;    
    };
    struct  
    {
        unsigned int Length;
        unsigned char Xnot_before[X509_TIME_LEN + 1];    
        unsigned char Xnot_after[X509_TIME_LEN + 1];     
    };
} pov_x_t;

struct ap_config;

struct cert_obj_st_t{
	int cert_type;
	char *cert_name;
	struct cert_bin_t  asu_cert_st;
	struct cert_bin_t  user_cert_st;
	struct cert_bin_t  ca_cert_st;
	item_data_t  asu_publickey;  
	item_data_t  ca_pubkey;  
	struct key_bin_t  user_key;
	int (*get_public_key)(struct cert_bin_t *cert_st, item_data_t *publickey);
	int (*get_subject)(struct cert_bin_t *cert_st, item_data_t *subject);
	int (*get_issuer)(struct cert_bin_t *cert_st, item_data_t *issuer);
	int (*get_serial)(struct cert_bin_t *cert_st, item_data_t *serial);
	int (*verify_key)(const uchar_t *publickey, uint_t publickey_len, const uchar_t *privatekey, uint_t privatekey_len);
	int (*sign)(const uchar_t *privatekey, uint_t privatekey_len,
			const uchar_t *data, uint_t data_len, uchar_t *sign_buffer, uint_t *sign_buffer_len);
	int (*verify)(const uchar_t *publickey, uint_t publickey_len,
			const uchar_t *data, uint_t data_len, const uchar_t *sign_value, uint_t sign_value_len);
	int (*dh)(const uchar_t *publickey, uint_t publickey_len,
			const uchar_t *privatekey, uint_t privatekey_len,
			uchar_t *ecdhkey_buffer, uint_t *ecdhkey_buffer_len);	
	int (*genkey)(uchar_t *publickey_buffer, uint_t *publickey_buffer_len,
			uchar_t *privatekey_buffer, uint_t *privatekey_buffer_len);
};
void cert_obj_register(struct cert_obj_st_t *cert_obj);
void cert_obj_unregister(const struct cert_obj_st_t *cert_obj);
void x509_free_obj_data(struct cert_obj_st_t *cert_obj);
int load_x509(struct ap_config *config,  struct cert_obj_st_t *cert_obj);
const struct cert_obj_st_t *get_cert_obj(int index);
void X509_exit();
int X509_init();
#endif
