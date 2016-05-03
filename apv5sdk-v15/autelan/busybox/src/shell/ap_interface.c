
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conf_file_opt.h"
#include "ap_interface.h"

#define MAX_STR_LEN 32
#define MAX_PATH_LEN 128      //512
#define MAX_CMD_LEN  256  //1024

const static char *CONF_FILE = "/etc/config/config.xml";
const static char *LOCK_FILE = "/tmp/apfunc.lock";

const static char *NO_MESSAGE ="" ;//"2>/dev/null 1>/dev/null";
const static char *IFCONFIG = "ifconfig";
const static char *NETMASK = "netmask";
const static char *ROUTEADD_NET = "route add -net";
const static char *ROUTEADD_HOST = "route add -host";
const static char *KILLALL = "killall";
const static char *UDHCPC = "udhcpc";
const static char *NTPCLIENT = "ntpclient";
const static char *HOSTAPD = "hostapd";
const static char *IWCONFIG = "iwconfig";
const static char *IWPRIV = "iwpriv";
const static char *SYSLOGD = "syslogd";

/***************************************************/
/*                   config                        */
/***************************************************/

/*-------------------------------------------------------*
    network [vlanname] ip [ipadress]  netmask  [netmask] 
*--------------------------------------------------------*/
static int conf_netwrok_set_ip( char *str_ifname, char *str_ipaddr, char *str_netmask)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;
	const char *XPATH = "/config/network/lan";
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }

   // if( NULL != pst_node->child )
  //  {
      //  printf("child node name : %s\n", pst_node->child->name );
   // }
    res = setoption( pst_node, "if", str_ifname);
    res += setoption( pst_node, "ipaddr", str_ipaddr );
    res += setoption( pst_node, "netmask", str_netmask );

    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
        destroynode(pst_node);
    	return IFERR_FILE_WRITE;
    }
  
    return IFERR_NO_ERROR;
    
}

static int ap_network_set_ip( char *str_ifname, char *str_ipaddr, char *str_netmask )
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s %s %s", IFCONFIG, str_ifname, str_ipaddr, NETMASK, str_netmask 
	, NO_MESSAGE );
	return (0==system(str_tmp_cmd)) ? IFERR_NO_ERROR: IFERR_COM_EXEC_FAILED;
}

int network_set_ip( char *str_ifname, char *str_ipaddr, char *str_netmask)
{
	int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curipaddr[MAX_STR_LEN]={0};
	if((res=network_get_ip( str_curipaddr, MAX_STR_LEN))==IFERR_NO_ERROR)
	{
	    if(0==strcmp(str_ipaddr,str_curipaddr))
	    {
	        return IFERR_ARG_INVALID;
	    }
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}

	if(flock( fd_mutex, LOCK_EX ) != 0)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}

	if( (res = ap_network_set_ip( str_ifname, str_ipaddr, str_netmask )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}

	res = conf_netwrok_set_ip( str_ifname, str_ipaddr, str_netmask );

	system("/usr/sbin/send_trap apIPChangeAlarm");
	flock( fd_mutex, LOCK_UN );
	close( fd_mutex );
   }
    
	return res;
}
static int conf_network_get_ip( char *str_ipaddr, unsigned int un_size )
{
	int res = IFERR_NO_ERROR;
    char *XPATH = "/config/network/lan";

	res = get_data_by_path( CONF_FILE, XPATH, "ipaddr", str_ipaddr, un_size );
	
	return res;
}

int network_get_ip( char *str_ipaddr, unsigned int un_size )
{
    return conf_network_get_ip( str_ipaddr, un_size );
}

/*---------------------------------------*
    network [vlanname] up/down 
*----------------------------------------*/

static int ap_network_set_ifstate( char *str_ifname, char *str_state )
{
    char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", IFCONFIG, str_ifname, str_state, 
	NO_MESSAGE );
	return system(str_tmp_cmd);
}

int network_set_ifstate( char *str_ifname, char *str_state )
{
    int fd_mutex;
	int res = IFERR_NO_ERROR;
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
    
    res = ap_network_set_ifstate( str_ifname, str_state );

	flock( fd_mutex, LOCK_UN );
	close(fd_mutex);
	return res;

}

/*----------------------------------------*
    network route add default [gateway]
*-----------------------------------------*/

static int ap_network_set_gw( char *str_gateway )
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s default gw %s %s ", ROUTEADD_NET, str_gateway, NO_MESSAGE );
	return system(str_tmp_cmd);
}

static int conf_netwrok_set_gw( char *str_gateway )
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;
	const char *XPATH = "/config/network/lan";
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    res = setoption( pst_node, "gateway", str_gateway);
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


int network_set_gw( char *str_gateway )
{
    int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curgateway[MAX_STR_LEN]={0};
	if(network_get_gw(str_curgateway,30)==IFERR_NO_ERROR)
	{
	    if(strcmp(str_curgateway,str_gateway)==0)
	    {
			return IFERR_ARG_INVALID;
	    }
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_network_set_gw( str_gateway )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}
	
	res = conf_netwrok_set_gw( str_gateway );
	
	flock( fd_mutex, LOCK_UN );
	close( fd_mutex );
	}
	
    return res;
}
static int conf_network_get_gw( char *str_gateway, unsigned int un_size )
{
	int res = IFERR_NO_ERROR;
    char *XPATH = "/config/network/lan";

	res = get_data_by_path( CONF_FILE, XPATH, "gateway", str_gateway, un_size );
	
	return res;
}

int network_get_gw( char *str_gateway, unsigned int un_size )
{
    return conf_network_get_gw( str_gateway, un_size );
}

/*----------------------------------------*
    network config ipmode  static/dhcp
*-----------------------------------------*/

static int ap_network_set_ipmode( char *str_ipmode )
{
	char str_tmp_cmd[MAX_CMD_LEN];
    char *str_ifname = NULL;
    int res = IFERR_NO_ERROR;
    char *XPAHT = "/config/network/lan";
    char *OPT_NAME = "if";
    
    str_ifname = (char *)malloc( MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
    memset( str_ifname, 0, MAX_STR_LEN );

    if( (res=get_data_by_path(CONF_FILE, XPAHT, OPT_NAME, str_ifname, MAX_STR_LEN)) != IFERR_NO_ERROR )
    {
        if( res < 0)
        {
            str_ifname = (char *)realloc( str_ifname, -res );
            get_data_by_path( CONF_FILE, XPAHT, OPT_NAME, str_ifname, -res);
        }
        else
        {
            free( str_ifname );
            return IFERR_ARG_NOT_FOUND;
        }
    }

    if( 0 == strcmp(str_ipmode, "static") )
    {
        //KILLALL udhcpc >/dev/null
        sprintf( str_tmp_cmd, "{ %s udhcpc %s || true;}", KILLALL, NO_MESSAGE );
    }
    else if( 0 == strcmp(str_ipmode, "dhcp") )
    {
        //$UDHCPC -i "$xml_if" -t 3 -b 2>/dev/null
        sprintf( str_tmp_cmd, "{ { %s %s %s || true;} "\
                              "&& %s -i %s -t 3 -b 2  %s; }", 
                              KILLALL, UDHCPC, NO_MESSAGE, 
                              UDHCPC, str_ifname, NO_MESSAGE );
    }
    else
    {
        free( str_ifname );
        return IFERR_ARG_WRONG;
    }
    
    res = system( str_tmp_cmd );
    free( str_ifname );
    return res;
    
}

static int conf_netwrok_set_ipmode( char *str_ipmode )
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;
	const char *XPATH = "/config/network/lan";
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    res = setoption( pst_node, "proto", str_ipmode);
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


int network_set_ipmode( char *str_ipmode )
{
    int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curipmode[MAX_STR_LEN]={0};
	
    if((res=network_get_ipmode( str_curipmode, 10))==IFERR_NO_ERROR)
    {
        if(0==strcmp(str_ipmode,str_curipmode))
        {
            return IFERR_ARG_INVALID;    
        }
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_network_set_ipmode( str_ipmode )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}
	
	res = conf_netwrok_set_ipmode( str_ipmode );
	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
    }
		
	 return res;
}

static int conf_network_get_ipmode( char *str_ipmode, unsigned int un_size )
{
	int res = IFERR_NO_ERROR;
    char *XPATH = "/config/network/lan";

	res = get_data_by_path( CONF_FILE, XPATH, "proto", str_ipmode, un_size );
	
	return res;
}
int network_get_ipmode( char *str_ipmode, unsigned int un_size )
{
    return conf_network_get_ipmode( str_ipmode, un_size );
}

/*----------------------------------------*
    network config dnsmode dhcp/static
*-----------------------------------------*/

static int conf_netwrok_set_dnsmode( char *str_dnsmode )
{
    MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;
	const char *XPATH = "/config/network/lan";
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    res = setoption( pst_node, "nameserver_config", str_dnsmode);
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
}

int network_set_dnsmode( char *str_dnsmode )
{
	int res = IFERR_NO_ERROR;
	char str_curdnsmode[MAX_STR_LEN]={0};
	
    if((res=network_get_dnsmode( str_curdnsmode, 10))==IFERR_NO_ERROR)
    {
        if(0==strcmp(str_dnsmode,str_curdnsmode))
        {
            return IFERR_ARG_INVALID;    
        }
	
	    res = conf_netwrok_set_dnsmode( str_dnsmode );
    }
	return res;
}

static int conf_network_get_dnsmode( char *str_dnsmode, unsigned int un_size )
{
	int res = IFERR_NO_ERROR;
    char *XPATH = "/config/network/lan";

	res = get_data_by_path( CONF_FILE, XPATH, "nameserver_config", str_dnsmode, un_size );
	
	return res;
}

int network_get_dnsmode( char *str_dnsmode, unsigned int un_size )
{
    return conf_network_get_dnsmode( str_dnsmode, un_size );
}

/*-----------------------------------------------*
    network config dnsnameserver[n] [ipaddress]
*------------------------------------------------*/

static int ap_network_set_dns( char *str_ipaddr, unsigned int un_seq )
{
    int res = IFERR_NO_ERROR;
	char str_tmp_cmd[MAX_CMD_LEN];
    char str_tmp[MAX_STR_LEN];
    char str_dns_servers[MAX_DNS_SERV][MAX_STR_LEN];
    char *XPATH = "/config/network/lan";
    int i = 0;

    memset( str_tmp_cmd, 0, MAX_CMD_LEN );
    memset( str_tmp, 0, MAX_STR_LEN);
    memset( str_dns_servers, 0, MAX_DNS_SERV*MAX_STR_LEN );

    for( i=0; i<MAX_DNS_SERV; i++ )
    {
        memset( str_tmp, 0, MAX_STR_LEN );
        sprintf( str_tmp, "nameserver%d", i+1 );
        res += get_data_by_path( CONF_FILE, XPATH, str_tmp,  str_dns_servers[i], MAX_STR_LEN );
    }
    strcpy( str_dns_servers[un_seq-1], str_ipaddr );
    
    if( res%IFERR_ARG_NO_DATA != 0 )
    {
        return IFERR_ARG_WRONG;
    }

    sprintf( str_tmp_cmd, "echo \" \" > /etc/resolv.conf %s", NO_MESSAGE);
    res = system( str_tmp_cmd );
    if( IFERR_NO_ERROR == res)
    {
        for( i=0; i<MAX_DNS_SERV; i++ )
        {  
            if( str_dns_servers[i][0] != 0 )
            {
                sprintf( str_tmp_cmd, "echo \"nameserver %s\" >> /etc/resolv.conf 2>/dev/null", 
                                      str_dns_servers[i]);
                res += system( str_tmp_cmd );
            }
    }
    }
    if( res != IFERR_NO_ERROR )
    {
        res = IFERR_FILE_WRITE;
    }
	return res;
}

static int conf_netwrok_set_dns( char *str_ipaddr, unsigned int un_seq )
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;
	const char *XPATH = "/config/network/lan";
    char str_arg_name[MAX_STR_LEN];
    memset( str_arg_name, 0, MAX_STR_LEN );

    sprintf( str_arg_name, "nameserver%d", un_seq );
    
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    res = setoption( pst_node, str_arg_name, str_ipaddr);
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


int network_set_dns( char *str_ipaddr , unsigned int un_seq )
{
    int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curipaddr[MAX_STR_LEN]={0};
	if((network_get_dns(str_curipaddr,un_seq,30))==IFERR_NO_ERROR)
	{
	    if(0==strcmp(str_ipaddr,str_curipaddr))
	    {
	       return IFERR_ARG_INVALID;
	    }
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_network_set_dns( str_ipaddr, un_seq )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}
	
	res = conf_netwrok_set_dns( str_ipaddr, un_seq );

	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
    }
	
	 return res;
}

/*-----------------------------------------------*
          network show dnsnameserver 
*------------------------------------------------*/

int network_get_dns( char *str_ipaddr , unsigned int un_seq , 
                     unsigned int un_size )
{
	int res = IFERR_NO_ERROR;
    char *XPATH = "/config/network/lan";
    char str_tmp[MAX_STR_LEN];

    memset( str_tmp, 0, MAX_STR_LEN );
    sprintf( str_tmp, "nameserver%d", un_seq );
   
	res = get_data_by_path( CONF_FILE, XPATH, str_tmp, str_ipaddr, un_size );
	
	return res;
}

/*---------------------------------------------*
          network show ntp server 
*----------------------------------------------*/

int network_get_timezone( char *str_timezone , unsigned int un_size)
{
	int res = IFERR_NO_ERROR;
    char *XPATH = "/config/system";
    
	res = get_data_by_path( CONF_FILE, XPATH, "timezone", str_timezone, un_size );
	
	return res;
}

int network_get_defaultntp( char *str_defaultntp , unsigned int un_size)
{
	int res = IFERR_NO_ERROR;
    char *XPATH = "/config/system";
    
	res = get_data_by_path( CONF_FILE, XPATH, "timeserverdefault", str_defaultntp, un_size );
	
	return res;
}



int network_get_ntptime(void )
{
   	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );

	sprintf( str_tmp_cmd, "%s", "/bin/date -R");
	return system(str_tmp_cmd);
}


/*-------------------------------------------*
          network config ntp server
*--------------------------------------------*/

static int ap_network_set_ntp( char *str_ipaddr )
{
    char str_tmp_cmd[MAX_CMD_LEN];
    char str_ntp_def[MAX_STR_LEN];
    const char *XPATH = "/config/system";
    const char *TAG_NAME = "timeserverdefault";

    memset( str_tmp_cmd, 0, MAX_CMD_LEN );
    memset( str_ntp_def, 0, MAX_STR_LEN );

    sprintf( str_tmp_cmd, "%s -c 3 -i 1 -h \"%s\" -s %s ", 
                          NTPCLIENT, str_ipaddr, NO_MESSAGE );
    if( 0 != system(str_tmp_cmd) )
    {
        if( IFERR_NO_ERROR == get_data_by_path(CONF_FILE, XPATH, TAG_NAME, 
                                                str_ntp_def, MAX_STR_LEN) )
        {
            memset( str_tmp_cmd, 0, MAX_CMD_LEN );
            sprintf( str_tmp_cmd, "%s -c 3 -i 1 -h \"%s\" -s %s ", 
                                  NTPCLIENT, str_ntp_def, NO_MESSAGE );
            system( str_tmp_cmd );
        }
        return IFERR_COM_EXEC_FAILED;
    }

    return IFERR_NO_ERROR;
}

