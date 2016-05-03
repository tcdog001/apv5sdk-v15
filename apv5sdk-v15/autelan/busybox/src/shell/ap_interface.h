#ifndef _AP_INTERFACE_H
#define _AP_INTERFACE_H

#define IFERR_NO_ERROR 0
#define IFERR_FILE_NOT_FOUND 1
#define IFERR_FILE_READ       2
#define IFERR_FILE_WRITE      3
#define IFERR_ARG_NOT_FOUND 4
#define IFERR_ARG_WRONG 5
#define IFERR_ARG_NO_DATA 6
#define IFERR_LOCK_FAILED 7
#define IFERR_COM_EXEC_FAILED 8
#define IFERR_COM_CANNOT_EXEC 9
#define IFERR_ARG_INVALID 10

#define MAX_DNS_SERV 2
#define MAX_NTP_SERV 2
#define MAX_VLAN_NUM 8
#define MAX_VAP_NUM 8

int network_set_ip( char *str_ifname, char *str_ipaddr, char *str_netmask);
int network_get_ip( char *str_ipaddr, unsigned int un_size );
int network_set_ifstate( char *str_ifname, char *str_state );
int network_get_gw( char *str_gateway, unsigned int un_size );
int network_set_gw( char *str_gateway );
int network_set_ipmode( char *str_ipmode );
int network_get_ipmode( char *str_ipmode, unsigned int un_size );
int network_get_ipaddr( char *str_ipaddr, unsigned int un_size );
int network_set_dnsmode( char *str_dnsmode );
int network_get_dnsmode( char *str_dnsmode, unsigned int un_size );
int network_set_dns( char *str_ipaddr , unsigned int un_seq );
int network_get_dns( char *str_ipaddr , unsigned int un_seq , 
                     unsigned int un_size);
int network_get_timezone( char *str_timezone , unsigned int un_size);
int network_get_ntp( char *str_ipaddr , unsigned int un_seq , 
                     unsigned int un_size);
int network_set_ntp( char *str_ipaddr , unsigned int un_seq );
int network_set_ntpmode( char *str_state );
int network_get_ntpmode( char *str_ntpmode , unsigned int un_size);
int network_get_defaultntp( char *str_defaultntp , unsigned int un_size);
int network_get_ntptime(void); //wangjr 081221


int wireless_get_enable( unsigned int un_vap_num,char *str_enable, unsigned int un_size);
int wireless_config_enable(unsigned int un_vap_num, char *str_enable );
int wireless_create_vap( char *str_wifi, char *str_ifname );
int wireless_addif_vlan( char *str_ifname, char *str_vlanname );
int wireless_destroy_vap( char *str_ifname );
int wireless_get_status( unsigned int un_vap_num,char *str_status, unsigned int un_size);
int wireless_config_ssid( unsigned int un_vap_num, char *str_ssid );
int wireless_get_ssid( unsigned int un_vap_num,char *str_ssid, unsigned int un_size);
int wireless_get_bssid( unsigned int un_vap_num,char *str_bssid, unsigned int un_size);
int wireless_get_region( unsigned int un_vap_num,char *str_region, unsigned int un_size);
int wireless_config_channel( unsigned int un_vap_num, char *str_channel );
int wireless_get_channel( unsigned int un_vap_num,char *str_channel, unsigned int un_size);
int wireless_config_mode( unsigned int un_vap_num, char *str_mode );
int wireless_get_mode( unsigned int un_vap_num,char *str_mode, unsigned int un_size);
int wireless_config_hidessid( unsigned int un_vap_num, char *str_hidessid );
int wireless_get_broadcastssid( unsigned int un_vap_num,char *str_hidessid, unsigned int un_size);
int wireless_config_rate( unsigned int un_vap_num, char *str_rate );
int wireless_get_rate( unsigned int un_vap_num,char *str_rate, unsigned int un_size);
int wireless_config_bintval(unsigned int un_vap_num, char *str_bintval );
int wireless_get_bintval( unsigned int un_vap_num,char *str_bintval, unsigned int un_size);
int wireless_config_dtim( unsigned int un_vap_num, char *str_dtim );
int wireless_get_dtim( unsigned int un_vap_num,char *str_dtim, unsigned int un_size);
int wireless_config_frag( unsigned int un_vap_num, char *str_frag);
int wireless_get_frag( unsigned int un_vap_num,char *str_frag, unsigned int un_size);
int wireless_config_txpower(unsigned int un_vap_num, char *str_txpower);
int wireless_get_txpower(unsigned int un_vap_num, char *str_txpower, unsigned int un_size);
int wireless_config_rts( unsigned int un_vap_num, char *str_rts);
int wireless_get_rts( unsigned int un_vap_num,char *str_rts, unsigned int un_size);
int wireless_config_wmm( unsigned int un_vap_num, char *str_wmm);
int wireless_get_wmm( unsigned int un_vap_num,char *str_wmm, unsigned int un_size);
int wireless_config_noack( unsigned int un_vap_num, char *str_noack);
int wireless_get_noack( unsigned int un_vap_num,char *str_noack, unsigned int un_size);
int wireless_config_vlan( unsigned int un_vap_num,char *str_enbale);
int wireless_get_vlan( unsigned int un_vap_num,char *str_vlan, unsigned int un_size);
int wireless_config_vlanvid(unsigned int un_vap_num,char *br_name);
int wireless_get_vlanvid( unsigned int un_vap_num,char *str_vlanvid, unsigned int un_size);
int wireless_config_isolation( unsigned int un_vap_num, char *str_isolation);
int wireless_get_isolation( unsigned int un_vap_num,char *str_isolation, unsigned int un_size);

