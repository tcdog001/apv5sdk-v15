
 #include <errno.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
#include <assert.h>	 
#include <unistd.h>
//#include <time.h>
#include <stdarg.h>
//#include <fcntl.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

#include "pack.h"
#include "auth.h"
#include "typedef.h"
#include "ctrl_iface.h"
#include "cert_info.h"
#include "debug.h"
#include "proc.h"
#include "init.h"
#include "wai_sta.h"
#include "key_neg.h"

extern int errno;


/*给证书对象绑定操作函数*/
static int  bind_certobj_function(struct cert_obj_st_t *dstobj, int  index)
{
	const struct cert_obj_st_t *x509obj = NULL;

	x509obj = get_cert_obj(index);

	if(x509obj 
		&& x509obj->get_public_key
		&& x509obj->verify_key
		&& x509obj->verify){
		
		dstobj->get_public_key = x509obj->get_public_key;
		dstobj->verify_key = x509obj->verify_key;
		dstobj->verify = x509obj->verify;
		return 0;
	}
	assert(!x509obj);
	assert(!x509obj->get_public_key);
	assert(!x509obj->verify_key);
	assert(!x509obj->verify);
	
	return -1;
}



#ifdef CTRL_UDP
/*创建UDP套接口,并绑定到127.0.0.1:9002*/
int open_socket_for_cgi()
{
	int sockfd, ret;
	struct sockaddr_in saddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		goto end;
	}

	bzero(&saddr, sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(CGI_PORT);
	
	ret = bind(sockfd, (struct sockaddr*) &saddr, sizeof(struct sockaddr_in));
	if (ret == -1) {
		close(sockfd);
		sockfd = -1;
	}

end:
	return sockfd;
}
/*****************************************************************************
	Description  : process data received by cgi_udp_sk.
	Input		 : mesg		-- received data
				 : msglen   -- mesg length
	return		 : 0 ---- OK.  other ----- error.

 -----------------------------------------------------------------------------
 mesg format:
	0          2          4         6           8
	+----------+----------+---------+-----------+-----------------+
	|ver(2)    |cmd(2)    |unused(2)|datalen(2) | data            |
	+----------+----------+---------+-----------+-----------------+
	                                            |<--- datalen --->|
 *****************************************************************************/