static int conf_netwrok_set_ntp( char *str_ipaddr, unsigned int un_seq )
{
    MXML_NODE* pst_node = NULL;
    int res = IFERR_NO_ERROR;
    const char *XPATH = "/config/system";
    char str_arg_name[MAX_STR_LEN];
    memset( str_arg_name, 0, MAX_STR_LEN );

    sprintf( str_arg_name, "timeserver%d", un_seq );
    
    if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
    {
        return IFERR_ARG_NOT_FOUND;
    }
    
    res = setoption( pst_node, str_arg_name, str_ipaddr );
    
    if( res != IFERR_NO_ERROR )
    {
        destroynode(pst_node);
        return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
            destroynode(pst_node);
            return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


int network_set_ntp( char *str_ipaddr , unsigned int un_seq )
{
    int fd_mutex;
    int res = IFERR_NO_ERROR;
	char str_curipaddr[MAX_STR_LEN]={0};
	if((network_get_ntp(str_curipaddr,un_seq,30))==IFERR_NO_ERROR)
	{   
	    if(strcmp(str_curipaddr,str_ipaddr)==0)
	    {
			return IFERR_ARG_INVALID;
	    }
    if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
    {
        return IFERR_FILE_NOT_FOUND;
    }
    if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
    {
        close( fd_mutex );
        return IFERR_LOCK_FAILED;
    }
    if( (res = ap_network_set_ntp( str_ipaddr )) != IFERR_NO_ERROR )
    {
        flock( fd_mutex, LOCK_UN );
        close(fd_mutex);
        return res;
    }
    
    res = conf_netwrok_set_ntp( str_ipaddr, un_seq );
   
    flock( fd_mutex, LOCK_UN );
    close( fd_mutex );
	}
    
    return res;
}
int network_get_ntp( char *str_ipaddr , unsigned int un_seq , unsigned int un_size)
{
	int res = IFERR_NO_ERROR;
    char *XPATH = "/config/system";
    char str_tmp[MAX_STR_LEN];

    memset( str_tmp, 0, MAX_STR_LEN );
    sprintf( str_tmp, "timeserver%d", un_seq );
    
	res = get_data_by_path( CONF_FILE, XPATH, str_tmp, str_ipaddr, un_size );
	
	return res;
}


/*-------------------------------------------*
           network config ntp timezone
*--------------------------------------------*/



/*---------------------------------------------------------*
                network config ntpmode yes/no
            
 note: if use default mode , this function will 
       change to default address automaticly ; while
       use user mode , you should call network_set_ntp
       to set ntp server address yourself 
*----------------------------------------------------------*/

static int ap_network_set_ntpmode( char *str_state )
{
    char str_tmp_cmd[MAX_CMD_LEN];
    char str_ntp_addr[MAX_STR_LEN];
    char *XPATH = "/config/system";
    char *TAG_NAME = "timeserverdefault";

    memset( str_tmp_cmd, 0, MAX_CMD_LEN );
    memset( str_ntp_addr, 0, MAX_STR_LEN );

    if( 0 == strcmp(str_state, "yes") )
    {
        if( XERR_NO_ERROR == get_data_by_path(CONF_FILE,
                                              XPATH,
                                              TAG_NAME, 
                                              str_ntp_addr,
                                              MAX_STR_LEN))
        {
            memset( str_tmp_cmd, 0, MAX_CMD_LEN );
            sprintf( str_tmp_cmd, "%s -c 3 -i 1 -h \"%s\" -s %s ", 
                              NTPCLIENT, str_ntp_addr, NO_MESSAGE );
            if( IFERR_NO_ERROR == system(str_tmp_cmd))
            {
                return IFERR_NO_ERROR;
            }
            return IFERR_COM_EXEC_FAILED;
        }
    }

    return IFERR_NO_ERROR;
}

static int conf_netwrok_set_ntpmode( char *str_state )
{
    MXML_NODE* pst_node = NULL;
    int res = IFERR_NO_ERROR;
    const char *XPATH = "/config/system";
    char str_arg_name[MAX_STR_LEN];
    
    memset( str_arg_name, 0, MAX_STR_LEN );
    sprintf( str_arg_name, "timeserver_usedefault" );
    
    if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
    {
        return IFERR_ARG_NOT_FOUND;
    }
    
    res = setoption( pst_node, str_arg_name, str_state );
    
    if( res != IFERR_NO_ERROR )
    {
        destroynode(pst_node);
        return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
        destroynode(pst_node);
        return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

int network_set_ntpmode( char *str_state )
{
    int fd_mutex;
    int res = IFERR_NO_ERROR;
	char str_curstate[MAX_STR_LEN]={0};
	if(network_get_ntpmode(str_curstate,10)==IFERR_NO_ERROR)
	{ 
	    if(strcmp(str_curstate,str_state)==0)
	    {  
	        return IFERR_ARG_INVALID;
	    }
    if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
    {
        return IFERR_FILE_NOT_FOUND;
    }
    if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
    {
        close( fd_mutex );
        return IFERR_LOCK_FAILED;
    }
    if( (res = ap_network_set_ntpmode( str_state )) != IFERR_NO_ERROR )
    {
        flock( fd_mutex, LOCK_UN );
        close(fd_mutex);
        return res;
    }
    
    res = conf_netwrok_set_ntpmode( str_state );
    
     flock( fd_mutex, LOCK_UN );
     close( fd_mutex );
	}
     return res;
}

int network_get_ntpmode( char *str_ntpmode , unsigned int un_size)
{
	int res = IFERR_NO_ERROR;
    char *XPATH = "/config/system";
    
	res = get_data_by_path( CONF_FILE, XPATH, "timeserver_usedefault", str_ntpmode, un_size );
	
	return res;
}

/*********************************************/
/*                wireless                   */
/*********************************************/

static int conf_wireless_create_vap( char *str_wifi, char *str_ifname)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50];                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%c","/config/network/vap",str_ifname[3]+1);
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    res = setoption( pst_node, "if", str_ifname);
    res += setoption( pst_node, "enable", "yes" );
	res += setoption( pst_node, "wifi", str_wifi );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int conf_wireless_destroy_vap( char *str_ifname)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%c","/config/network/vap",str_ifname[3]+1);
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    res = setoption( pst_node, "if", str_ifname);
    res += setoption( pst_node, "enable", "no" );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


static int conf_wireless_addif_vlan( char *str_ifname, char *str_vlanname)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%c%s","/config/network/vap",str_ifname[3]+1,"/vlan");
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    if(strcmp(str_vlanname,"br-lan")==0)
	{
		 res += setoption( pst_node, "enable", "0" );

	}
	else
	{
         res += setoption( pst_node, "enable", "1" );
		 res += setoption( pst_node, "vid", strpbrk(str_vlanname,"12345678") );
	}
   
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


/*************************************/
/************wireless enable***********/
/*************************************/

static int conf_wireless_config_enable( unsigned int un_vap_num, char *str_enable)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;
	char XPATH[50] ;
	char str_curssid[MAX_STR_LEN]={0};

	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    res = setoption( pst_node, "enable", str_enable );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

int wireless_config_enable(unsigned int un_vap_num, char *str_enable )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curenable[MAX_STR_LEN]={0};
	if(wireless_get_enable(un_vap_num,str_curenable,MAX_STR_LEN)==IFERR_NO_ERROR)
	{
        if(strcmp(str_curenable,str_enable)==0)
        {
           return IFERR_ARG_INVALID;
        }
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		
		res = conf_wireless_config_enable(  un_vap_num, str_enable  );

		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}	
	 return res;
}



static int conf_wireless_get_enable( unsigned int un_vap_num,char *str_enable, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "enable",
                             str_enable , un_size );

}

int wireless_get_enable( unsigned int un_vap_num,char *str_enable, unsigned int un_size)
{
    return conf_wireless_get_enable( un_vap_num, str_enable,un_size);
}


/*************************************/
/************wireless status***********/
/*************************************/

int wireless_get_status( unsigned int un_vap_num,char *str_status, unsigned int un_size)
{
  	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	char szVal[50]={0};
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );

	FILE   *stream; 
    memset(szVal, 0x00, sizeof(szVal));	
	sprintf(str_tmp_cmd,"{ /sbin/ifconfig %s | grep 'UP' ; echo $? ; }",str_vap_if);
	stream = popen( str_tmp_cmd, "r" );
	fread( szVal, sizeof(char), sizeof(szVal),  stream); 
	fclose( stream );
    szVal[strlen(szVal)-1]='\0';
    if(0==atoi(szVal))
	    strcpy(str_status,"Up");
	else if(1==atoi(szVal)) 	
        strcpy(str_status,"Disable");
	else
		return IFERR_ARG_INVALID;
        
	return IFERR_NO_ERROR;

}
/*************************************/
/************wireless ssid***********/
/*************************************/
static int conf_wireless_config_ssid( unsigned int un_vap_num, char *str_ssid)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;
	char XPATH[50] ;
	char str_curssid[MAX_STR_LEN]={0};

	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "ssid", str_ssid );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int ap_wireless_config_ssid( unsigned int un_vap_num, char *str_ssid )
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwconfig",  str_vap_if, "essid", str_ssid );
	return system(str_tmp_cmd);
}

int wireless_config_ssid(unsigned int un_vap_num, char *str_ssid )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curssid[MAX_STR_LEN]={0};
	if(wireless_get_ssid(un_vap_num,str_curssid,MAX_STR_LEN)==IFERR_NO_ERROR)
	{
        if(strcmp(str_curssid,str_ssid)==0)
        {
           return IFERR_ARG_INVALID;
        }
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_ssid(  un_vap_num, str_ssid )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_ssid(  un_vap_num, str_ssid  );

		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}	
	 return res;
}


static int conf_wireless_get_ssid( unsigned int un_vap_num,char *str_ssid, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "ssid",
                             str_ssid , un_size );

}

int wireless_get_ssid( unsigned int un_vap_num,char *str_ssid, unsigned int un_size)
{
    return conf_wireless_get_ssid( un_vap_num, str_ssid,un_size);
}


int wireless_get_bssid( unsigned int un_vap_num,char *str_bssid, unsigned int un_size)
{
	FILE   *stream; 
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
	char str_tmp_path[MAX_PATH_LEN];
	sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",str_vap_if, MAX_STR_LEN );
	sprintf(str_tmp_cmd,"iwconfig %s |grep -o -i -e '[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}'",str_vap_if);
	stream = popen( str_tmp_cmd, "r" );
	fread( str_bssid, sizeof(char), 100,stream); 
	fclose( stream );
	str_bssid[strlen(str_bssid)-1]='\0';
    return 0;
}

/***************************************/
/************wireless channel***********/
/***************************************/
static int conf_wireless_config_channel( unsigned int un_vap_num, char *str_channel)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "channel", str_channel );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


static int ap_wireless_config_channel( unsigned int un_vap_num, char *str_channel )
{
	char str_tmp_cmd[MAX_CMD_LEN];
    char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwconfig", str_vap_if, "channel", str_channel );
	return system(str_tmp_cmd);
}
int wireless_config_channel( unsigned int un_vap_num, char *str_channel )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curchannel[MAX_STR_LEN]={0};
	if((res=wireless_get_channel( un_vap_num,str_curchannel,MAX_STR_LEN))==IFERR_NO_ERROR)
	{
	    if(strcmp(str_curchannel,str_channel)==0)
        {
           return IFERR_ARG_INVALID;
        }
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_channel(  un_vap_num, str_channel )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_channel( un_vap_num, str_channel  );
		system("/usr/sbin/send_trap apChannelConfigChange");
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}
	
	 return res;
}
static int conf_wireless_get_channel( unsigned int un_vap_num,char *str_channel, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "channel",
                             str_channel , un_size );

}

int wireless_get_channel( unsigned int un_vap_num,char *str_channel, unsigned int un_size)
{
    return conf_wireless_get_channel( un_vap_num, str_channel,un_size);
}

/***************************************/
/************wireless region***********/
/***************************************/
static int conf_wireless_get_region( unsigned int un_vap_num,char *str_region, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "region",
                             str_region , un_size );

}

int wireless_get_region( unsigned int un_vap_num,char *str_region, unsigned int un_size)
{
    return conf_wireless_get_region( un_vap_num, str_region,un_size);
}

/***************************************/
/************wireless mode***********/
/***************************************/

static int conf_wireless_config_mode( unsigned int un_vap_num, char *str_mode)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
    sprintf( XPATH, "/config/network/vap%d", un_vap_num );
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "mode", str_mode );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}
/*****************************************************
before we change the mode ,we must change the channel 0
*****************************************************/

static int ap_wireless_config_mode( unsigned int un_vap_num, char *str_mode )
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	char tmp[10]={0};
	wireless_get_channel(un_vap_num,tmp,10);
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf(str_tmp_cmd,"iwconfig %s channel 0",str_vap_if);
	system(str_tmp_cmd);
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv", str_vap_if, "mode", str_mode );
    system(str_tmp_cmd);
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf(str_tmp_cmd,"iwconfig %s channel %s",str_vap_if,tmp);
	return system(str_tmp_cmd);		
}
int wireless_config_mode( unsigned int un_vap_num, char *str_mode )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curmode[MAX_STR_LEN]={0};
	if((res=wireless_get_mode(un_vap_num,str_curmode,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{

		if(strcmp(str_curmode,str_mode) == 0)
        {
           return IFERR_ARG_INVALID;
        }	
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_mode(  un_vap_num, str_mode )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_mode(  un_vap_num, str_mode);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
		
		 return res;
	}
}
static int conf_wireless_get_mode( unsigned int un_vap_num,char *str_mode, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "mode",
                             str_mode , un_size );

}

int wireless_get_mode( unsigned int un_vap_num,char *str_mode, unsigned int un_size)
{
    return conf_wireless_get_mode( un_vap_num, str_mode,un_size);
}

/***************************************/
/************wireless hidessid***********/
/***************************************/
static int conf_wireless_config_hidessid( unsigned int un_vap_num, char *str_hidessid)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
    sprintf( XPATH, "/config/network/vap%d", un_vap_num );
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
	if(atoi(str_hidessid) == 0)
        res = setoption( pst_node, "broadcast", "yes" );
    else
       res = setoption( pst_node, "broadcast", "no" );
	
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


static int ap_wireless_config_hidessid( unsigned int un_vap_num, char *str_hidessid )
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv", str_vap_if, "hide_ssid", str_hidessid );
	return system(str_tmp_cmd);
}


int wireless_config_hidessid( unsigned int un_vap_num, char *str_hidessid )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curhidessid[MAX_STR_LEN]={0};

	printf("");
    if((res=wireless_get_broadcastssid(un_vap_num,str_curhidessid,MAX_STR_LEN)) == IFERR_NO_ERROR)
    {
        if(((strstr(str_hidessid,"1")!=NULL)&&(strstr(str_curhidessid,"no")!=NULL))
		  ||((strstr(str_hidessid,"0")!=NULL)&&(strstr(str_curhidessid,"yes")!=NULL))
		  )
        {
	        return IFERR_ARG_INVALID;
        }
	 	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	 	{
	 		return IFERR_FILE_NOT_FOUND;
	 	}
	 	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	 	{
	 		close( fd_mutex );
	 		return IFERR_LOCK_FAILED;
	 	}
	 	if( (res = ap_wireless_config_hidessid( un_vap_num, str_hidessid )) != IFERR_NO_ERROR )
	 	{
	 	    flock( fd_mutex, LOCK_UN );
	 	    close(fd_mutex);
	 	    return res;
	 	}	
	 	
	 	res = conf_wireless_config_hidessid( un_vap_num, str_hidessid);
	 	
	 	 flock( fd_mutex, LOCK_UN );
	 	 close( fd_mutex );
    }
 	
 	 return res;
}
static int conf_wireless_get_broadcastssid( unsigned int un_vap_num,char *str_broadcastssid, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "broadcast",
                             str_broadcastssid , un_size );

}

int wireless_get_broadcastssid( unsigned int un_vap_num,char *str_broadcastssid, unsigned int un_size)
{
    return conf_wireless_get_broadcastssid( un_vap_num, str_broadcastssid,un_size);
}

