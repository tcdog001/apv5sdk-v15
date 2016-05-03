/*	For Public Interface Function.
 *
 */

#include "apnp_util.h"

static BOOL str_is_legal(char c) 
{
	return	((c >= 0x00 && c <= 0x7F) &&(c != 0x0A) && (c != 0x0D));
}

char *apnp_strncpy(char *dest, char *src, size_t num)
{
	int i = 0;
	char* p = dest;
	if (NULL == dest || NULL == src) {
		return NULL;
	}

	for (i = 0; i < num && src[i] != '\0'; i++) {
		if (str_is_legal(src[i])) {
			*dest++ = src[i];
		} 
	}
	*dest = '\0';
	return p;
}

char *apnp_strdeal(char *dest,char *src)
{
	int i = 0;
	char* p = dest;
	if (NULL == src) {
		return NULL;
	}

	for (i = 0;src[i] != '\0'; i++) {
		if (str_is_legal(src[i])) {
			*dest++ = src[i];
		} 
	}
	*dest = '\0';
	return p;
}


void 
apnp_syslogv(int priority,const char *message,va_list args)
{
	vsyslog(priority,message,args);
//	if(LOG_ERR == priority)
		//exit(1);
}		



void 
apnp_syslog(int priority,const char *message,...)
{
	int ret;
	va_list args;
	va_start(args,message);

	apnp_syslogv(priority,message,args);

	va_end(args);
	return (ret);

}