int save_certificate(const char *fname, unsigned char *fcontent, int flen)
{
	FILE *f;
	int ret = 0;

	f=fopen(fname,"wb");

	if(f == NULL)
	{
		ret = 1;
	}
	/*写文件*/
	if(fwrite(fcontent, flen ,1,f) != 1)
	{
		ret = 2;
	}
	
	fclose(f);

	return ret;
}
/*检查证书内容*/
int Process_CGI_checkcert(u8 *mesg, int msglen)
{
	u16 	index = 0;
	u16	certlen = 0;
	int	ret = -1;
	char *cert_path = "/tmp/user_$.cer";

	struct cert_obj_st_t certobj;

	memset(&certobj, 0, sizeof(certobj));
	
	do{
		
		/*取证书类型号*/	
		GET16((mesg+4), index);

		if(index != 1){
			DPrintf("[WAPID]:: in %s:%d certificate index(%d) is too big\n", __func__, __LINE__, index);
			break;
		}
		/*取证书长度*/
		GET16((mesg+6),certlen);
		
		if (certlen + 8 != msglen)	{
			DPrintf("[WAPID]:: in %s:%d certificate datalen(%d) is wrong\n", __func__, __LINE__, certlen);
			ret = -2;
			break;
		}
		if(save_certificate(cert_path, mesg+6, certlen) != 0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		
		if(bind_certobj_function(&certobj, index) != 0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			break;
		}
		ret = load_x509(cert_path, &certobj);
	}while(0);
	x509_free_obj_data(&certobj);
	return ret;
}


/*处理CGI的消息*/
int Process_CTRL_message(u8 *recv_buf, int readlen, int *cgicmd)
{
	int 	ret = 0;
	u16	cmd = 0;

	/*取消息类型*/
	GET16((recv_buf+2), cmd);
	do{
		if (cmd == CGI_CMD_RELOAD)/*wapid 重新加载消息*/
		{
			/*构造reload 消息响应包*/
			
			SET16((recv_buf+2), CGI_RELOAD_RESPONSE);
			SET16((recv_buf+6), 0);
			ret = 8;
			usleep(2000);
			break;
		} 
		else if (cmd == CGI_CMD_CHECKCERT)/*证书检查消息*/
		{
			ret = Process_CGI_checkcert(recv_buf, readlen);
			/*构造证书检查 消息响应包*/
			SET16((recv_buf+2), CGI_CHECK_RESPONSE);
			SET16((recv_buf+6), 2);
			if (ret == 0)
				memset(recv_buf+8, 0x0000, 2);
			else
				memset(recv_buf+8, 0xFFFF, 2);
			ret = 10;
			break;
		}
	}while(0);	
	*cgicmd = cmd;
	return ret;
}
#endif //CTRL_UDP


#ifdef CTRL_UNIX

int open_socket_for_ctrl()
{
	int s = -1;

	s = socket(PF_UNIX, SOCK_DGRAM, 0);
	
	return s;
}
static char * wapid_ctrl_iface_path(char *ctrl_interface, char *iface)
{
	char *buf;
	size_t len;

	if (ctrl_interface == NULL)
		return NULL;

	len = strlen(ctrl_interface) + strlen(iface) + 2;
	buf = malloc(len);
	if (buf == NULL)
		return NULL;

	snprintf(buf, len, "%s/%s",ctrl_interface, iface);
	buf[len - 1] = '\0';
        
	return buf;
}

static void wapid_create_directory_path(char *filepath)
{
        /* create leading directories needed for file. Ignore errors */
        /* caution: filepath must be temporarily writeable */
        char *slash = filepath;
        for (;;) {
                slash = strchr(slash+1, '/');
                if (slash == NULL) break;
                *slash = 0;     /* temporarily terminate */
                (void) mkdir(filepath, 0777);
                *slash = '/';   /* restore */
        }
        return;
}


int wapid_ctrl_iface_init(struct eloop_data *eloop)
{
	struct sockaddr_un addr;
	int s = eloop->ctrl_sk;
	char *fname = NULL;
	static char *iface="ath0";

	if (eloop->ctrl_interface == NULL)
		return 0;

	if (mkdir(eloop->ctrl_interface, S_IRWXU | S_IRWXG) < 0) {
		if (errno == EEXIST) {
			DPrintf("[WAPID]:: Using existing control "
				   "interface directory.\n");
		} else {
			perror("mkdir[ctrl_interface]");
			goto fail;
		}
	}
#if  0
	if (hapd->conf->ctrl_interface_gid_set &&
	    chown(hapd->conf->ctrl_interface, 0,
		  hapd->conf->ctrl_interface_gid) < 0) {
		perror("chown[ctrl_interface]");
		return -1;
	}
#endif
	if (strlen(eloop->ctrl_interface) + 1 + strlen(iface)  >= sizeof(addr.sun_path))
		goto fail;

	//s = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (s < 0) {
		perror("socket(PF_UNIX)");
		goto fail;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	fname = wapid_ctrl_iface_path(eloop->ctrl_interface, iface);
	if (fname == NULL)
		goto fail;
        /* An existing socket could indicate that we are trying to run
         * program twice, but more likely it is left behind when a previous
         * instance crashed... remove it to be safe.
         */
        (void) unlink(fname);
        wapid_create_directory_path(fname);
	strncpy(addr.sun_path, fname, sizeof(addr.sun_path));
	if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("bind(PF_UNIX)");
		goto fail;
	}
#if 0
	if (hapd->conf->ctrl_interface_gid_set &&
	    chown(fname, 0, hapd->conf->ctrl_interface_gid) < 0) {
		perror("chown[ctrl_interface/ifname]");
		goto fail;
	}
#endif
	if (chmod(fname, S_IRWXU | S_IRWXG) < 0) {
		perror("chmod[ctrl_interface/ifname]");
		goto fail;
	}
	DPrintf("Sk = %d, fname='%s'\n", s, fname);
	free(fname);
	return 0;

fail:
	if (fname) {
		unlink(fname);
		free(fname);
	}
	return -1;
}


void wapid_ctrl_iface_deinit(struct eloop_data *eloop)
{
	static char *iface="ath0";
	if (eloop->ctrl_sk > -1) {
		char *fname;
		close(eloop->ctrl_sk);
		eloop->ctrl_sk = -1;
		fname = wapid_ctrl_iface_path(eloop->ctrl_interface, iface);
		if (fname)
			unlink(fname);
		free(fname);

		if (eloop->ctrl_interface &&
		    rmdir(eloop->ctrl_interface) < 0) {
			if (errno == ENOTEMPTY) {
				DPrintf("WAPID]::  Control interface "
					   "directory not empty - leaving it "
					   "behind");
			} else {
				perror("rmdir[ctrl_interface]");
			}
		}
		if(eloop->ctrl_interface){
			free(eloop->ctrl_interface);
			eloop->ctrl_interface = NULL;
		}
		
	}

}
//static int wapid_ctrl_reload_config();
//static int wapid_ctrl_start();
//static int wapid_ctrl_restart();
static int wapid_ctrl_stop(struct eloop_data *eloop);
static int wapid_ctrl_iface_status(struct eloop_data *eloop,char *buf, int buflen);
//static int wapid_ctrl_get_key(struct eloop_data *eloop, const char *key_name, char *buf, int buflen);
static int wapid_ctrl_check_cert(const char *cert_file);
#ifdef DOT11_WAPI_MIB
extern int wapid_ctrl_getmib(struct eloop_data *eloop, const char *req, char *buf, int buf_len);
#endif