/***************************************/
/************wireless rate***************/
/***************************************/
static int conf_wireless_config_rate( unsigned int un_vap_num, char *str_rate)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	 sprintf( XPATH, "/config/network/vap%d", un_vap_num );
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "rate", str_rate );
	
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


static int ap_wireless_config_rate( unsigned int un_vap_num, char *str_rate )
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwconfig", str_vap_if, "rate", str_rate);
	return system(str_tmp_cmd);
}

int wireless_config_rate( unsigned int un_vap_num, char *str_rate )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_currate[MAX_STR_LEN]={0};
	
    if((res=wireless_get_rate(un_vap_num,str_currate,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{

		if(strcmp(str_currate,str_rate) == 0)
        {
           return IFERR_ARG_INVALID;
        }
	
	 	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	 	{
	 		return IFERR_FILE_NOT_FOUND;
	 	}
	 	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	 	{
	 		close( fd_mutex );
	 		return IFERR_LOCK_FAILED;
	 	}
	 	if( (res = ap_wireless_config_rate( un_vap_num, str_rate )) != IFERR_NO_ERROR )
	 	{
	 	    flock( fd_mutex, LOCK_UN );
	 	    close(fd_mutex);
	 	    return res;
	 	}	
	 	
	 	res = conf_wireless_config_rate( un_vap_num, str_rate);
	 	
	 	 flock( fd_mutex, LOCK_UN );
	 	 close( fd_mutex );
    }
	
	 return res;
}


static int conf_wireless_get_rate( unsigned int un_vap_num,char *str_rate, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "rate",
                             str_rate , un_size );

}

int wireless_get_rate( unsigned int un_vap_num,char *str_rate, unsigned int un_size)
{
    return conf_wireless_get_rate( un_vap_num, str_rate, un_size);
}


/*********************************************/
/************wireless bintval*****************/
/*********************************************/

static int conf_wireless_config_bintval(unsigned int un_vap_num, char *str_bintval)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "beacon", str_bintval );
	
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


static int ap_wireless_config_bintval( unsigned int un_vap_num, char *str_bintval)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv", str_vap_if, "bintval", str_bintval);
	return system(str_tmp_cmd);
}


int wireless_config_bintval( unsigned int un_vap_num, char *str_bintval )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curbintval[MAX_STR_LEN]={0};

	if((res=wireless_get_bintval(un_vap_num,str_curbintval,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{

		if(strcmp(str_curbintval,str_bintval) == 0)
        {
           return IFERR_ARG_INVALID;
        }
	
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_bintval( un_vap_num, str_bintval )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_bintval( un_vap_num, str_bintval);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}
	
	 return res;
}

static int conf_wireless_get_bintval( unsigned int un_vap_num,char *str_bintval, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "beacon",
                             str_bintval , un_size );

}

int wireless_get_bintval( unsigned int un_vap_num,char *str_bintval, unsigned int un_size)
{
    return conf_wireless_get_bintval( un_vap_num, str_bintval, un_size);
}
static int conf_wireless_config_dtim( unsigned int un_vap_num, char *str_dtim)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "dtim", str_dtim );
	
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}
static int ap_wireless_config_dtim( unsigned int un_vap_num, char *str_dtim)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv", str_vap_if, "dtim_period", str_dtim);
	return system(str_tmp_cmd);
}


int wireless_config_dtim( unsigned int un_vap_num, char *str_dtim )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curdtim[MAX_STR_LEN]={0};

	if((res=wireless_get_dtim(un_vap_num,str_curdtim,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
		if(strcmp(str_curdtim,str_dtim) == 0)
        {
           return IFERR_ARG_INVALID;
        }
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_dtim( un_vap_num, str_dtim )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_dtim( un_vap_num, str_dtim);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
   }
	
	 return res;
}

static int conf_wireless_get_dtim( unsigned int un_vap_num,char *str_dtim, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "dtim",
                             str_dtim , un_size );

}

int wireless_get_dtim( unsigned int un_vap_num,char *str_dtim, unsigned int un_size)
{
    return conf_wireless_get_dtim( un_vap_num, str_dtim, un_size);
}
/*********************************************/
/************wireless frag*****************/
/*********************************************/

static int conf_wireless_config_frag(unsigned int un_vap_num, char *str_frag)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "frag", str_frag );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


static int ap_wireless_config_frag( unsigned int un_vap_num, char *str_frag)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwconfig", str_vap_if, "frag", str_frag);
	return system(str_tmp_cmd);
}


int wireless_config_frag( unsigned int un_vap_num, char *str_frag)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curfrag[MAX_STR_LEN]={0};

	if((res=wireless_get_frag(un_vap_num,str_curfrag,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{

		if(strcmp(str_curfrag,str_frag) == 0)
        {
           return IFERR_ARG_INVALID;
        }
	
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_frag( un_vap_num, str_frag )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_frag( un_vap_num, str_frag);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}
	
	 return res;
}

static int conf_wireless_get_frag( unsigned int un_vap_num,char *str_frag, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "frag",
                             str_frag , un_size );

}

int wireless_get_frag( unsigned int un_vap_num,char *str_frag, unsigned int un_size)
{
    return conf_wireless_get_frag( un_vap_num, str_frag, un_size);
}
static int conf_wireless_config_txpower( unsigned int un_vap_num, char *str_txpower)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;
	char str_wifi[10]={0};
	char XPATH[50]={0} ;  
	
	sprintf( XPATH, "/config/network/vap%d", un_vap_num );

	get_data_by_path( CONF_FILE, XPATH, "wifi", 
								str_wifi, 10 );

	memset(XPATH, 0,sizeof(XPATH));
	sprintf(XPATH,"%s%s","/config/network/",str_wifi);

	/*** the ap have many vap****/
    
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "txpower", str_txpower );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int ap_wireless_config_txpower( unsigned int un_vap_num, char *str_txpower)
{
	char str_tmp_cmd[MAX_CMD_LEN];
    char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwconfig", str_vap_if, "txpower", str_txpower);
	return system(str_tmp_cmd);
}


int wireless_config_txpower( unsigned int un_vap_num, char *str_txpower)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curtxpower[MAX_STR_LEN]={0};
	if((res=wireless_get_txpower(un_vap_num,str_curtxpower,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
	    if(strcmp(str_curtxpower,str_txpower) == 0)
	    {
			return IFERR_ARG_INVALID;
	    }
	
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_txpower( un_vap_num, str_txpower )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_txpower( un_vap_num, str_txpower);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}
	
	 return res;
}

static int conf_wireless_get_txpower( unsigned int un_vap_num,char *str_txpower, unsigned int un_size)
{
    char XPATH[MAX_PATH_LEN]={0};
	char str_wifi[10]={0};

	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, XPATH, "wifi", str_wifi, 10 );							
	memset(XPATH, 0,sizeof(XPATH));
	sprintf(XPATH,"%s%s","/config/network/",str_wifi);
    return get_data_by_path( CONF_FILE, XPATH, "txpower",
                             str_txpower , un_size );

}

int wireless_get_txpower( unsigned int un_vap_num,char *str_txpower, unsigned int un_size)
{
    return conf_wireless_get_txpower( un_vap_num, str_txpower, un_size);
}
/*********************************************/
/************wireless rts*****************/
/*********************************************/
static int conf_wireless_config_rts( unsigned int un_vap_num, char *str_rts)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "rts", str_rts );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int ap_wireless_config_rts( unsigned int un_vap_num, char *str_rts)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwconfig", str_vap_if, "rts", str_rts);
	return system(str_tmp_cmd);
}


int wireless_config_rts( unsigned int un_vap_num, char *str_rts)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_currts[MAX_STR_LEN]={0};
	if((res=wireless_get_rts(un_vap_num,str_currts,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{

		if(strcmp(str_currts,str_rts) == 0)
        {
           return IFERR_ARG_INVALID;
        }
	
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_rts( un_vap_num, str_rts )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_rts( un_vap_num, str_rts);

		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}
	 return res;
}

static int conf_wireless_get_rts( unsigned int un_vap_num,char *str_rts, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "rts",
                             str_rts , un_size );

}

int wireless_get_rts( unsigned int un_vap_num,char *str_rts, unsigned int un_size)
{
    return conf_wireless_get_rts( un_vap_num, str_rts, un_size);
}
/*********************************************/
/************wireless wmm**********/
/*********************************************/
static int conf_wireless_config_wmm( unsigned int un_vap_num, char *str_wmm)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%d%s","/config/network/vap",un_vap_num,"/wmm");
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "enable", str_wmm );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int ap_wireless_config_wmm( unsigned int un_vap_num, char *str_wmm)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv", str_vap_if, "wmm", str_wmm);
	return system(str_tmp_cmd);
}


int wireless_config_wmm( unsigned int un_vap_num, char *str_wmm)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curwmm[MAX_STR_LEN]={0};
	if((res=wireless_get_wmm( un_vap_num,str_curwmm,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
        if(strcmp(str_curwmm,str_wmm) == 0)
        {
		   return IFERR_ARG_INVALID;
        }
 		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_wmm( un_vap_num, str_wmm )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_wmm( un_vap_num, str_wmm);

		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}	
	 return res;
}

static int conf_wireless_get_wmm( unsigned int un_vap_num,char *str_wmm, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d/wmm", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "enable",
                             str_wmm, un_size );

}

int wireless_get_wmm( unsigned int un_vap_num,char *str_wmm, unsigned int un_size)
{
    return conf_wireless_get_wmm( un_vap_num, str_wmm, un_size);
}

static int conf_wireless_config_noack( unsigned int un_vap_num, char *str_noack)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%d%s","/config/network/vap",un_vap_num,"/wmm");
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    
    //res = setoption( pst_node, "if", str_ifname);
    res = setoption( pst_node, "noack", str_noack );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int ap_wireless_config_noack( unsigned int un_vap_num, char *str_noack)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv", str_vap_if, "noackpolicy 0 0", str_noack);
	return system(str_tmp_cmd);
}

int wireless_config_noack(unsigned int un_vap_num, char *str_noack)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char *str_curnoack[MAX_STR_LEN]={0};
	if((res=wireless_get_noack( un_vap_num, str_curnoack, MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
	   if(strcmp(str_curnoack,str_noack) == 0)
	   {
		   return IFERR_ARG_INVALID;
	   }
	
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_noack( un_vap_num, str_noack )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_noack( un_vap_num, str_noack);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}	
	 return res;
}

static int conf_wireless_get_noack( unsigned int un_vap_num,char *str_noack, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d/wmm", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "noack",
                             str_noack, un_size );

}

int wireless_get_noack( unsigned int un_vap_num,char *str_noack, unsigned int un_size)
{
    return conf_wireless_get_noack( un_vap_num, str_noack, un_size);
}
static int conf_wireless_config_isolation(unsigned int un_vap_num, char *str_isolation)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    res = setoption( pst_node, "isolation", str_isolation);
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


static int ap_wireless_config_isolation( unsigned int un_vap_num, char *str_isolation)
{

   	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );	
    sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv", str_vap_if, "ap_bridge", str_isolation);
	return system(str_tmp_cmd);
}


int wireless_config_isolation( unsigned int un_vap_num, char *str_isolation)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curisolation[MAX_STR_LEN]={0};
	if((res=wireless_get_isolation(un_vap_num,str_curisolation,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{

		if(strcmp(str_curisolation,str_isolation) == 0)
        {
           return IFERR_ARG_INVALID;
        }
	
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_isolation(  un_vap_num, str_isolation )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_isolation(  un_vap_num, str_isolation);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}	
	 return res;
}

static int conf_wireless_get_isolation( unsigned int un_vap_num,char *str_isolation, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "isolation",
                             str_isolation , un_size );

}

int wireless_get_isolation( unsigned int un_vap_num,char *str_isolation, unsigned int un_size)
{
    return conf_wireless_get_isolation( un_vap_num, str_isolation, un_size);
}
/*********************************************/
/************wireless maxsimultusers**********/
/*********************************************/

static int conf_wireless_config_maxsimultusers( unsigned int un_vap_num, char *str_maxsimultusers)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf( XPATH, "/config/network/vap%d", un_vap_num );
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
    res = setoption( pst_node, "count", str_maxsimultusers);
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int ap_wireless_config_maxsimultusers( unsigned int un_vap_num, char *str_maxsimultusers)
{
   	char str_tmp_cmd[MAX_CMD_LEN];
    char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );	
    sprintf( str_tmp_cmd, "%s%s%s%s", "/sbin/sysctl -w net.", str_vap_if, ".maxaid=", str_maxsimultusers);
	return system(str_tmp_cmd);
}


int wireless_config_maxsimultusers( unsigned int un_vap_num, char *str_maxsimultusers)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curmaxsimultusers[MAX_STR_LEN]={0};
	if((res=wireless_get_maxsimultusers(un_vap_num,str_curmaxsimultusers,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{

		if(strcmp(str_curmaxsimultusers,str_maxsimultusers) == 0)
        {
           return IFERR_ARG_INVALID;
        }
	
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_config_maxsimultusers( un_vap_num, str_maxsimultusers )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_config_maxsimultusers( un_vap_num, str_maxsimultusers);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}
	
	 return res;
}


static int conf_wireless_get_maxsimultusers( unsigned int un_vap_num,char *str_maxsimultusers, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "count",
                             str_maxsimultusers , un_size );

}

int wireless_get_maxsimultusers( unsigned int un_vap_num,char *str_maxsimultusers, unsigned int un_size)
{
    return conf_wireless_get_maxsimultusers( un_vap_num, str_maxsimultusers,un_size);
}


/***************************************************
                       stp
****************************************************/



static int conf_vlan_create( char *str_vlanname)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%s","/config/network/vlan",strpbrk(str_vlanname,"12345678"));
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }


	res = setoption( pst_node, "create", "yes" );

    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int conf_vlan_destroy( char *str_vlanname)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%s","/config/network/vlan",strpbrk(str_vlanname,"12345678"));
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }


	res = setoption( pst_node, "create", "no" );

    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int conf_vlan_addport( char *str_vlanname,char *str_portname)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50];     
    char tmp[50];
    /*** the ap have many vap****/
	sprintf(XPATH,"%s%s","/config/network/vlan",strpbrk(str_vlanname,"12345678"));
	sprintf(tmp,"%s%s","vlan",strpbrk(str_vlanname,"12345678"));
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }

    res = setoption( pst_node, "create", "yes");
    res += setoption( pst_node, "vid", strpbrk(str_vlanname,"12345678") );
    res += setoption( pst_node, "vlanname", tmp );
	res = setoption( pst_node, "ifeth", "yes" );

    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


static int conf_vlan_delport( char *str_vlanname,char *str_portname)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50];  
    char tmp[50];
    /*** the ap have many vap****/
	sprintf(XPATH,"%s%s","/config/network/vlan",strpbrk(str_vlanname,"12345678"));
	sprintf(tmp,"%s%s","vlan",strpbrk(str_vlanname,"12345678"));
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }

    res = setoption( pst_node, "create", "no");
    res += setoption( pst_node, "vid", strpbrk(str_vlanname,"12345678") );
    res += setoption( pst_node, "vlanname", tmp );
	res = setoption( pst_node, "ifeth", "no" );

    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

/***************************************************
*****************macfileter***************************
****************************************************/


static int conf_wireless_security_filter_delmac( unsigned int un_vap_num,char *str_macnum)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50];                                        /*** the ap have many vap****/
    char MACMUM[20];
	sprintf(XPATH,"%s%c","/config/network/vap",un_vap_num);
    sprintf(MACMUM,"%s%s","mac",strpbrk(str_macnum,"0123456789"));
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }

	res = setoption( pst_node, MACMUM, "" );
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

/***************************************************
*****************wireless***************************
****************************************************/


