#ifndef _APNP_IPADDRESS_H
#define _APNP_IPADDRESS_H
unsigned long 
do_ioctl_get_ipaddress(const char *dev);

char *
ip2str(uint32_t ip, char *str, size_t size);

int 

do_ioctl_get_mac(char *dev,char *mac);

#endif
