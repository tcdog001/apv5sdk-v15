#ifndef __CONFIG__H__
#define __CONFIG__H__
#include "cert_info.h"

struct ap_config
{
	
	char cert_name[256];
	unsigned short used_cert;
//	unsigned short pad;
    unsigned short cert_mode;
};
struct _apcert_info {
	struct cert_obj_st_t *ap_cert_obj;
	struct ap_config config;
};
#endif