static int ap_wireless_create_vap( char *str_wifi, char *str_ifname )
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s %s %s", "wlanconfig", str_ifname, "create wlandev", str_wifi,"wlanmode ap" );
	system(str_tmp_cmd);
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf(str_tmp_cmd,"ifconfig %s up",str_ifname);
	system(str_tmp_cmd);
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf(str_tmp_cmd,"brctl addif br-lan %s",str_ifname);
	return system(str_tmp_cmd);
}

static int ap_wireless_destroy_vap(  char *str_ifname )
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s", "wlanconfig", str_ifname, "destroy" );
	return system(str_tmp_cmd);
}

static int ap_wireless_addif_vlan(  char *str_ifname,char *str_vlanname )
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s", "brctl addif", str_ifname, str_vlanname );
	return system(str_tmp_cmd);
}

/***************************************************
*****************stp*******************************
****************************************************/




/***************************************************
*****************vlan*******************************
****************************************************/

static int ap_vlan_create( char *str_vlanname)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s", "brctl addbr", str_vlanname);
	return system(str_tmp_cmd);
}

static int ap_vlan_destroy( char *str_vlanname)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s", "brctl delbr", str_vlanname);
	return system(str_tmp_cmd);
}

static int ap_vlan_addport( char *str_vlanname,char *str_portname)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
		
	sprintf( str_tmp_cmd, "%s %s", "brctl addbr", str_vlanname);
	system(str_tmp_cmd);
	sprintf( str_tmp_cmd, "%s %s %s", "vconfig add",str_portname, strpbrk(str_vlanname,"12345678"));
	system(str_tmp_cmd);
	
	sprintf( str_tmp_cmd, "%s %s%s%s %s", "ifconfig", str_portname,".",strpbrk(str_vlanname,"12345678"),"up");
	system(str_tmp_cmd);

  sprintf( str_tmp_cmd, "%s %s %s%s%s", "brctl addif", str_vlanname,str_portname,".",strpbrk(str_vlanname,"12345678"));
	return system(str_tmp_cmd);
}

static int ap_vlan_delport( char *str_vlanname,char *str_portname)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );

	sprintf( str_tmp_cmd, "%s %s %s%s%s", "brctl delif", str_vlanname,str_portname,".",strpbrk(str_vlanname,"12345678"));
	system(str_tmp_cmd);

  sprintf( str_tmp_cmd, "%s %s%s%s", "vconfig rem",str_portname,"." ,strpbrk(str_vlanname,"12345678"));
	system(str_tmp_cmd);
		
	sprintf( str_tmp_cmd, "%s %s", "brctl delbr", str_vlanname);	

	return system(str_tmp_cmd);
}


/***************************************************
*****************system*****************************
****************************************************/

int ap_system_upgrade(char *str_ipaddr,char *str_filename)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN ); 

	sprintf(str_tmp_cmd," { /usr/sbin/clearmemory "\

	                     "&& tftp -g -r %s %s"\
	                     "&& dd if=%s of=/dev/mtdblock1 bs=10"\
	                     "&& rm -rf %s"\
	                     "&& reboot ; } "
	                      ,str_filename,str_ipaddr,str_filename,str_filename);
	
   	//system("./usr/sbin/clearmemory");
	
	//sprintf( str_tmp_cmd, "%s %s %s", "tftp -g -r",str_filename,str_ipaddr);
	//system(str_tmp_cmd);

//  memset( str_tmp_cmd, 0, MAX_CMD_LEN );
  //sprintf( str_tmp_cmd, "%s%s %s", "dd if=",str_filename,"of=/dev/mtdblock1 bs=1024");
  //system(str_tmp_cmd);
  //memset( str_tmp_cmd, 0, MAX_CMD_LEN );
 // sprintf(str_tmp_cmd,"{rm -rf %s && %s ;}",str_filename,"reboot");
  return system(str_tmp_cmd);
}

int ap_system_show(void)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );

	sprintf( str_tmp_cmd, "%s", "showsysinfo");
	return system(str_tmp_cmd);
}

int ap_cpuinfo_show(void)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s", "cat /proc/cpuinfo");
	return system(str_tmp_cmd);
}

int ap_get_macaddressconnectedWithac(char* str_mac)
{

	FILE   *stream; 
	memset(str_mac, 0x00, sizeof(str_mac));
	stream = popen( "/sbin/ifconfig eth0 |grep -o -i -e 'hwaddr [0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}' | cut -d' ' -f2", "r" );
	fread( str_mac, sizeof(char), sizeof(str_mac),  stream); 
	fclose( str_mac );
	str_mac[strlen(str_mac)-1]='\0';
	return 0;
}


int ap_system_restore(char *str_ipaddr,char *str_filename)
{
	  char str_tmp_cmd[MAX_CMD_LEN];
	  memset( str_tmp_cmd, 0, MAX_CMD_LEN ); 
	
	  sprintf(str_tmp_cmd," { cd /jffs/ "\
						   "&& tftp -g -r %s %s"\
						   "&& reboot ; } "
							,str_filename,str_ipaddr,str_filename,str_filename);
	return system(str_tmp_cmd);

}

int ap_system_backup(char *str_ipaddr)
{
	char *str_filename="config.xml";
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "{ cd /etc/config && %s %s %s ; }", "tftp -p -r",str_filename,str_ipaddr);
	return system(str_tmp_cmd);
}

 int ap_system_reset(void)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );

	sprintf( str_tmp_cmd, "%s", "rm -f /etc/config/config.xml");
	system(str_tmp_cmd);
	return system("reboot");
}

 int ap_system_reboot(void)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	return system("reboot");
	
}

 int ap_system_ping(char *str_ipaddr)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );

	sprintf( str_tmp_cmd, "%s %s", "ping -c 4",str_ipaddr);
	return system(str_tmp_cmd);
}

int ap_wireless_security_filter_delmac( unsigned int un_vap_num,char *str_macn)
{
  char str_tmp_cmd[MAX_CMD_LEN];
  char XPATH[50] ; 
  char str_tmp[MAX_STR_LEN];
  char MACMUM[20] ;
  char str_vap_if[MAX_STR_LEN];
  memset( str_tmp_cmd, 0, MAX_CMD_LEN );

  sprintf(XPATH,"%s%c","/config/network/vap",un_vap_num); 
  get_data_by_path( CONF_FILE, XPATH, "if",
                             str_vap_if, MAX_STR_LEN );

  sprintf(MACMUM,"%s%s","mac",strpbrk(str_macn,"0123456789"));
	
  memset( str_tmp, 0, MAX_STR_LEN );	   
  get_data_by_path( CONF_FILE, XPATH, MACMUM,str_tmp, 20 );

  sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv",str_vap_if,"delmac",str_tmp);
  
  return system(str_tmp_cmd);
}

int wireless_create_vap( char *str_wifi, char *str_ifname )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_wireless_create_vap( str_wifi, str_ifname )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_wireless_create_vap( str_wifi, str_ifname  );
	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	
	 return res;
}

int wireless_addif_vlan( char *str_ifname, char *str_vlanname )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_wireless_addif_vlan( str_ifname, str_vlanname )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_wireless_addif_vlan( str_ifname, str_vlanname  );

	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	
	 return res;
}



int wireless_destroy_vap( char *str_ifname )
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_wireless_destroy_vap( str_ifname )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_wireless_destroy_vap( str_ifname  );
	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	
	 return res;
}


static int conf_wireless_get_vlan( unsigned int un_vap_num,char *str_vlan, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d/vlan", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "enable",
                             str_vlan , un_size );

}

int wireless_get_vlan( unsigned int un_vap_num,char *str_vlan, unsigned int un_size)
{
   return conf_wireless_get_vlan( un_vap_num, str_vlan, un_size);
}

static int conf_wireless_config_vlan( unsigned int un_vap_num,char *str_enbale)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%d%s","/config/network/vap",un_vap_num,"/vlan");

    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }

    if(strcmp(str_enbale,"enable")==0)
    {
	    res = setoption( pst_node, "enable", "1" );
    }
	else if(strcmp(str_enbale,"disable")==0)
    {
	    res = setoption( pst_node, "enable", "0" );
    }
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

int wireless_config_vlan( unsigned int un_vap_num,char *str_enbale)
{
      return conf_wireless_config_vlan( un_vap_num,str_enbale);
}

static int conf_wireless_config_vlanvid( unsigned int un_vap_num,char *br_name)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50];                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%d%s","/config/network/vap",un_vap_num,"/vlan");

    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }
	if(strcmp(br_name,"br-lan")==0)
	{
	   res = setoption( pst_node, "enable", "0" ); 
	}
	else
	{
	   res = setoption( pst_node, "vid", br_name ); 
	}	   
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

static int conf_wireless_get_vlanvid( unsigned int un_vap_num,char *str_vlanvid, unsigned int un_size)
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d/vlan", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "vid",
                             str_vlanvid , un_size );

}

int wireless_get_vlanvid( unsigned int un_vap_num,char *str_vlanvid, unsigned int un_size)
{
   return conf_wireless_get_vlanvid( un_vap_num, str_vlanvid, un_size);
}
static int ap_wireless_config_vlanvid( unsigned int un_vap_num,char *br_name)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	char str_vap_if[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
	
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
	get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s", "brctl addif", br_name, str_vap_if);
	return system(str_tmp_cmd);
}


int wireless_config_vlanvid( unsigned int un_vap_num,char *br_name)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_wireless_config_vlanvid( un_vap_num ,br_name )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_wireless_config_vlanvid( un_vap_num ,br_name);

	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	
	 return res;
}



static int conf_stp_get_state( char *str_state, 
                                  unsigned int un_size )
{
    const char *str_xpath = "/config/network/advanset";
    int ret = XERR_NO_ERROR;
    
    ret = get_data_by_path( CONF_FILE, str_xpath, "stpenable", 
                            str_state, un_size );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

int stp_get_state( char *str_state, unsigned int un_size )
{
    return conf_stp_get_state( str_state, un_size );
}

static int ap_stp_config_enable( char *str_ifname,char * str_enable)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	if(strcmp(str_enable,"enable")==0)
	{
	    sprintf( str_tmp_cmd, "%s %s %s", "brctl stp", str_ifname, "on");
	}
	else
	{
        sprintf( str_tmp_cmd, "%s %s %s", "brctl stp", str_ifname, "off");
	}
	return system(str_tmp_cmd);
}

static int conf_stp_config_enable( char *str_ifname,char *str_enable)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s","/config/network/advanset");
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }

	if(strcmp(str_enable,"enable")==0)
	{
		 res = setoption( pst_node, "stpenable", "yes" );
	}
	else
	{
		  res = setoption( pst_node, "stpenable", "no" );
	}
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}
int stp_config_enable( char *str_ifname,char *str_enable)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curstate[MAX_STR_LEN]={0};
	if((res=stp_get_state(str_curstate,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
	    if(((strstr(str_curstate,"yes")!= NULL) && (strstr(str_enable,"enable")!= NULL))
			||((strstr(str_curstate,"no")!= NULL) && (strstr(str_enable,"disable")!= NULL)))
	    {
		   return IFERR_NO_ERROR;
	    }
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_stp_config_enable( str_ifname ,str_enable )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_stp_config_enable( str_ifname ,str_enable);

	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	}	
	 return res;
}

static int conf_stp_get_bridgeprio( char *str_bridgeprio, 
                                  unsigned int un_size )
{
    const char *str_xpath = "/config/network/advanset";
    int ret = XERR_NO_ERROR;
    
    ret = get_data_by_path( CONF_FILE, str_xpath, "portprio", 
                            str_bridgeprio, un_size );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

int stp_get_bridgeprio( char *str_bridgeprio, unsigned int un_size )
{
    return conf_stp_get_bridgeprio( str_bridgeprio, un_size );
}

static int ap_stp_config_bridgeprio( char *str_ifname,char *str_bridgeprio)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s", "brctl setbridgeprio", str_ifname, str_bridgeprio);
	return system(str_tmp_cmd);
}

static int conf_stp_config_bridgeprio( char *str_ifname,char *str_bridgeprio)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s","/config/network/advanset");
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }


	res = setoption( pst_node, "portprio", str_bridgeprio );

    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}
int stp_config_bridgeprio( char *str_ifname,char *str_bridgeprio)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curbridgeprio[MAX_STR_LEN]={0};
    if((res=stp_get_bridgeprio(str_curbridgeprio,MAX_STR_LEN)) == IFERR_NO_ERROR)
    {
        if(strcmp(str_curbridgeprio,str_bridgeprio) == 0)
        {
			return IFERR_ARG_INVALID;
        }
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_stp_config_bridgeprio( str_ifname ,str_bridgeprio )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_stp_config_bridgeprio( str_ifname ,str_bridgeprio);
	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
    }
	
	 return res;
}

static int conf_stp_get_fd( char *str_fd, 
                                  unsigned int un_size )
{
    const char *str_xpath = "/config/network/advanset";
    int ret = XERR_NO_ERROR;
    
    ret = get_data_by_path( CONF_FILE, str_xpath, "setfd", 
                            str_fd, un_size );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

int stp_get_fd( char *str_fd, unsigned int un_size )
{
    return conf_stp_get_fd( str_fd, un_size );
}

static int ap_stp_config_setfd( char *str_ifname,char *str_setfd)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s", "brctl setfd", str_ifname, str_setfd);
	return system(str_tmp_cmd);
}


static int conf_stp_config_setfd( char *str_ifname,char *str_setfd)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s","/config/network/advanset");
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }


	res = setoption( pst_node, "setfd", str_setfd );

    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}
int stp_config_setfd( char *str_ifname,char *str_setfd)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_cursetfd[MAX_STR_LEN]={0};
    if((res=stp_get_fd(str_cursetfd,MAX_STR_LEN)) == IFERR_NO_ERROR)
    {
        if(strcmp(str_cursetfd,str_setfd) == 0)
        {
			return IFERR_ARG_INVALID;
        }
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_stp_config_setfd( str_ifname ,str_setfd )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_stp_config_setfd( str_ifname ,str_setfd);
	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
    }
	
	 return res;
}

static int conf_stp_get_hello( char *str_hello, 
                                  unsigned int un_size )
{
    const char *str_xpath = "/config/network/advanset";
    int ret = XERR_NO_ERROR;
    
    ret = get_data_by_path( CONF_FILE, str_xpath, "sethello", 
                            str_hello, un_size );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

int stp_get_hello( char *str_hello, unsigned int un_size )
{
    return conf_stp_get_hello( str_hello, un_size );
}

static int ap_stp_config_hello( char *str_ifname,char *str_hello)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s", "brctl sethello", str_ifname, str_hello);
	return system(str_tmp_cmd);
}
static int conf_stp_config_hello( char *str_ifname,char *str_hello)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s","/config/network/advanset");
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }


	res = setoption( pst_node, "sethello", str_hello );

    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

int stp_config_hello( char *str_ifname,char *str_hello)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curhello[MAX_STR_LEN]={0};
	if((res=stp_get_hello(str_curhello,MAX_STR_LEN)) == 0)
	{
	    if(strcmp(str_curhello,str_hello) == 0)
	    {
			return IFERR_ARG_INVALID;
	    }
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_stp_config_hello( str_ifname ,str_hello )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_stp_config_hello( str_ifname ,str_hello);
	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	}
	 return res;
}

static int conf_stp_get_maxage( char *str_maxage, 
                                  unsigned int un_size )
{
    const char *str_xpath = "/config/network/advanset";
    int ret = XERR_NO_ERROR;
    
    ret = get_data_by_path( CONF_FILE, str_xpath, "maxage", 
                            str_maxage, un_size );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

int stp_get_maxage( char *str_maxage, unsigned int un_size )
{
    return conf_stp_get_maxage( str_maxage, un_size );
}

static int ap_stp_config_maxage( char *str_ifname,char *str_maxage)
{
	char str_tmp_cmd[MAX_CMD_LEN];
	memset( str_tmp_cmd, 0, MAX_CMD_LEN );
	sprintf( str_tmp_cmd, "%s %s %s", "brctl setmaxage", str_ifname, str_maxage);
	return system(str_tmp_cmd);
}

static int conf_stp_config_maxage( char *str_ifname,char *str_maxage)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s","/config/network/advanset");
    
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }


	res = setoption( pst_node, "maxage", str_maxage );

    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}
