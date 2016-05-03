/* vi: set sw=4 ts=4: */
/* script.c
 *
 * Functions to call the DHCP client notification scripts
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "common.h"
#include "dhcpd.h"
#include "dhcpc.h"
#include "options.h"


/* get a rough idea of how long an option will be (rounding up...) */
static const int max_option_length[] = {
	[OPTION_IP] =		sizeof("255.255.255.255 "),
	[OPTION_IP_PAIR] =	sizeof("255.255.255.255 ") * 2,
	[OPTION_STRING] =	1,
	[OPTION_BOOLEAN] =	sizeof("yes "),
	[OPTION_U8] =		sizeof("255 "),
	[OPTION_U16] =		sizeof("65535 "),
	[OPTION_S16] =		sizeof("-32768 "),
	[OPTION_U32] =		sizeof("4294967295 "),
	[OPTION_S32] =		sizeof("-2147483684 "),
};


static inline int upper_length(int length, int opt_index)
{
	return max_option_length[opt_index] *
		(length / option_lengths[opt_index]);
}


static int sprintip(char *dest, char *pre, uint8_t *ip)
{
	return sprintf(dest, "%s%d.%d.%d.%d", pre, ip[0], ip[1], ip[2], ip[3]);
}


/* really simple implementation, just count the bits */
static int mton(struct in_addr *mask)
{
	int i;
	unsigned long bits = ntohl(mask->s_addr);
	/* too bad one can't check the carry bit, etc in c bit
	 * shifting */
	for (i = 0; i < 32 && !((bits >> i) & 1); i++);
	return 32 - i;
}


/* Fill dest with the text of option 'option'. */
static void fill_options(char *dest, uint8_t *option,
			const struct dhcp_option *type_p)
{
	int type, optlen;
	uint16_t val_u16;
	int16_t val_s16;
	uint32_t val_u32;
	int32_t val_s32;
	int len = option[OPT_LEN - 2];
	dest += sprintf(dest, "%s=", type_p->name);
	type = type_p->flags & TYPE_MASK;
	optlen = option_lengths[type];
	for (;;) {
		switch (type) {
		case OPTION_IP_PAIR:
			dest += sprintip(dest, "", option);
			*(dest++) = '/';
			option += 4;
			optlen = 4;
		case OPTION_IP:	/* Works regardless of host byte order. */
			dest += sprintip(dest, "", option);
			break;
		case OPTION_BOOLEAN:
			dest += sprintf(dest, *option ? "yes" : "no");
			break;
		case OPTION_U8:
			dest += sprintf(dest, "%u", *option);
			break;
		case OPTION_U16:
			memcpy(&val_u16, option, 2);
			dest += sprintf(dest, "%u", ntohs(val_u16));
			break;
		case OPTION_S16:
			memcpy(&val_s16, option, 2);
			dest += sprintf(dest, "%d", ntohs(val_s16));
			break;
		case OPTION_U32:
			memcpy(&val_u32, option, 4);
			dest += sprintf(dest, "%lu", (unsigned long) ntohl(val_u32));
			break;
		case OPTION_S32:
			memcpy(&val_s32, option, 4);
			dest += sprintf(dest, "%ld", (long) ntohl(val_s32));
			break;
		case OPTION_STRING:
			memcpy(dest, option, len);
			dest[len] = '\0';
			return;	 /* Short circuit this case */
		}
		option += optlen;
		len -= optlen;
		if (len <= 0) break;
		dest += sprintf(dest, " ");
	}
}

/* get correspond index */
static int get_index( int code )
{
    int i = 0;
    while( dhcp_option43[i].code && (dhcp_option43[i].code != code) ){
        i++;
    }
    return (0 == dhcp_option43[i].code) ? -1 : i;
}