int wireless_config_maxsimultusers(unsigned int un_vap_num, char *str_maxsimultusers);
int wireless_get_maxsimultusers( unsigned int un_vap_num,char *str_maxsimultusers, unsigned int un_size);

int wireless_get_macfilter( unsigned int un_vap_num,char *str_state, unsigned int un_size );
int wireless_security_macfilter( unsigned int un_vap_num,char *str_enbale);
int wireless_get_macfiltermode( unsigned int un_vap_num,char *str_filtermode, unsigned int un_size );
int wireless_security_filtermode( unsigned int un_vap_num,char *str_filtermode);

int wireless_sec_get_macfilter( unsigned int un_vap_num, 
                                char *str_filter, 
                                char *str_macaddr, 
                                unsigned int un_size );
int wireless_security_filter_addmac( unsigned int un_vap_num,
                                     char *str_macn, 
                                     char *str_macaddr);
int wireless_security_filter_delmac( unsigned int un_vap_num,char *str_macn);


int get_vnum_by_vname( char *str_vname );

int wireless_sec_set_type( unsigned int un_vap_num, char *str_type );
int wireless_sec_get_type( unsigned int un_vap_num, char *str_type, 
                           unsigned int un_size );
int wireless_sec_set_mode( unsigned int un_vap_num, char *str_mode,
                           int flag );
int wireless_sec_get_mode( unsigned int un_vap_num, char *str_mode,
                           unsigned int un_size );
int wireless_sec_get_basicmode( unsigned int un_vap_num, char *str_basicmode, unsigned int un_size );
int wireless_sec_get_wpaencrypt( unsigned int un_vap_num, char *str_wpaencrypt, unsigned int un_size );
int wireless_sec_set_encrypt( unsigned int un_vap_num, 
                              char *str_encrypt,
                              int flag );
int wireless_sec_get_encrypt( unsigned int un_vap_num,
                              char *str_encrypt,
                              unsigned int un_size );
int wireless_sec_set_wepkey( unsigned int un_vap_num, 
                             char *str_key_seq,
                             char *str_key_type,
                             char *str_key,
                             int flag );
int wireless_sec_get_wepkey( unsigned int un_vap_num,
                             char *str_key_seq,
                             char *str_key,
                             unsigned int un_size );
int wireless_sec_set_wpakey( unsigned int un_vap_num, char *str_wpakey,
                             int flag );
int wireless_sec_get_wpakey( unsigned int un_vap_num, char *str_wpakey,
                             unsigned int un_size );
int wireless_sec_set_wepkey_seq( unsigned int un_vap_num,
                                 char *str_key_seq );
int wireless_sec_get_wepkey_seq( unsigned int un_vap_num,
                                 char *str_key_seq,
                                 unsigned int un_size );
int wireless_sec_get_wepkey_level( unsigned int un_vap_num, 
                                 char *str_key_level, 
                                 unsigned int un_size );
int wireless_radius_set_authip( char *str_flag, 
                                char *str_ipaddr );
int wireless_radius_get_authip( char *str_flag, 
                                char *str_ipaddr,
                                unsigned int un_size  );
int wireless_radius_set_authport( char *str_flag, 
                                  char *str_port );

int wireless_radius_get_authport( char *str_flag, 
                                  char *str_port,
                                  unsigned int un_size );
int wireless_radius_set_acctip( char *str_flag, 
                                char *str_ipaddr );
int wireless_radius_get_acctip( char *str_flag, 
                                char *str_ipaddr, 
                                unsigned int un_size );
int wireless_radius_set_acctport( char *str_flag, 
                                  char *str_port );
int wireless_radius_get_acctport( char *str_flag,
                                  char *str_port,
                                  unsigned int un_size );
int wireless_radius_set_sharekey( char *str_flag, 
                                  char *str_key );
int wireless_radius_get_sharekey( char *str_flag, 
                                  char *str_key,
                                  unsigned int un_size );


int stp_get_state( char *str_state, unsigned int un_size );
int stp_config_enable( char *str_ifname,char *str_enable);
int stp_get_bridgeprio( char *str_bridgeprio, unsigned int un_size );
int stp_config_bridgeprio( char *str_ifname,char *str_bridgeprio);
int stp_get_fd( char *str_fd, unsigned int un_size );
int stp_config_setfd( char *str_ifname,char *str_setfd);
int stp_get_hello( char *str_hello, unsigned int un_size );
int stp_config_hello( char *str_ifname,char *str_hello);
int stp_get_maxage( char *str_maxage, unsigned int un_size );
int stp_config_maxage( char *str_ifname,char *str_maxage);


int vlan_create( char *str_vlanname);
int vlan_addport( char *str_vlanname,char * str_vlanport);
int vlan_delport( char *str_vlanname,char * str_vlanport);
int vlan_destroy( char *str_vlanname);


int ap_system_upgrade(char *str_ipaddr,char *str_filename);
int ap_system_show(void);
int ap_system_backup(char *str_ipaddr);
int ap_system_restore(char *str_ipaddr,char *str_filename);
int ap_system_reset(void);
int ap_system_reboot(void);
int ap_system_ping(char *str_ipaddr);
int ap_cpuinfo_show(void);
int ap_get_macaddressconnectedWithac(char *str_mac);

int syslog_set_state( char *str_state );
int syslog_get_state( char *str_state, unsigned int un_size );
int syslog_set_ip( char *str_ipaddr, int flag );
int syslog_get_ip( char *str_ipaddr, unsigned int un_size );
int syslog_set_port( char *str_port, int flag );
int syslog_get_port( char *str_port, unsigned int un_size );

int snmp_get_trapipaddr(char *str_ipaddr, unsigned int un_size);
int snmp_set_trapipaddr(char *str_ipaddr );
#endif