int stp_config_maxage( char *str_ifname,char *str_maxage)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
    char str_curmaxage[MAX_STR_LEN]={0};
	if((res=stp_get_maxage(str_curmaxage,MAX_STR_LEN)) == 0)
	{
	    if(strcmp(str_curmaxage,str_maxage) == 0)
	    {
			return IFERR_ARG_INVALID;
	    }
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_stp_config_maxage( str_ifname ,str_maxage )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_stp_config_maxage( str_ifname ,str_maxage);
	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	}
	
	 return res;
}

int vlan_create( char *str_vlanname)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_vlan_create( str_vlanname )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_vlan_create( str_vlanname);

	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	
	 return res;
}

int vlan_addport( char *str_vlanname,char * str_vlanport)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_vlan_addport( str_vlanname,str_vlanport )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_vlan_addport( str_vlanname,str_vlanport);

	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	
	 return res;
}

int vlan_delport( char *str_vlanname,char * str_vlanport)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_vlan_delport( str_vlanname,str_vlanport )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_vlan_delport( str_vlanname,str_vlanport);
	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	
	 return res;
}

int vlan_destroy( char *str_vlanname)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_vlan_destroy( str_vlanname )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}	
	
	res = conf_vlan_destroy( str_vlanname);
	
	 flock( fd_mutex, LOCK_UN );
	 close( fd_mutex );
	
	 return res;
}







/*----------------------------------------------------------*
                    get mac address of filter
*-----------------------------------------------------------*/
static int conf_wireless_get_macfilter( unsigned int un_vap_num,char *str_state, unsigned int un_size )									  
{
    char str_xpath[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;
    memset( str_xpath, 0, MAX_PATH_LEN );
    sprintf( str_xpath, "/config/network/vap%d", un_vap_num );
		
    ret = get_data_by_path( CONF_FILE, str_xpath, "macfilter", 
								str_state, un_size );
	return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}
	
int wireless_get_macfilter( unsigned int un_vap_num,char *str_state, unsigned int un_size )
{
    return conf_wireless_get_macfilter(  un_vap_num,str_state, un_size );
}

int ap_wireless_security_macfilter( unsigned int un_vap_num,char *str_enbale)
{
  char str_tmp_cmd[MAX_CMD_LEN];
  char str_vap_if[MAX_STR_LEN];
  char str_tmp_path[MAX_PATH_LEN];
	
  sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
  get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
  memset( str_tmp_cmd, 0, MAX_CMD_LEN );
  
  if(strcmp(str_enbale,"enable")==0)
  {
     sprintf( str_tmp_cmd, "%s %s %s", "iwpriv",str_vap_if,"maccmd 3");
  }
  else if(strcmp(str_enbale,"disable")==1)
  {
     sprintf( str_tmp_cmd, "%s %s %s", "iwpriv",str_vap_if,"maccmd 0");
  }
  return system(str_tmp_cmd);
}

static int conf_wireless_security_macfilter( unsigned int un_vap_num,char *str_enbale)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;
	char XPATH[50] ;       /*** the ap have many vap****/
    sprintf( XPATH, "/config/network/vap%d", un_vap_num );
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }

    if(strcmp(str_enbale,"enable")==0)
    {
	    res = setoption( pst_node, "macfilter", "yes" );
    }
	else if(strcmp(str_enbale,"disable")==0)
    {
	    res = setoption( pst_node, "macfilter", "no" );
    }
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


int wireless_security_macfilter( unsigned int un_vap_num,char *str_enable)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curenable[MAX_STR_LEN]={0};
	if( (res =wireless_get_macfilter( un_vap_num, str_curenable,30))== IFERR_NO_ERROR)
	{
	    if(((strstr(str_enable,"enable")!=NULL)&&(strstr(str_curenable,"yes")!=NULL))
			||((strstr(str_enable,"disable")!=NULL)&&(strstr(str_curenable,"no")!=NULL)))
	    {
			return IFERR_ARG_INVALID;
	    }
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_security_macfilter( un_vap_num,str_enable )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_security_macfilter( un_vap_num,str_enable);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}
	
	 return res;
}

static int conf_wireless_get_macfiltermode( unsigned int un_vap_num,char *str_filtermode, unsigned int un_size )									  
{
    char str_xpath[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;
    memset( str_xpath, 0, MAX_PATH_LEN );
    sprintf( str_xpath, "/config/network/vap%d", un_vap_num );
		
    ret = get_data_by_path( CONF_FILE, str_xpath, "filtermode", 
								str_filtermode, un_size );
	return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}
	
int wireless_get_macfiltermode( unsigned int un_vap_num,char *str_filtermode, unsigned int un_size )
{
    return conf_wireless_get_macfiltermode(  un_vap_num,str_filtermode, un_size );
}


int ap_wireless_security_filtermode( unsigned int un_vap_num,char *str_filtermode)
{
  char str_tmp_cmd[MAX_CMD_LEN];
  char str_vap_if[MAX_STR_LEN];
  char str_tmp_path[MAX_PATH_LEN];
	
  sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
  get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
  memset( str_tmp_cmd, 0, MAX_CMD_LEN );
  if(strcmp(str_filtermode,"permit")==0)
  {
     sprintf( str_tmp_cmd, "%s %s %s", "iwpriv",str_vap_if,"maccmd 1");
  }
  else if(strcmp(str_filtermode,"prevent")==0)
  {
     sprintf( str_tmp_cmd, "%s %s %s", "iwpriv",str_vap_if,"maccmd 2");
  }
  return system(str_tmp_cmd);
}

static int conf_wireless_security_filtermode( unsigned int un_vap_num,char *str_filtermode)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50] ;                                        /*** the ap have many vap****/
	sprintf(XPATH,"%s%d","/config/network/vap",un_vap_num);
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }

    if(strcmp(str_filtermode,"prevent")==0)
    {
	    res = setoption( pst_node, "filtermode", "prevent" );
    }
	else if(strcmp(str_filtermode,"permit")==0)
    {
	    res = setoption( pst_node, "filtermode", "permit" );
    }
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}