/* get sub option from option43 */
static unsigned int get_sub_options( uint8_t *option, char ***sub_opt )
{
    unsigned int length = option[OPT_LEN - 2];
    uint8_t *temp;
    int iter = 0;
    int i = 0;
    int count = 0;
    int index = -1;
    
    if( !option )
        return 0;
    while( length > iter )
    {
        temp = option + iter;
        if ( (index=get_index(temp[OPT_CODE])) >= 0 )
        {
            count++;
        }
        iter += (OPT_DATA + temp[OPT_LEN]);
    }
    *sub_opt = xzalloc(sizeof(char *) * count);

    iter = 0;
    while( length >= iter )
    {  
        temp = option + iter;
        if ( (index=get_index( temp[OPT_CODE] )) >= 0 )
        {
            (*sub_opt)[i] = xmalloc(
                            upper_length( temp[OPT_LEN], dhcp_option43[index].flags & TYPE_MASK) + 
			                strlen(dhcp_option43[index].name) + 2);
		    fill_options((*sub_opt)[i++], temp+OPT_DATA, &dhcp_option43[index]);
        }
        iter += (OPT_DATA + temp[OPT_LEN]);
    }
    return count;
}

/* put all the parameters into an environment */
static char **fill_envp(struct dhcpMessage *packet)
{
	int num_options = 0;
    int num_sub_options = 0;
	int i, j, k,mlen,n;
	char *mem189 = NULL;
	char *mem138 = NULL;
	char **envp;
    char **sub_opt;
	uint8_t *temp;
	struct in_addr subnet;
	char over = 0;
    unsigned int vendor_flag = 0;
	int high,low;

	if (packet == NULL)
		num_options = 0;
	else {
		for (i = 0; dhcp_options[i].code; i++)
			if ( (temp = get_option(packet, dhcp_options[i].code)) ) {
				num_options++;
				if (dhcp_options[i].code == DHCP_SUBNET)
					num_options++; /* for mton */
                /* for option 43, get sub options number */
                if (dhcp_options[i].code == DHCP_VENDOR_INFO)
                {
                    //if( 0 != (temp[OPT_LEN-2]%4) )
                    {
                        vendor_flag = 1;
                        num_options--;
                        num_sub_options = get_sub_options( temp, &sub_opt );      
                        num_options += num_sub_options;
                    }
                }
			}
		if (packet->siaddr) num_options++;
		if ((temp = get_option(packet, DHCP_OPTION_OVER)))
			over = *temp;
		if (!(over & FILE_FIELD) && packet->file[0]) num_options++;
		if (!(over & SNAME_FIELD) && packet->sname[0]) num_options++;
	}

//	envp = xzalloc(sizeof(char *) * (num_options + 5)); 
	/*add by sundl@*/
	envp = xzalloc(sizeof(char *) * (num_options + 7));
	memset(envp,0,sizeof(char *) * (num_options + 7));
	/*end*/
	printf("envp Count:%d\n",num_options + 7);
	j = 0;
	envp[j++] = xasprintf("interface=%s", client_config.interface);
	envp[j++] = xasprintf("PATH=%s",
		getenv("PATH") ? : "/bin:/usr/bin:/sbin:/usr/sbin");
	envp[j++] = xasprintf("HOME=%s", getenv("HOME") ? : "/");

	if (packet == NULL) return envp;

	envp[j] = xmalloc(sizeof("ip=255.255.255.255"));
	sprintip(envp[j++], "ip=", (uint8_t *) &packet->yiaddr);

	for (i = 0; dhcp_options[i].code; i++) {
		temp = get_option(packet, dhcp_options[i].code);
		if (!temp)
			continue;
        if ( (dhcp_options[i].code == DHCP_VENDOR_INFO) && (1 == vendor_flag) )
        {
            for(k=0; k<num_sub_options; k++)
            {
                envp[j++] = sub_opt[k];
            }
            continue;
        }
		envp[j] = xmalloc(upper_length(temp[OPT_LEN - 2],
			dhcp_options[i].flags & TYPE_MASK) + strlen(dhcp_options[i].name) + 2);
		fill_options(envp[j++], temp, &dhcp_options[i]);

		/* Fill in a subnet bits option for things like /24 */
		if (dhcp_options[i].code == DHCP_SUBNET) {
			memcpy(&subnet, temp, 4);
			envp[j++] = xasprintf("mask=%d", mton(&subnet));
		}
	}
	if (packet->siaddr) {
		envp[j] = xmalloc(sizeof("siaddr=255.255.255.255"));
		sprintip(envp[j++], "siaddr=", (uint8_t *) &packet->siaddr);
	}
	if (!(over & FILE_FIELD) && packet->file[0]) {
		/* watch out for invalid packets */
		packet->file[sizeof(packet->file) - 1] = '\0';
		envp[j++] = xasprintf("boot_file=%s", packet->file);
	}
	if (!(over & SNAME_FIELD) && packet->sname[0]) {
		/* watch out for invalid packets */
		packet->sname[sizeof(packet->sname) - 1] = '\0';
		envp[j++] = xasprintf("sname=%s", packet->sname);
	}
	/*add by sundl@*/
	for (i = 0; dhcp_options[i].code; i++)
		if ( (temp = get_option(packet, dhcp_options[i].code)) ){
			if (dhcp_options[i].code == DHCP_ACADDR_BD)
			{
				  printf("In parsing packet:\n");
				  
				  mem189 = malloc(sizeof (char)*200);
				  if(NULL == mem189)
				  {
					printf("malloc memory failure.\n");	
					return NULL;
				  }
				  memset(mem189,0,200);
				  memcpy(mem189,temp,*(temp -1));
				  printf("Option 189 length:%d,Payload:%s\n",*(temp -1),mem189);					  
				  
				  if(NULL != strchr(mem189,' ')|| NULL != strchr(mem189,','))
				  	{
				  		for(int num=0;num<strlen(mem189);num++)
							if( mem189[num]==' ' || mem189[num]==',' )
									mem189[num]='\n';
				  	}
				  envp[j] = xasprintf("acaddr189=%s",mem189);
				  printf("\nenvp[%d]:%s\n\n",j,envp[j++]);

				  free(mem189);
				  mem189 = NULL;
			}
		}

	for (i = 0; dhcp_options[i].code; i++)
		if ( (temp = get_option(packet, dhcp_options[i].code)) ){
			if (dhcp_options[i].code == DHCP_ACADDR_8A)
			{
				  printf("In parsing packet:\n");
				  
				  mem138 = (char *)malloc(sizeof(char) *(*(temp - 1)) + 1);
				  if( NULL == mem138)
				  {
					printf("malloc memory failure.\n");	
					return NULL;
				  }
				  memset(mem138,0,*(temp - 1) + 1);
				  memcpy(mem138,temp,*(temp - 1));
				  printf("Option 138 length:%d\n",*(temp -1));	
				  
				  mlen = *(temp -1)/4 * 16 + 1;
				  char *pMem = (char *)malloc(mlen + 1);
				  if( NULL == pMem)
				  {
					printf("malloc memory failure.\n");	
					return NULL;
				  }
				  memset(pMem,0,mlen + 1);
				  
				  char *src = NULL,*dest = NULL;
				  dest = pMem;
				  src  = mem138;  
				  for(int num=0;num<*(temp-1);num++)
				  {
					  high = (*(src+num)>>4)&0x0F;
					  low  = *(src+num)&0x0F;
					  sprintf(dest,"%i",high*16+low);
					  if (high*16+low >99)		
					   dest += 3;
					  else if(high*16+low > 9)  //10.10.10.10
					   dest += 2;
					  else 						//1.1.1.1
					   dest += 1;

					  if ((num+1) % 4!=0)
					  	 *(dest)='.';  			
					  else
					  	 *(dest)='\n'; 			//depart ip address
					  dest++;
			  	}
			  
			  printf("Payload Length:%d\n%s",strlen(pMem),pMem);
			  envp[j] = xasprintf("acaddr138=%s",pMem);
			  printf("\nenvp[%d]:%s\n",j,envp[j++]);

			  free(pMem);
	      	  pMem	 = NULL; 
			  free(mem138);
			  mem138 = NULL;
			}
		}/*end*/

	return envp;
}


/* Call a script with a par file and env vars */
void udhcp_run_script(struct dhcpMessage *packet, const char *name)
{
	int pid;
	char **envp, **curr;

	if (client_config.script == NULL)
		return;

	DEBUG("vfork'ing and execle'ing %s", client_config.script);
	envp = fill_envp(packet);
	printf("vfork'ing and execle'ing %s\n", client_config.script);
	/* call script */
	pid = vfork();
	if (pid) {
		waitpid(pid, NULL, 0);
		for (curr = envp; *curr; curr++) free(*curr);
		free(envp); 
		return;
	} else if (pid == 0) {
		/* close fd's? */
		/* exec script */
		execle(client_config.script, client_config.script,
		       name, NULL, envp);
		bb_perror_msg("script %s failed", client_config.script);
		exit(1);
	}
}