static int os_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list ap;
	int ret;

	/* See http://www.ijs.si/software/snprintf/ for portable
	 * implementation of snprintf.
	 */

	va_start(ap, format);
	ret = vsnprintf(str, size, format, ap);
	va_end(ap);
	if (size > 0)
		str[size - 1] = '\0';
	return ret;
}

static int wapid_ctrl_iface_status(struct eloop_data *eloop,char *buf, int buflen)
{
	struct wapid_interfaces *user = NULL;
	apdata_info *pap = NULL;
	struct auth_sta_info_t *sta = NULL;
	
	char *pos, *end,tmp[256]="";
	int ret = 0;
	int stas = 0;
	unsigned int i = 0;

	pos = buf;
	end = buf+buflen;
	user = eloop->vap_user;

	for (;user; user = user->next)
	{
		pap = user->wapid;
		sta = pap->sta_info;
		ret = os_snprintf(pos, end-pos, "%s{\n",  user->identity); 
		if (ret < 0 || ret >= end - pos){
			return pos - buf;
		}
		pos += ret;
		
		ret = os_snprintf(pos, end - pos, "	bssid=" MACSTR "\n", MAC2STR(pap->macaddr));
		if (ret < 0 || ret >= end - pos){
			return pos - buf;
		}
		pos += ret;

#if 0
		ret = os_snprintf(pos, end-pos, "	SSID=%s\n", user->ssid);
		if (ret < 0 || ret >= end - pos){
			return pos - buf;
		}
		pos += ret;
#endif

		
		if(user->wapid->wai_policy &0x04){
			ret = os_snprintf(pos, end-pos, "	Authenication_suit=%s\n", "WAPI-PSK");
			if (ret < 0 || ret >= end - pos){
				return pos - buf;
			}
			pos += ret;
			
			ret = os_snprintf(pos, end-pos, "	Pre-PSK=");
			if (ret < 0 || ret >= end - pos){
				return pos - buf;
			}
			pos += ret;
			
			if(user->psk_type == 0){
				sprintf(tmp, "%s",user->password );
			}else{
				for(i=0; i<user->password_len; i++){
					sprintf(tmp+i*2, "%02X:", user->password[i]);
				}
			}
			ret = os_snprintf(pos, end-pos, "%s\n", tmp);
			if (ret < 0 || ret >= end - pos){
				return pos - buf;
			}
			pos += ret;
		}else if(user->wapid->wai_policy &0x08){
			ret = os_snprintf(pos, end-pos, "	Authenication_suit=WAPI-Cert\n");
			if (ret < 0 || ret >= end - pos){
				return pos - buf;
			}
			pos += ret;
		}else {
			ret = os_snprintf(pos, end-pos, "	Authenication_suit=Open\n");
			if (ret < 0 || ret >= end - pos){
				return pos - buf;
			}
			pos += ret;
		}
		if(user->wapid->wai_policy &0x03){
			ret = os_snprintf(pos, end-pos, "	Crypto_Suit=SMS4-OFB-CBC\n");
			if (ret < 0 || ret >= end - pos){
				return pos - buf;
			}
			pos += ret;
			
			ret = os_snprintf(pos, end-pos, "	MSK_REKEY=%d\n", user->wapid->msk_rekey_period);
			if (ret < 0 || ret >= end - pos){
				return pos - buf;
			}
			pos += ret;

			ret = os_snprintf(pos, end-pos, "	USK_REKEY=%d\n", user->wapid->usk_rekey_period);
			if (ret < 0 || ret >= end - pos){
				return pos - buf;
			}
			pos += ret;
		}
		for(i=0; i<MAX_AUTH_MT_SIMU; i++){
			if(sta[i].status != NO_AUTH)
				stas++;
		}
		ret = os_snprintf(pos, end-pos, "	Association stations=%d\n	}\n", stas);
		if (ret < 0 || ret >= end - pos){
			return pos - buf;
		}
		pos += ret;
	}
	return pos - buf;
}