int wireless_security_filtermode( unsigned int un_vap_num,char *str_filtermode)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char str_curfiltermode[MAX_STR_LEN]={0};

	if((res=wireless_get_macfiltermode(un_vap_num,str_curfiltermode,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
	    if(strcmp(str_curfiltermode,str_filtermode) == 0)
	    {
			return IFERR_ARG_INVALID;
	    }
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_security_filtermode( un_vap_num,str_filtermode )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_security_filtermode( un_vap_num,str_filtermode);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}
	return res;
}

/*******************************************************/
/*****************filter addmac****************************/
/*******************************************************/

int ap_wireless_security_filter_addmac( unsigned int un_vap_num,char *str_macn,char *str_macaddr)
{
  char str_tmp_cmd[MAX_CMD_LEN];
  char XPATH[50] ; 
  char str_tmp[MAX_STR_LEN];
  char MACMUM[20] ;
  char str_vap_if[MAX_STR_LEN];
  
  sprintf(XPATH,"%s%c","/config/network/vap",un_vap_num);
  get_data_by_path( CONF_FILE, XPATH, "if",
                             str_vap_if, MAX_STR_LEN );
  memset( str_tmp_cmd, 0, MAX_CMD_LEN );
  sprintf(MACMUM,"%s%s","mac",strpbrk(str_macn,"0123456789"));
	
  memset( str_tmp, 0, MAX_STR_LEN );	   
  get_data_by_path( CONF_FILE, XPATH, MACMUM,str_tmp, 20 );

  if(strlen(str_macaddr)==17)
  {
      sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv",str_vap_if,"delmac",str_tmp);
      system(str_tmp_cmd);
  }
  
  memset( str_tmp_cmd, 0, MAX_CMD_LEN );
  sprintf( str_tmp_cmd, "%s %s %s %s", "iwpriv",str_vap_if,"addmac",str_macaddr);
  return system(str_tmp_cmd);
}

static int conf_wireless_security_filter_addmac( unsigned int un_vap_num,char *str_macnum,char *str_macaddr)
{
	MXML_NODE* pst_node = NULL;
	int res = IFERR_NO_ERROR;

	char XPATH[50];                                        /*** the ap have many vap****/
    char MACMUM[20];
    sprintf(XPATH,"%s%c","/config/network/vap",un_vap_num);
    sprintf(MACMUM,"%s%s","mac",strpbrk(str_macnum,"0123456789"));
	
	if( NULL == (pst_node=getnode(CONF_FILE, XPATH)) )
	{
		return IFERR_ARG_NOT_FOUND;
    }

	res = setoption( pst_node, MACMUM, str_macaddr);
    
    if( res != IFERR_NO_ERROR )
    {
    	destroynode(pst_node);
    	return IFERR_ARG_WRONG;
    }
    
    if( setnode( CONF_FILE, XPATH, pst_node ) != IFERR_NO_ERROR )
    {
    	    destroynode(pst_node);
    	    return IFERR_FILE_WRITE;
    }
    
    return IFERR_NO_ERROR;
    
}

int wireless_security_filter_addmac( unsigned int un_vap_num,char *str_macn, char *str_macaddr)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
    char str_curmacaddr[MAX_STR_LEN]={0};
	char MACMUM[20]={0} ;
    sprintf(MACMUM,"%s%s","mac",strpbrk(str_macn,"0123456789"));
	if((res=wireless_sec_get_macfilter(un_vap_num,MACMUM,str_curmacaddr,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
	    if(strcmp(str_curmacaddr,str_macaddr) == 0)
	    {
			 return IFERR_NO_ERROR;
	    }
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_security_filter_addmac( un_vap_num,str_macn,str_macaddr )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_security_filter_addmac( un_vap_num,str_macn,str_macaddr);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
		
		 return res;
	}
}


/*******************************************************/
/*****************filter delmac****************************/
/*******************************************************/

int wireless_security_filter_delmac(unsigned int un_vap_num,char *str_macn)
{
	int fd_mutex ;
	int res = IFERR_NO_ERROR;
	char MACMUM[20]={0} ;
	char str_curmacaddr[MAX_STR_LEN]={0};
    sprintf(MACMUM,"%s%s","mac",strpbrk(str_macn,"0123456789"));
	
	if((res=wireless_sec_get_macfilter(un_vap_num,MACMUM,str_curmacaddr,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
	    if(strlen(str_curmacaddr) ==0)
	    {
			return IFERR_ARG_INVALID;
	    }
		if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
		{
			return IFERR_FILE_NOT_FOUND;
		}
		if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
		{
			close( fd_mutex );
			return IFERR_LOCK_FAILED;
		}
		if( (res = ap_wireless_security_filter_delmac( un_vap_num,str_macn )) != IFERR_NO_ERROR )
		{
		    flock( fd_mutex, LOCK_UN );
		    close(fd_mutex);
		    return res;
		}	
		
		res = conf_wireless_security_filter_delmac( un_vap_num,str_macn);
		
		 flock( fd_mutex, LOCK_UN );
		 close( fd_mutex );
	}
	
	 return res;
}
static int conf_wireless_sec_get_macfilter( unsigned int un_vap_num, 
                                            char *str_filter, 
                                            char *str_macaddr, 
                                            unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;
    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    ret = get_data_by_path( CONF_FILE, str_tmp_path, str_filter, str_macaddr, 
                            un_size );
    if( ret < 0 )
    {
        return ret;
    }

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_ARG_NOT_FOUND;
        
}

int wireless_sec_get_macfilter( unsigned int un_vap_num, 
                                char *str_filter, 
                                char *str_macaddr, 
                                unsigned int un_size )
{
    return conf_wireless_sec_get_macfilter( un_vap_num,
                                            str_filter,
                                            str_macaddr,
                                            un_size );
}

/*----------------------------------------------------------*
       wireless security [vapn] type none/wep/wpa/wpa2]
*-----------------------------------------------------------*/
static int conf_wireless_sec_get_driver( char *str_driver, unsigned int un_size )
{
    strcpy( str_driver, "madwifi" );
    return IFERR_NO_ERROR;
}

static int conf_wireless_sec_get_syslog( char *str_syslog, unsigned int un_size )
{
    strcpy( str_syslog, "-1" );
    return IFERR_NO_ERROR;
}

static int conf_wireless_sec_get_loglev( char *str_log_level, unsigned int un_size )
{
    strcpy( str_log_level, "2" );
    return IFERR_NO_ERROR;
}

static int conf_wireless_sec_get_stdout( char *str_std_out, unsigned int un_size )
{
    strcpy( str_std_out, "-1" );
    return IFERR_NO_ERROR;
}

static int conf_wireless_sec_get_outlev( char *str_out_lev, unsigned int un_size )
{
    strcpy( str_out_lev, "2" );
    return IFERR_NO_ERROR;
}

static int conf_wireless_sec_get_debug( char *str_debug, unsigned int un_size )
{
    strcpy( str_debug, "0" );
    return IFERR_NO_ERROR;
}

static int conf_wireless_sec_get_macacl( char *str_macacl, unsigned int un_size )
{
    strcpy( str_macacl, "0" );
    return IFERR_NO_ERROR;
}

static int conf_wireless_sec_get_eapol( char *str_eapol, unsigned int un_size )
{
    strcpy( str_eapol, "0" );
    return IFERR_NO_ERROR;
}

static int conf_wireless_sec_get_bcast( char *str_bcast, unsigned int un_size )
{
    strcpy( str_bcast, "5" );
    return IFERR_NO_ERROR;
}
    
static int conf_wireless_sec_get_ucast( char *str_ucast, unsigned int un_size )
{
    strcpy( str_ucast, "5" );
    return IFERR_NO_ERROR;
}
    
static int conf_wireless_sec_get_period( char *str_period, unsigned int un_size )
{
    strcpy( str_period, "300" );
    return IFERR_NO_ERROR;
}

#define UNKNOWN 0
#define RADIUS 1
#define PSK 2
#define WEP 1
#define WPA 2
#define WPA2 3

int get_vnum_by_vname( char *str_vname )
{
    char str_tmp_path[MAX_PATH_LEN];
    char str_tmp_data[MAX_STR_LEN];
    int i = 1;
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    memset( str_tmp_data, 0, MAX_STR_LEN );

    for( ; i<=MAX_VAP_NUM; i++ )
    {
        sprintf( str_tmp_path, "/config/network/vap%d", i );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "if",
                                str_tmp_data, MAX_STR_LEN );
        if( XERR_NO_ERROR == ret )
        {
            if( 0 == strcmp(str_vname, str_tmp_data) )
            {
                break;
            }
        }
    }
    return (i<=8) ? i : -1;
}

static int ap_wireless_sec_set_type( unsigned int un_vap_num,  char *str_type )
{
    int res = IFERR_NO_ERROR;
    int ret = XERR_NO_ERROR;
    char str_tmp_cmd[MAX_CMD_LEN];
    char str_conf_file[MAX_PATH_LEN];
    char str_old_type[MAX_STR_LEN];
    char str_if_name[MAX_STR_LEN];
    char str_if_addr[MAX_STR_LEN];
    char str_vap_if[MAX_STR_LEN];
    char str_tmp_data1[MAX_STR_LEN];
    char str_tmp_data2[MAX_STR_LEN];
    char str_tmp_data3[MAX_STR_LEN];
    char str_tmp_data4[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
    const char *CONF_PATH = "/jffs/hostapd/";
    const char *DUMP_FILE = "/tmp/hostapd.dump";
    const char *CTRL_IF = "/var/run/hostapd";
    int flag_sec_type = UNKNOWN;
    int flag_sec_mode = UNKNOWN;
    
    memset( str_tmp_cmd, 0, MAX_CMD_LEN );
    memset( str_conf_file, 0, MAX_PATH_LEN );
    memset( str_old_type, 0, MAX_STR_LEN );
    memset( str_if_name, 0, MAX_STR_LEN );
    memset( str_if_addr, 0, MAX_STR_LEN );
    memset( str_vap_if, 0, MAX_STR_LEN );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    memset( str_tmp_data2, 0, MAX_STR_LEN );
    memset( str_tmp_data3, 0, MAX_STR_LEN );
    memset( str_tmp_data4, 0, MAX_STR_LEN );
    memset( str_tmp_path, 0, MAX_PATH_LEN );

    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    ret = get_data_by_path( CONF_FILE, str_tmp_path, "enable",
                            str_tmp_data1, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_WRONG;
    }
    if( 0 == strcmp(str_tmp_data1, "no") )
    {
        return IFERR_COM_CANNOT_EXEC;
    }
    ret = get_data_by_path( CONF_FILE, str_tmp_path, "security", 
                            str_old_type, MAX_STR_LEN );
    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    ret += get_data_by_path( CONF_FILE, str_tmp_path, "if",
                             str_vap_if, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }
    
// stop encrypt service
    if( strcmp(str_old_type, "none") != 0 )
    {
        sprintf( str_tmp_cmd, "{ { { %s %s && %s %s up ; } || true ;} && %s %s key open off ; } %s ",
		                      KILLALL, HOSTAPD, 
		                      IFCONFIG, str_vap_if,
		                      IWCONFIG, str_vap_if,
                              NO_MESSAGE );
        res = system( str_tmp_cmd );
        if( res != 0 )
        {
            return IFERR_COM_EXEC_FAILED;
        }
    }
	
    if( 0 == strcmp(str_type, "none") )
    {
        return IFERR_NO_ERROR;
    }
    sprintf( str_tmp_cmd, "%s %s down",
		                  IFCONFIG, str_vap_if );
	res = system( str_tmp_cmd );
	if( res != 0 )
	{
		return IFERR_COM_EXEC_FAILED;
	}

    if( 0 == strcmp(str_type, "wep") )
    {
        memset( str_tmp_path, 0, MAX_PATH_LEN );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "authmode", 
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != XERR_NO_ERROR )
        {
            return IFERR_ARG_NOT_FOUND;
        }
        //data1:autemode data2:wepkey data3:passkey data4: s:or\0
        if( 0 == strcmp(str_tmp_data1, "1") 
         || 0 == strcmp(str_tmp_data1, "2") )
        {
            ret = get_data_by_path( CONF_FILE, str_tmp_path, "wepkey",
                                    str_tmp_data2, MAX_STR_LEN );
			//printf("wepkey:%s\n", str_tmp_data2);
            sprintf( str_tmp_data4, "passkey%s", str_tmp_data2 );
			//printf("passname : %s\n", str_tmp_data4);
            ret += get_data_by_path( CONF_FILE, str_tmp_path, str_tmp_data4,
                                     str_tmp_data3, MAX_STR_LEN );
			//printf("key: %s\n", str_tmp_data3);
            memset( str_tmp_data4, 0, MAX_STR_LEN );
            ret += get_data_by_path( CONF_FILE, str_tmp_path, "wepkeytype", 
                                     str_tmp_data4, MAX_STR_LEN );
			//printf("keytype: %s\n", str_tmp_data4);
            if( ret != XERR_NO_ERROR )
            {
                return IFERR_ARG_NOT_FOUND;
            }
            
            sprintf( str_tmp_cmd, "{ %s %s key [%s] %s%s open "\
				                  "&&  %s %s authmode %s "\
				                  "&& %s %s up ; } %s ", 
				                  IWCONFIG, str_vap_if, str_tmp_data2, 
                                  strcmp(str_tmp_data4,"hex")?"s:":" " , str_tmp_data3,
                                  IWPRIV, str_vap_if, str_tmp_data1,
                                  IFCONFIG, str_vap_if,
                                  NO_MESSAGE );
			//printf("cmd: %s\n", str_tmp_cmd);
            res = system( str_tmp_cmd );
            return (0==res) ? IFERR_NO_ERROR : IFERR_COM_EXEC_FAILED;
        }
        if( strcmp(str_tmp_data1, "3" ) != 0 )
        {
            return IFERR_ARG_WRONG;
        }
        flag_sec_type = WEP;
        flag_sec_mode = RADIUS;
    }
    else
    {
        if( 0 == strcmp(str_type, "wpa") )
        {
            flag_sec_type = WPA;
        }
        else if( 0 == strcmp(str_type, "wpa2") )
        {
            flag_sec_type = WPA2;
        }
        else
        {
            return IFERR_ARG_WRONG;
        }
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "wpamode", 
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != XERR_NO_ERROR )
        {
            return IFERR_ARG_NOT_FOUND;
        }
        if( 0 == strcmp(str_tmp_data1, "PSK") )
        {
            flag_sec_mode = PSK;
        }
        else
        {
            flag_sec_mode = RADIUS;
        }
    }
//get if_name and ip_addr by vid, if there is not any vlan , we use lan default
    memset( str_tmp_path, 0, MAX_STR_LEN );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    sprintf( str_tmp_path, "/config/network/vap%d/vlan", un_vap_num );
    ret = get_data_by_path( CONF_FILE, str_tmp_path, "vid", str_tmp_data1,
                            MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        if( XERR_NODE_NO_DATA == ret )
        {
            memset( str_tmp_path, 0, MAX_STR_LEN );
            sprintf( str_tmp_path, "/config/network/lan" );
        }
        else
        {
            return IFERR_ARG_NOT_FOUND;
        }
    }
    else
    {
        int i = 1;
        for( ; i<=8; i++ )
        {
            memset( str_tmp_path, 0, MAX_STR_LEN );
            memset( str_tmp_data2, 0, MAX_STR_LEN );
            sprintf( str_tmp_path, "/config/network/vlan%d", i );
            get_data_by_path( CONF_FILE, str_tmp_path, "vid",
                                    str_tmp_data2, MAX_STR_LEN );
            if( 0 == strcmp( str_tmp_data1, str_tmp_data2 ) )
            {
                break;
            }
        }
    }
    ret = get_data_by_path( CONF_FILE, str_tmp_path, "if", 
                            str_if_name, MAX_STR_LEN );
    ret += get_data_by_path( CONF_FILE, str_tmp_path, "ipaddr",
                             str_if_addr, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }

    //  create hostapd config file
    memset( str_tmp_path, 0, MAX_PATH_LEN );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    sprintf( str_conf_file, "%s%s%d", CONF_PATH, (RADIUS==flag_sec_mode)?"b":"a", un_vap_num );
    sprintf( str_tmp_cmd, "echo \"\" > %s ", str_conf_file );
    res = system( str_tmp_cmd );
    if( res != 0 )
    {
        return IFERR_COM_EXEC_FAILED;
    }

    //public head
    sprintf( str_tmp_cmd, "echo \"interface=%s\" >> %s ", str_vap_if, str_conf_file );
    res = system( str_tmp_cmd );
    sprintf( str_tmp_cmd, "echo \"bridge=%s\" >> %s", str_if_name, str_conf_file );
    res += system( str_tmp_cmd );
    ret = conf_wireless_sec_get_driver( str_tmp_data1, MAX_STR_LEN );
    if( ret != IFERR_NO_ERROR )
    {
        return IFERR_ARG_WRONG;
    }
    sprintf( str_tmp_cmd, "echo \"driver=%s\" >> %s", str_tmp_data1,str_conf_file );
    res += system( str_tmp_cmd );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    ret = conf_wireless_sec_get_syslog( str_tmp_data1 , MAX_STR_LEN );
    if( ret != IFERR_NO_ERROR )
    {
        return IFERR_ARG_WRONG;
    }
    sprintf( str_tmp_cmd, "echo \"logger_syslog=%s\" >> %s", str_tmp_data1, 
                                                             str_conf_file );
    res += system( str_tmp_cmd );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    ret =conf_wireless_sec_get_loglev( str_tmp_data1 , MAX_STR_LEN );
    if( ret != IFERR_NO_ERROR )
    {
        return IFERR_ARG_WRONG;
    }
    sprintf( str_tmp_cmd, "echo \"logger_syslog_level=%s\" >> %s", str_tmp_data1, 
                                                                   str_conf_file );
    res += system( str_tmp_cmd );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    ret = conf_wireless_sec_get_stdout( str_tmp_data1, MAX_STR_LEN );
    if( ret != IFERR_NO_ERROR )
    {
        return IFERR_ARG_WRONG;
    }
    sprintf( str_tmp_cmd, "echo \"logger_stdout=%s\" >> %s", str_tmp_data1, 
                                                             str_conf_file );
    res += system( str_tmp_cmd );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    ret = conf_wireless_sec_get_outlev( str_tmp_data1, MAX_STR_LEN );
    if( ret != IFERR_NO_ERROR )
    {
        return IFERR_ARG_WRONG;
    }    
    sprintf( str_tmp_cmd, "echo \"logger_stdout_level=%s\" >> %s", str_tmp_data1, 
                                                                   str_conf_file );
    res += system( str_tmp_cmd );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    ret = conf_wireless_sec_get_debug( str_tmp_data1, MAX_STR_LEN );
    if( ret != IFERR_NO_ERROR )
    {
        return IFERR_ARG_WRONG;
    }
    sprintf( str_tmp_cmd, "echo \"debug=%s\" >> %s", str_tmp_data1, 
                                                                   str_conf_file );
    res += system( str_tmp_cmd );
    sprintf( str_tmp_cmd, "echo \"dump_file=%s\" >>%s ", DUMP_FILE,
                                                         str_conf_file );
    res += system( str_tmp_cmd );
    sprintf( str_tmp_cmd, "echo \"ctrl_interface=%s\" >> %s", CTRL_IF,
                                                              str_conf_file);
    res += system( str_tmp_cmd );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    ret = conf_wireless_sec_get_macacl( str_tmp_data1, MAX_STR_LEN );
    if( ret != IFERR_NO_ERROR )
    {
        return IFERR_ARG_WRONG;
    }
    sprintf( str_tmp_cmd, "echo \"macaddr_acl=%s\" >>%s",str_tmp_data1,
                                                         str_conf_file );
    res += system( str_tmp_cmd );

    if( res != 0 )
    {
        return IFERR_COM_EXEC_FAILED;
    }
    memset( str_tmp_path, 0, MAX_PATH_LEN );
    memset( str_tmp_data1, 0, MAX_STR_LEN );
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    ret = get_data_by_path( CONF_FILE, str_tmp_path, "ssid",
                            str_tmp_data1, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }
    sprintf( str_tmp_cmd, "echo \"ssid=%s\" >> %s", str_tmp_data1,
                                                    str_conf_file );
    res = system( str_tmp_cmd );
    if( res != 0 )
    {
        return IFERR_COM_EXEC_FAILED;
    }

    //radius part
    if( RADIUS == flag_sec_mode )
    {
        sprintf( str_tmp_cmd, "echo \"ieee8021x=1\" >> %s", str_conf_file );
        res = system( str_tmp_cmd );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = conf_wireless_sec_get_eapol( str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"eapol_key_index_workaround=%s\" >> %s", str_tmp_data1,
                                                                              str_conf_file );
        res += system( str_tmp_cmd );
        sprintf( str_tmp_cmd, "echo \"own_ip_addr=%s\" >> %s", str_if_addr,
                                                               str_conf_file );
        res += system( str_tmp_cmd );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        sprintf( str_tmp_path, "/config/network/radius" );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "radiusip",
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"auth_server_addr=%s\" >> %s ", str_tmp_data1,
                                                                     str_conf_file );
        res += system( str_tmp_cmd );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "authport",
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"auth_server_port=%s\" >> %s ", str_tmp_data1,
                                                                     str_conf_file );
        res += system( str_tmp_cmd );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "sharekey",
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"auth_server_shared_secret=%s\" >> %s ", str_tmp_data1,
                                                                              str_conf_file );
        res += system( str_tmp_cmd );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "radiusaip",
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"acct_server_addr=%s\" >> %s ", str_tmp_data1,
                                                                     str_conf_file );
        res += system( str_tmp_cmd );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "acceptport",
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"acct_server_port=%s\" >> %s", str_tmp_data1,
                                                                    str_conf_file );
        res += system( str_tmp_cmd );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "sharekey",
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"acct_server_shared_secret=%s\" >> %s", str_tmp_data1,
                                                                             str_conf_file );
        res += system( str_tmp_cmd );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "radiusipb",
                                str_tmp_data1, MAX_STR_LEN );
        if( IFERR_NO_ERROR == ret )
        {
            sprintf( str_tmp_cmd, "echo \"auth_server_addr_back=%s\" >> %s", str_tmp_data1,
                                                                             str_conf_file );
            res += system( str_tmp_cmd ); 
        }
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "authportb",
                                str_tmp_data1, MAX_STR_LEN );
        if( IFERR_NO_ERROR == ret )
        {
            sprintf( str_tmp_cmd, "echo \"auth_server_port_back=%s\" >> %s", str_tmp_data1,
                                                                             str_conf_file );
            res += system( str_tmp_cmd ); 
        }
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "sharekeyb",
                                str_tmp_data1, MAX_STR_LEN );
        if( IFERR_NO_ERROR == ret )
        {
            sprintf( str_tmp_cmd, "echo \"auth_server_shared_secret_back=%s\" >> %s", str_tmp_data1,
                                                                                      str_conf_file );
            res += system( str_tmp_cmd ); 
        }
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "sharekeyb",
                                str_tmp_data1, MAX_STR_LEN );
        if( IFERR_NO_ERROR == ret )
        {
            sprintf( str_tmp_cmd, "echo \"auth_server_shared_secret_back=%s\" >> %s", str_tmp_data1,
                                                                                      str_conf_file );
            res += system( str_tmp_cmd ); 
        }
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "radiusaipb",
                                str_tmp_data1, MAX_STR_LEN );
        if( IFERR_NO_ERROR == ret )
        {
            sprintf( str_tmp_cmd, "echo \"acct_server_addr_back=%s\" >> %s", str_tmp_data1,
                                                                             str_conf_file );
            res += system( str_tmp_cmd ); 
        }
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "accepportb",
                                str_tmp_data1, MAX_STR_LEN );
        if( IFERR_NO_ERROR == ret )
        {
            sprintf( str_tmp_cmd, "echo \"acct_server_port_back=%s\" >> %s", str_tmp_data1,
                                                                             str_conf_file );
            res += system( str_tmp_cmd ); 
        }
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "sharekeyb",
                                str_tmp_data1, MAX_STR_LEN );
        if( IFERR_NO_ERROR == ret )
        {
            sprintf( str_tmp_cmd, "echo \"acct_server_shared_secret_back=%s\" >> %s", str_tmp_data1,
                                                                                      str_conf_file );
            res += system( str_tmp_cmd ); 
        }
        if( res != 0 )
        {
            return IFERR_COM_EXEC_FAILED;
        }

    }
    //psk part
    else
    {
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "passphrase",
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != XERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"wpa_passphrase=%s\" >> %s", str_tmp_data1,
                                                                  str_conf_file );
        res = system( str_tmp_cmd );
        if( res != 0 )
        {
            return IFERR_COM_EXEC_FAILED;
        }
    }

    //wep part
    if( WEP == flag_sec_type )
    {
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = conf_wireless_sec_get_bcast( str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"wep_key_len_broadcast=%s\" >> %s", str_tmp_data1,
                                                                         str_conf_file );
        res = system( str_tmp_cmd );        
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = conf_wireless_sec_get_ucast( str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"wep_key_len_unicast=%s\" >> %s", str_tmp_data1,
                                                                       str_conf_file );
        res += system( str_tmp_cmd );          
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        ret = conf_wireless_sec_get_period( str_tmp_data1, MAX_STR_LEN );
        if( ret != IFERR_NO_ERROR )
        {
            return IFERR_ARG_WRONG;
        }
        sprintf( str_tmp_cmd, "echo \"wep_rekey_period=%s\" >> %s", str_tmp_data1,
                                                                       str_conf_file );
        res += system( str_tmp_cmd );  
        
        if( res != 0 )
        {
            return IFERR_COM_EXEC_FAILED;
        }

    }
    // wpa part
    else
    {
        sprintf( str_tmp_cmd, "echo \"wpa=%d\" >> %s", 
                      (WPA==flag_sec_type)?1:2, str_conf_file );
        res = system( str_tmp_cmd );
        sprintf( str_tmp_cmd, "echo \"wpa_key_mgmt=WPA-%s\" >> %s", 
                              (RADIUS==flag_sec_mode)?"EAP":"PSK",
                              str_conf_file );
        res += system( str_tmp_cmd );
        memset( str_tmp_data1, 0, MAX_STR_LEN );
        sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
        ret = get_data_by_path( CONF_FILE, str_tmp_path, "encryption",
                                str_tmp_data1, MAX_STR_LEN );
        if( ret != XERR_NO_ERROR )
        {
            return IFERR_ARG_NOT_FOUND;
        }
        sprintf( str_tmp_cmd, "echo \"wpa_pairwise=%s\" >> %s", 
                              str_tmp_data1, str_conf_file );
        res += system( str_tmp_cmd );

        if( res != 0 )
        {
            return IFERR_COM_EXEC_FAILED;
        }
    }

    //start hostapd
    sprintf( str_tmp_cmd, "{ { %s -B %s & } && %s %s up ; } %s", 
                          HOSTAPD, str_conf_file,
                          IFCONFIG, str_vap_if,
                          NO_MESSAGE );
    res = system( str_tmp_cmd );
    
    return (0==res) ? IFERR_NO_ERROR : IFERR_COM_EXEC_FAILED;
}

static int conf_wireless_sec_set_type( unsigned int un_vap_num, char *str_type )
{
    char str_tmp_path[MAX_PATH_LEN];
    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    if( set_data_by_path(CONF_FILE, str_tmp_path, "security", str_type ) != XERR_NO_ERROR )
    {
        return IFERR_FILE_WRITE;
    }
    return IFERR_NO_ERROR;
}

int wireless_sec_set_type( unsigned int un_vap_num, char *str_type )
{
    int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curtype[MAX_STR_LEN]={0};
	if((res=wireless_sec_get_type(un_vap_num,str_curtype,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{   
	    if(strcmp(str_type,str_curtype)==0)
	    {
		   return IFERR_ARG_INVALID;
	    }
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_wireless_sec_set_type( un_vap_num, str_type )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}
	res = conf_wireless_sec_set_type( un_vap_num, str_type );
	system("/usr/sbin/send_trap apEncryptionTypeChange");

	flock( fd_mutex, LOCK_UN );
	close( fd_mutex );
	}
	return res;
}

int wireless_sec_get_type( unsigned int un_vap_num, char *str_type, unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "security",
                             str_type , un_size );
}

/*----------------------------------------------------------*
       wireless security [vapn] mode [open,psk,802.1x]
*-----------------------------------------------------------*/

static int ap_wireless_sec_set_mode( unsigned int un_vap_num,  char *str_mode )
{
    char str_old_type[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );

    ret = get_data_by_path( CONF_FILE, str_tmp_path, "security", 
                            str_old_type, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }

    return ap_wireless_sec_set_type( un_vap_num, str_old_type );
}

/*-- use authmode and wpamode now, should use union sec mode instead --*/
static int conf_wireless_sec_set_mode( unsigned int un_vap_num,  char *str_mode )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    if( 0 == strcmp(str_mode, "open") )
    {
        ret = set_data_by_path( CONF_FILE, str_tmp_path, "authmode", "1" );
    }
    else if( 0 == strcmp(str_mode, "psk") )
    {
        ret = set_data_by_path( CONF_FILE, str_tmp_path, "authmode", "2" );
        ret += set_data_by_path( CONF_FILE, str_tmp_path, "wpamode", "PSK" );
    }
    else if( 0 == strcmp(str_mode, "802.1x") )
    {
        ret = set_data_by_path( CONF_FILE, str_tmp_path, "authmode", "3" );
        ret += set_data_by_path( CONF_FILE, str_tmp_path, "wpamode", "RADIUS" );
    }
    else
    {
        return IFERR_ARG_WRONG;
    }
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

int wireless_sec_set_mode( unsigned int un_vap_num,  char *str_mode, int flag )
{
    int fd_mutex;
	int res = IFERR_NO_ERROR;

    res = conf_wireless_sec_set_mode( un_vap_num, str_mode );

    if( res != IFERR_NO_ERROR )
    {
        return res;
    }
    
    if( flag )
    {
        if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
    	{
    		return IFERR_FILE_NOT_FOUND;
    	}
    	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
    	{
    		close( fd_mutex );
    		return IFERR_LOCK_FAILED;
    	}
        
        res = ap_wireless_sec_set_mode(un_vap_num, str_mode );
        flock( fd_mutex, LOCK_UN );
        close( fd_mutex );
    }

    return res; 

}

/*------ use "wpamode" for tmp, it should use a union encryption mode name ------*/
int wireless_sec_get_mode( unsigned int un_vap_num, char *str_mode, unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "wpamode",
                             str_mode , un_size );
}

int wireless_sec_get_basicmode( unsigned int un_vap_num, char *str_basicmode, unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "authmode",
                             str_basicmode , un_size );
}

int wireless_sec_get_wpaencrypt( unsigned int un_vap_num, char *str_wpaencrypt, unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "encryption",
                             str_wpaencrypt , un_size );
}

/*--------------------------------------------------------------------------*
       wireless security [vapn]  encryption [tkip,aes,wep64,wep128,wep152]
*---------------------------------------------------------------------------*/
static int ap_wireless_sec_set_encrypt( unsigned int un_vap_num, char *str_encrypt )
{
    char str_old_type[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );

    ret = get_data_by_path( CONF_FILE, str_tmp_path, "security", 
                            str_old_type, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }

    return ap_wireless_sec_set_type( un_vap_num, str_old_type );
    
}

static int conf_wireless_sec_set_encrypt( unsigned int un_vap_num, char *str_encrypt )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    if( 0 == strcmp(str_encrypt, "tkip") )
    {
        ret = set_data_by_path( CONF_FILE, str_tmp_path, "encryption", "TKIP" );
    }
    else if( 0 == strcmp(str_encrypt, "aes") )
    {
        ret = set_data_by_path( CONF_FILE, str_tmp_path, "encryption", "CCMP" );
    }
    else if( 0 == strcmp(str_encrypt, "wep64") )
    {
        ret = set_data_by_path( CONF_FILE, str_tmp_path, "wepbit", "64" );
    }
    else if( 0 == strcmp(str_encrypt, "wep128") )
    {
        ret = set_data_by_path( CONF_FILE, str_tmp_path, "wepbit", "128" );
    }
    else if( 0 == strcmp(str_encrypt, "wep152") )
    {
        ret = set_data_by_path( CONF_FILE, str_tmp_path, "wepbit", "152" );
    }
    else
    {
        return IFERR_ARG_WRONG;
    }
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;    
}

int wireless_sec_set_encrypt( unsigned int un_vap_num, char *str_encrypt, 
                              int flag )
{ 
    int fd_mutex;
	int res = IFERR_NO_ERROR;

    res = conf_wireless_sec_set_encrypt( un_vap_num, str_encrypt );

    if( res != IFERR_NO_ERROR )
    {
        return res;
    }
    
    if( flag )
    {
        if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
    	{
    		return IFERR_FILE_NOT_FOUND;
    	}
    	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
    	{
    		close( fd_mutex );
    		return IFERR_LOCK_FAILED;
    	}
        
        res = ap_wireless_sec_set_encrypt(un_vap_num, str_encrypt);
        flock( fd_mutex, LOCK_UN );
        close( fd_mutex );
    }

    return res; 
}

int wireless_sec_get_encrypt( unsigned int un_vap_num, 
                              char * str_encrypt,
                              unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    char str_cur_type[MAX_STR_LEN];
    char str_cur_mode[MAX_STR_LEN];
    int ret = XERR_NO_ERROR;

    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );

    ret = get_data_by_path( CONF_FILE, str_tmp_path, "security", 
                            str_cur_type, MAX_STR_LEN );
    ret = get_data_by_path( CONF_FILE, str_tmp_path, "authmode",
                            str_cur_mode, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }
    if( 0 == strcmp(str_cur_type, "wpa")
     || 0 == strcmp(str_cur_type, "wpa2") )
    {
        return get_data_by_path( CONF_FILE, str_tmp_path, "encryption",
                                 str_encrypt , un_size );
    }
    if( 0 == strcmp(str_cur_mode, "3") )
    {
        return get_data_by_path( CONF_FILE, str_tmp_path, "encryption",
                                 str_encrypt , un_size );
    }
    return get_data_by_path( CONF_FILE, str_tmp_path, "wepbit",
                                 str_encrypt , un_size );

}


/*------------------------------------------------------*
      wireless security  [vapn] [1,2,3,4] [ascii,hex]
*-------------------------------------------------------*/

static int ap_wireless_sec_set_wepkey( unsigned int un_vap_num, 
                                       char *str_key_seq,
                                       char *str_key_type,
                                       char *str_key )
{
    char str_old_type[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );

    ret = get_data_by_path( CONF_FILE, str_tmp_path, "security", 
                            str_old_type, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }

    return ap_wireless_sec_set_type( un_vap_num, str_old_type );

}


static int conf_wireless_sec_set_wepkey( unsigned int un_vap_num, 
                                         char *str_key_seq,
                                         char *str_key_type,
                                         char *str_key,
                                         int flag )
{
    char str_tmp_path[MAX_PATH_LEN];
    char str_key_name[MAX_STR_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    memset( str_key_name, 0, MAX_STR_LEN );
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    ret = set_data_by_path( CONF_FILE, str_tmp_path, "wepkeytype", 
                            str_key_type );
    sprintf( str_key_name, "passkey%s", str_key_seq );
    ret += set_data_by_path( CONF_FILE, str_tmp_path, str_key_name,
                             str_key );
    if( flag )
    {
        ret += set_data_by_path( CONF_FILE, str_tmp_path, "wepkey", 
                                 str_key_seq );
    }
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_sec_set_wepkey( unsigned int un_vap_num, 
                             char *str_key_seq,
                             char *str_key_type,
                             char *str_key, 
                             int flag )
{ 	
    int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curkey[70]={0};
    if((res=wireless_sec_get_wepkey( un_vap_num,str_key_seq,str_curkey,70)) == IFERR_NO_ERROR)
    {
        if(strcmp(str_curkey,str_key) == 0)
        {
           return IFERR_ARG_INVALID;
        }
    res = conf_wireless_sec_set_wepkey( un_vap_num, str_key_seq, str_key_type,
                                        str_key, flag );

    if( res != IFERR_NO_ERROR )
    {
        return res;
    }
    
    if( flag )
    {
        if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
    	{
    		return IFERR_FILE_NOT_FOUND;
    	}
    	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
    	{
    		close( fd_mutex );
    		return IFERR_LOCK_FAILED;
    	}
        
        res = ap_wireless_sec_set_wepkey( un_vap_num, str_key_seq, str_key_type,
                                          str_key );
        flock( fd_mutex, LOCK_UN );
        close( fd_mutex );
    }
    }
    return res; 

}

int wireless_sec_get_wepkey_type( unsigned int un_vap_num, 
                                       char *str_key_type, 
                                       unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "wepkeytype",
                             str_key_type , un_size );
}


int wireless_sec_get_wepkey( unsigned int un_vap_num, 
                                  char *str_key_seq,
                                  char *str_key, 
                                  unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    char str_key_name[MAX_STR_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    sprintf( str_key_name, "passkey%s", str_key_seq );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, str_key_name,
                             str_key , un_size );
}


//------- set wep key sequence number -------//
static int ap_wireless_sec_set_wepkey_seq( unsigned int un_vap_num, char *str_key_seq )
{
    char str_old_type[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );

    ret = get_data_by_path( CONF_FILE, str_tmp_path, "security", 
                            str_old_type, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }

    return ap_wireless_sec_set_type( un_vap_num, str_old_type );
}

static int conf_wireless_sec_set_wepkey_seq( unsigned int un_vap_num, char *str_key_seq )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    ret = set_data_by_path( CONF_FILE, str_tmp_path, "wepkey", str_key_seq );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

int wireless_sec_set_wepkey_seq( unsigned int un_vap_num, char *str_key_seq )
{
	int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curkeyseq[MAX_STR_LEN]={0};
	if((res=wireless_sec_get_wepkey_seq( un_vap_num,str_curkeyseq,MAX_STR_LEN))== IFERR_NO_ERROR)
	{
	     if(strcmp(str_curkeyseq,str_key_seq) == 0)
	     {
	         return IFERR_ARG_INVALID;
	     }
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_wireless_sec_set_wepkey_seq( un_vap_num, str_key_seq )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}
    
	res = conf_wireless_sec_set_wepkey_seq( un_vap_num, str_key_seq );

	flock( fd_mutex, LOCK_UN );
	close( fd_mutex );
	}
	return res;
}

int wireless_sec_get_wepkey_seq( unsigned int un_vap_num, 
                                 char *str_key_seq, 
                                 unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "wepkey",
                             str_key_seq , un_size );
}
int wireless_sec_get_wepkey_level( unsigned int un_vap_num, 
                                 char *str_key_level, 
                                 unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    return get_data_by_path( CONF_FILE, str_tmp_path, "wepbit",
                             str_key_level , un_size );
}


/*-----------------------------------------------------*
       wireless security  [vapn] wpakey [string]
*------------------------------------------------------*/

static int ap_wireless_sec_set_wpakey( unsigned int un_vap_num,  char *str_wpakey )
{
    char str_old_type[MAX_STR_LEN];
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );

    ret = get_data_by_path( CONF_FILE, str_tmp_path, "security", 
                            str_old_type, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }

    return ap_wireless_sec_set_type( un_vap_num, str_old_type );
}


static int conf_wireless_sec_set_wpakey( unsigned int un_vap_num, 
                                         char *str_wpakey )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );
    
    ret = set_data_by_path( CONF_FILE, str_tmp_path, "passphrase", str_wpakey);
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;    
}

int wireless_sec_set_wpakey( unsigned int un_vap_num, char *str_wpakey, int flag )
{ 
    int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curwpakey[70]={0};
	if(wireless_sec_get_wpakey( un_vap_num,str_curwpakey,70) == IFERR_NO_ERROR)
	{
	    if(strcmp(str_curwpakey,str_wpakey) == 0)
	    {
		  return IFERR_ARG_INVALID;
	    }
    res = conf_wireless_sec_set_wpakey( un_vap_num, str_wpakey );

    if( res != IFERR_NO_ERROR )
    {
        return res;
    }
    
    if( flag )
    {
        if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
    	{
    		return IFERR_FILE_NOT_FOUND;
    	}
    	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
    	{
    		close( fd_mutex );
    		return IFERR_LOCK_FAILED;
    	}
        
        res = ap_wireless_sec_set_wpakey(un_vap_num, str_wpakey );
        flock( fd_mutex, LOCK_UN );
        close( fd_mutex );
	    }
    }
    return res; 
}

int wireless_sec_get_wpakey( unsigned int un_vap_num, char *str_wpakey,
                             unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    sprintf( str_tmp_path, "/config/network/vap%d", un_vap_num );

    return get_data_by_path( CONF_FILE, str_tmp_path, "passphrase",
                             str_wpakey, un_size );
}