static int wapid_ctrl_stop(struct eloop_data *eloop)
{
	struct wapid_interfaces *user = NULL;
	apdata_info *pap = NULL;
	struct auth_sta_info_t *sta = NULL;

	eloop->stop = 1;
	user = eloop->vap_user;
	for (;user; user = user->next)
	{
		int i = 0;
		
		pap = user->wapid;
		sta = pap->sta_info;
		
		pap->wai_policy = 0;
		set_wapi(pap);
		
		for(i=0; i<MAX_AUTH_MT_SIMU; i++){
			if(sta[i].status !=NO_AUTH){
				sta_deauth(sta[i].mac, IEEE80211_MLME_DEAUTH, pap);
				reset_sta_info(&sta[i], pap);
			}
		}
		
		memset(pap->wie_ae, 0, 255);
	#ifdef LSDK6	
		/*send IE buf to driver via ioctl*/
		ap_setdriver_wie(pap, IEEE80211_APPIE_FRAME_BEACON);
		ap_setdriver_wie(pap, IEEE80211_APPIE_FRAME_PROBE_RESP);
	#endif
	}	
	return 0;
}

static int wapid_ctrl_check_cert(const char *cert_file)
{
	int ret = 0;
	struct cert_obj_st_t certobj;

	memset(&certobj, 0, sizeof(certobj));
	
	ret = bind_certobj_function(&certobj, 1);

	if(ret == 0){
		ret =  load_x509(cert_file, &certobj);
	}
	x509_free_obj_data(&certobj);

	if(ret != 0){
		DPrintf("error at %s:%d\n", __func__, __LINE__);
	}
	return ret;
}
static int wapid_ctrl_rekey(struct eloop_data *eloop, const char *txtaddr, char *buf, int buf_len)
{
	unsigned char mac[WLAN_ADDR_LEN] = {0,};
	struct auth_sta_info_t * sta = NULL;
	
	if((strlen(txtaddr) < 17) ||(hwaddr_aton(txtaddr, mac) !=0)){
		return -1;
	}
	if(eloop->stop){
		return -1; // "WAPID STOP"
	}
	sta = get_sta(eloop, mac);
	if(!sta){
		return -1;//"NO STA"
	}
	if(sta->status != MT_AUTHENTICATED){
		return -1; //STA NO AUTHENTICATED
	}
	usk_rekey_handle(sta);
	return 0;
}
int wapid_ctrl_handle(int sock, struct eloop_data *eloop)
{
	char buf[256];
	int res = -1;
	struct sockaddr_un from;
	socklen_t fromlen = sizeof(from);
	char *reply;
	const int reply_size = 4096;
	int reply_len;
	
	res = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *) &from, &fromlen);
	if (res < 0) {
		perror("recvfrom(ctrl_iface)");
		return res;
	}
	buf[res] = '\0';

/*for cmd getmib, bin buf can't be printed*/
//	wapi_str_dump("RX ctrl CMD", buf, res);

	reply = malloc(reply_size);
	if (reply == NULL) {
		sendto(sock, "FAIL\n", 5, 0, (struct sockaddr *) &from, fromlen);
		return res;
	}

	memcpy(reply, "OK\n", 3);
	reply_len = 3;
	
	if (strcmp(buf, "PING") == 0) {
		memcpy(reply, "PONG\n", 5);
		reply_len = 5;
	} else if (strncmp(buf, "STATUS", 6) == 0) {
		reply_len = wapid_ctrl_iface_status(eloop, reply, reply_size);
	}else if ((strncmp(buf, "START", 5) == 0) 
		||(strncmp(buf, "RESTART", 7) == 0)) {
		res = CGI_CMD_RELOAD;
		eloop->stop = 0;
	} else if (strncmp(buf, "STOP", 4) == 0) {
		if (wapid_ctrl_stop(eloop))
			reply_len = -1;
	}else if (strncmp(buf, "REKEY ", 6) == 0) {
		reply_len = wapid_ctrl_rekey(eloop, buf+6, reply, reply_size);
	}else if (strncmp(buf, "CHECKCERT ", 10) == 0) {
		if (wapid_ctrl_check_cert(buf+10))
			reply_len = -1;
#ifdef DOT11_WAPI_MIB
	}else if (strncmp(buf, "GETMIB",6 ) == 0) {
		reply_len=wapid_ctrl_getmib(eloop,buf+6,reply,reply_size);
#endif
	}else {
		memcpy(reply, "UNKNOWN COMMAND\n", 16);
		reply_len = 16;
	}

	if (reply_len < 0) {
		memcpy(reply, "FAIL\n", 5);
		reply_len = 5;
	}
	sendto(sock, reply, reply_len, 0, (struct sockaddr *) &from, fromlen);
	free(reply);
	return res;
}

#endif