/*-----------------------------------------------------*
         wireless radius  [1/2] authip [ipadress]
*------------------------------------------------------*/
static int conf_wireless_radius_set_authip( char *str_flag, char *str_ipaddr )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius" );
    
    ret = set_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"radiusipb":"radiusip", 
                            str_ipaddr );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_radius_set_authip( char *str_flag, char *str_ipaddr )
{ 
    char str_curipaddr[MAX_STR_LEN]={0};
	if(wireless_radius_get_authip( str_flag,str_curipaddr, MAX_STR_LEN) == IFERR_NO_ERROR)
	{
	    if(strcmp(str_curipaddr,str_ipaddr) == 0)
	    {
			return IFERR_ARG_INVALID;   
	    }
        return conf_wireless_radius_set_authip( str_flag, str_ipaddr );
       }
    system("/usr/sbin/send_trap apShareKeyChange");
	return IFERR_FILE_READ;  
    
}

static int conf_wireless_radius_get_authip( char *str_flag, char *str_ipaddr, 
                                            unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius" );
    
    ret = get_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"radiusipb":"radiusip", 
                            str_ipaddr, un_size );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;   

}

int wireless_radius_get_authip( char *str_flag, char *str_ipaddr,
                                unsigned int un_size )
{
    return conf_wireless_radius_get_authip( str_flag, str_ipaddr, un_size );
}

/*-----------------------------------------------------*
         wireless radius  [1/2] authport  [port]
*------------------------------------------------------*/
static int conf_wireless_radius_set_authport( char *str_flag, char *str_port )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius");
    
    ret = set_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"authportb":"authport", 
                            str_port );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_radius_set_authport( char *str_flag, char *str_port )
{ 

    char str_curport[MAX_STR_LEN]={0};
	if(wireless_radius_get_authport( str_flag,str_curport, MAX_STR_LEN) == IFERR_NO_ERROR)
	{
	    if(strcmp(str_curport,str_port) == 0)
	    {
			return IFERR_ARG_INVALID;   
	    }
        return conf_wireless_radius_set_authport( str_flag, str_port );
	}
	return IFERR_FILE_READ;   
}

static int conf_wireless_radius_get_authport( char *str_flag, char *str_port,
                                              unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius");
    
    ret = get_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"authportb":"authport", 
                            str_port, un_size );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_radius_get_authport( char *str_flag, char *str_port,
                                  unsigned int un_size )
{ 
    return conf_wireless_radius_get_authport( str_flag, str_port, un_size );
}


/*-----------------------------------------------------*
        wireless radius  [1/2]  accountip [port]
*------------------------------------------------------*/

static int conf_wireless_radius_set_acctip( char *str_flag, char *str_ipaddr )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius" );
    
    ret = set_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"radiusaipb":"radiusaip", 
                            str_ipaddr );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_radius_set_acctip( char *str_flag, char *str_ipaddr )
{ 
    char str_curipaddr[MAX_STR_LEN]={0};
	if(wireless_radius_get_acctip( str_flag,str_curipaddr, MAX_STR_LEN) == IFERR_NO_ERROR)
	{
	    if(strcmp(str_curipaddr,str_ipaddr) == 0)
	    {
			return IFERR_ARG_INVALID;   
	    }
        return conf_wireless_radius_set_acctip( str_flag, str_ipaddr );
	}
	return IFERR_FILE_READ;   
}

static int conf_wireless_radius_get_acctip( char *str_flag, char *str_ipaddr,
                                            unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius");
    
    ret = get_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"radiusaipb":"radiusaip", 
                            str_ipaddr, un_size );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_radius_get_acctip( char *str_flag, char *str_ipaddr,
                                unsigned int un_size )
{ 
    return conf_wireless_radius_get_acctip( str_flag, str_ipaddr, un_size );
}

/*-----------------------------------------------------*
        wireless radius  [1/2]  accountport [port]
*------------------------------------------------------*/

static int conf_wireless_radius_set_acctport( char *str_flag, char *str_port )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius" );
    
    ret = set_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"accepportb":"acceptport", 
                            str_port );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_radius_set_acctport( char *str_flag, char *str_port )
{ 
    char str_curport[MAX_STR_LEN]={0};
	if(wireless_radius_get_acctport( str_flag,str_curport, MAX_STR_LEN) == IFERR_NO_ERROR)
	{		  
	    if(strcmp(str_curport,str_port) == 0)
	    {
			return IFERR_ARG_INVALID;   
	    }
        return conf_wireless_radius_set_acctport( str_flag, str_port );
	}
	return IFERR_FILE_READ;
}

static int conf_wireless_radius_get_acctport( char *str_flag, char *str_ipaddr,
                                            unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius" );
    
    ret = get_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"acceptportb":"acceptport", 
                            str_ipaddr,
                            un_size );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_radius_get_acctport( char *str_flag, char *str_ipaddr,
                                unsigned int un_size )
{ 
    return conf_wireless_radius_get_acctport( str_flag, str_ipaddr, un_size );
}

/*-----------------------------------------------------*
        wireless radius [1/2] sharekey [integer]
*------------------------------------------------------*/

static int conf_wireless_radius_set_sharekey( char *str_flag, char *str_key )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius" );
    
    ret = set_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"sharekeyb":"sharekey", 
                            str_key );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_radius_set_sharekey( char *str_flag, char *str_key )
{ 
    char str_curkey[70]={0};
	if(wireless_radius_get_sharekey( str_flag,str_curkey, 70) == IFERR_NO_ERROR)
	{
	    if(strcmp(str_curkey,str_key) == 0)
	    {
			return IFERR_ARG_INVALID;   
	    }
        return conf_wireless_radius_set_sharekey( str_flag, str_key );
	}
	return IFERR_FILE_READ;
}

static int conf_wireless_radius_get_sharekey( char *str_flag, char *str_key,
                                              unsigned int un_size )
{
    char str_tmp_path[MAX_PATH_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_tmp_path, 0, MAX_PATH_LEN );
    sprintf( str_tmp_path, "/config/network/radius" );
    
    ret = get_data_by_path( CONF_FILE, str_tmp_path, 
                            strcmp(str_flag, "1")?"sharekeyb":"sharekey", 
                            str_key, un_size );
    
    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;       

}

int wireless_radius_get_sharekey( char *str_flag, char *str_key, 
                                  unsigned int un_size )
{ 
    return conf_wireless_radius_get_sharekey( str_flag, str_key, un_size );
}

/*------------------------------------------*
             syslog enable[diable]
*-------------------------------------------*/

#define SYSLOG_DISABLE 0
#define SYSLOG_ENABLE 1
static int ap_syslog_set_state( char *str_state )
{
    char str_tmp_cmd[MAX_CMD_LEN];
    char str_ipaddr[MAX_STR_LEN];
    char str_port[MAX_STR_LEN];
    const char *str_xpath = "/config/system/log";
    int ret = XERR_NO_ERROR;
    int res = IFERR_NO_ERROR;
    int flag = SYSLOG_DISABLE;
    
    memset( str_tmp_cmd, 0, MAX_CMD_LEN );
    memset( str_ipaddr, 0, MAX_STR_LEN );
    memset( str_port, 0, MAX_STR_LEN );

    if( 0 == strcmp("enable", str_state) )
    {
        flag = SYSLOG_ENABLE;
    }
    else if( 0 == strcmp("disable", str_state) )
    {
        flag = SYSLOG_DISABLE;
    }
    else
    {
        return IFERR_ARG_WRONG;
    }
    
    ret = get_data_by_path( CONF_FILE, str_xpath, "sip", 
                            str_ipaddr, MAX_STR_LEN );
    ret += get_data_by_path( CONF_FILE, str_xpath, "sport", 
                             str_port, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        if( ret != XERR_NODE_NO_DATA )
        {
            return IFERR_ARG_WRONG;
        }
        flag = SYSLOG_DISABLE;
    }

    if( SYSLOG_ENABLE == flag )
    {
        sprintf( str_tmp_cmd, "{  %s %s "\
                              "&& %s -s 2 -R %s:%s ;}", 
                              KILLALL, SYSLOGD,
                              SYSLOGD, str_ipaddr, str_port );
    }
    else
    {
        sprintf( str_tmp_cmd, "{ %s %s && %s -s 2 ; }",
                              KILLALL, SYSLOGD, SYSLOGD );
    }
    res = system( str_tmp_cmd );
    return (0==res) ? IFERR_NO_ERROR : IFERR_COM_EXEC_FAILED;
} 

static int conf_syslog_set_state( char *str_state )
{
    const char *str_xpath = "/config/system/log";
    char str_isenable[MAX_STR_LEN];
    int ret = XERR_NO_ERROR;

    memset( str_isenable, 0, MAX_STR_LEN );

    if( 0 == strcmp("enable", str_state) )
    {
        sprintf( str_isenable, "yes" );
    }
    else if( 0 == strcmp("disable", str_state) )
    {
        sprintf( str_isenable, "no" );
    }
    else
    {
        return IFERR_ARG_WRONG;
    }
    
    ret = set_data_by_path( CONF_FILE, str_xpath, "enable", str_isenable );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

int syslog_set_state( char *str_state )
{
	int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curstate[MAX_STR_LEN]={0};
	if((res=syslog_get_state(str_curstate,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
	    if(((strstr(str_curstate,"yes")!= NULL) && (strstr(str_state,"enable")!= NULL))
			||((strstr(str_curstate,"no")!= NULL) && (strstr(str_state,"disable")!= NULL)))
	    {
		   return IFERR_NO_ERROR;
	    }
	
	if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
	{
		return IFERR_FILE_NOT_FOUND;
	}
	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
	{
		close( fd_mutex );
		return IFERR_LOCK_FAILED;
	}
	if( (res = ap_syslog_set_state( str_state )) != IFERR_NO_ERROR )
	{
	    flock( fd_mutex, LOCK_UN );
	    close(fd_mutex);
	    return res;
	}
	res = conf_syslog_set_state( str_state );

	flock( fd_mutex, LOCK_UN );
	close( fd_mutex );
	}	
	return res;
}

static int conf_syslog_get_state( char *str_state, 
                                  unsigned int un_size )
{
    const char *str_xpath = "/config/system/log";
    int ret = XERR_NO_ERROR;
    
    ret = get_data_by_path( CONF_FILE, str_xpath, "enable", 
                            str_state, un_size );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

int syslog_get_state( char *str_state, unsigned int un_size )
{
    return conf_syslog_get_state( str_state, un_size );
}

/*------------------------------------------*
      syslog  config serverip  [ipaddr]
*-------------------------------------------*/

static int conf_syslog_set_ip( char *str_ipaddr )
{
    const char *str_xpath = "/config/system/log"; 
    int ret = XERR_NO_ERROR;

    ret = set_data_by_path( CONF_FILE, str_xpath, "sip", str_ipaddr );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

static int ap_syslog_set_ip( char *str_ip )
{
    char str_cur_state[MAX_STR_LEN];
    int ret = XERR_NO_ERROR;

    ret = get_data_by_path( CONF_FILE, "/config/system/log", "enable", 
                            str_cur_state, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }

    return ap_syslog_set_state( str_cur_state );    
}


int syslog_set_ip( char *str_ipaddr, int flag )
{
    int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curipaddr[MAX_STR_LEN]={0};
	if((res=syslog_get_ip(str_curipaddr,MAX_STR_LEN)) == 0)
	{
        if(strcmp(str_curipaddr,str_ipaddr) == 0)
        {
			return IFERR_ARG_INVALID;
        }
	    res = conf_syslog_set_ip( str_ipaddr );

    if( res != IFERR_NO_ERROR )
    {
        return res;
    }
    
    if( flag )
    {
        if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
    	{
    		return IFERR_FILE_NOT_FOUND;
    	}
    	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
    	{
    		close( fd_mutex );
    		return IFERR_LOCK_FAILED;
    	}
        
        res = ap_syslog_set_ip( str_ipaddr );
        flock( fd_mutex, LOCK_UN );
        close( fd_mutex );
	    }
    }

    return res; 

}

static int conf_syslog_get_ip( char *str_ipaddr, unsigned int un_size )
{
    return get_data_by_path( CONF_FILE, "/config/system/log", "sip",
                             str_ipaddr, un_size );
}

int syslog_get_ip( char *str_ipaddr, unsigned int un_size )
{
    return conf_syslog_get_ip( str_ipaddr, un_size );
}


/*------------------------------------------*
        syslog  config serverport  [port]
*-------------------------------------------*/

static int conf_syslog_set_port( char *str_port )
{
    const char *str_xpath = "/config/system/log"; 
    int ret = XERR_NO_ERROR;

    ret = set_data_by_path( CONF_FILE, str_xpath, "sport", str_port );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}

static int ap_syslog_set_port( char *str_port )
{
    char str_cur_state[MAX_STR_LEN];
    int ret = XERR_NO_ERROR;

    ret = get_data_by_path( CONF_FILE, "/config/system/log", "enable", 
                            str_cur_state, MAX_STR_LEN );
    if( ret != XERR_NO_ERROR )
    {
        return IFERR_ARG_NOT_FOUND;
    }

    return ap_syslog_set_state( str_cur_state );    
}

int syslog_set_port( char *str_port, int flag )
{
    int fd_mutex;
	int res = IFERR_NO_ERROR;
	char str_curport[MAX_STR_LEN]={0};
	if((res=syslog_get_port(str_curport,MAX_STR_LEN)) == IFERR_NO_ERROR)
	{
	     if(strcmp(str_curport,str_port) == 0)
	     {
		     return IFERR_ARG_INVALID;
	     }
    res = conf_syslog_set_port( str_port );

    if( res != IFERR_NO_ERROR )
    {
        return res;
    }
    
    if( flag )
    {
        if( -1 == (fd_mutex = open( LOCK_FILE, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR )) )
    	{
    		return IFERR_FILE_NOT_FOUND;
    	}
    	if(flock( fd_mutex, LOCK_EX ) != IFERR_NO_ERROR)
    	{
    		close( fd_mutex );
    		return IFERR_LOCK_FAILED;
    	}
        
        res = ap_syslog_set_port( str_port );
        flock( fd_mutex, LOCK_UN );
        close( fd_mutex );
	    }
    }

    return res; 
}

static int conf_syslog_get_port( char *str_port, unsigned int un_size )
{
    return get_data_by_path( CONF_FILE, "/config/system/log", "sport",
                             str_port, un_size );
}

int syslog_get_port( char *str_port, unsigned int un_size )
{
    return conf_syslog_get_port( str_port, un_size );
}

int snmp_get_trapipaddr(char *str_ipaddr, unsigned int un_size)
{

    return get_data_by_path( CONF_FILE, "/config/administrator/snmp", "snmpip",
                             str_ipaddr, un_size );

}

static int conf_snmp_set_trapipaddr( char *str_ipaddr )
{
    const char *str_xpath = "/config/administrator/snmp"; 
    int ret = XERR_NO_ERROR;

    ret = set_data_by_path( CONF_FILE, str_xpath, "snmpip", str_ipaddr );

    return (XERR_NO_ERROR==ret) ? IFERR_NO_ERROR : IFERR_FILE_WRITE;
}


int snmp_set_trapipaddr(char *str_ipaddr )
{
    char str_curipaddr[MAX_STR_LEN]={0};
	int res = IFERR_NO_ERROR;
	if(snmp_get_trapipaddr(str_curipaddr, MAX_STR_LEN)== IFERR_NO_ERROR)
	{
        if(strcmp(str_curipaddr,str_ipaddr) == 0)
        {
			return IFERR_ARG_INVALID;
        }
		return  conf_snmp_set_trapipaddr( str_ipaddr );
        
	}
    return IFERR_FILE_READ; 
}
