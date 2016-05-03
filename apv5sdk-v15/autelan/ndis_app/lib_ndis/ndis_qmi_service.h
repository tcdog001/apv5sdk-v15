
#ifndef NDIS_QMI_SERVICE_H
#define NDIS_QMI_SERVICE_H

#define NDIS_IP_LEN_MAX 18

typedef struct ndis_ipinfo_t
{
	int  i32status;   //CONNECTING, CONNECTED,DISCONNECTED
    char wan_ip[NDIS_IP_LEN_MAX];//valid when status is CONNECTED
    char gateway[NDIS_IP_LEN_MAX];//valid when status is CONNECTED
    char dns1[NDIS_IP_LEN_MAX];//valid when status is CONNECTED
    char dns2[NDIS_IP_LEN_MAX];//valid when status is CONNECTED
    char mask[NDIS_IP_LEN_MAX];//valid when status is CONNECTED
} ndis_ipinfo;
#define NDIS_CONNECTING         0X001
#define NDIS_CONNECTED          0X002
#define NDIS_DISCONNECTED       0X004


#define INVALID_DEV_HANDLE      0XFFFFFFFF
#define COMMUNICATE_DEV_FAIL    0XFFFFFFFE
#define CONNECTION_TIME_OUT     0XFFFFFFFD
#define FAILED_GET_IPADDR       0XFFFFFFFC
//#define QMI_DAIL_FAILED         0XFF00FF00  // 
#define DEVICE_STATUS_ERR       0XFFFFFFFB
//#define   
/********************************************************************
 *
 *         Name:  ndis_get_lib_version
 *  Description:  return the ndis qmi dail lib version
 *        Input:  ndis_fd:the ndis net operate file handle
 *                i32len :the version char buffer size;
 *       Output:  version:(64 length)the lib version returned.
 *       Return:  other : error code.
 *                0  : call success.
 *        Notes:  
 ********************************************************************/
int ndis_get_lib_version(int ndis_fd, char* version, int i32len);
/********************************************************************
 *
 *         Name:  ndis_connect
 *  Description:  connect to internet thought ndis net
 *        Input:  ndis_fd:the ndis net operate file handle
 * 				  apn      : apn name.
 *                username : user name 
 *  			  passwd   : password
 *	    		  auth     : auth type: 1----PAP;2----CHAP;3----PAP and CHAP
 *                           default: 0
 *       Output:  null
 *       Return:  >0 :  qmi error code-----dail failed.
 *                0  :  call success.
 *                <0 :  error code
 *        Notes:  
 ********************************************************************/
int ndis_connect(int ndis_fd, char *apn, char *username, char *passwd, int auth);
/********************************************************************
 *
 *         Name:  ndis_disconnect
 *  Description:  disconnect from internet thought ndis net
 *  			  if qmi error num==9 invalid qmi handle,do not process it.
 *        Input:  ndis_fd:the ndis net operate file handle
 *       Output:  null
 *       Return:  other : error code.
 *                0  : call success.
 *        Notes:  
 ********************************************************************/
int ndis_disconnect(int ndis_fd);
/********************************************************************
 *
 *         Name:  ndis_get_status
 *  Description:  get the ndis dail status
 *        Input:  ndis_fd:the ndis net operate file handle
 *       Output:  pipinfo:the ipinfo buffer
 *       Return:  other : error code.
 *                0  : call success.
 *        Notes:  
 ********************************************************************/
int ndis_get_status(int ndis_fd, ndis_ipinfo *pipinfo);
/********************************************************************
 *
 *         Name:  ndis_open
 *  Description:  open ndis dail port.
 *        Input:  ndis_fd:the ndis net operate file handle
 *       Output:  null
 *       Return:  -1 : call faled.
 *                >0 : ndis net dev handle.
 *        Notes:  
 ********************************************************************/
int ndis_open();
/********************************************************************
 *
 *         Name:  ndis_close
 *  Description:  ndis dail close
 *        Input:  ndis_fd:the ndis net operate file handle
 *       Output:  null
 *       Return:  other : error code.
 *                0  : call success.
 *        Notes:  
 ********************************************************************/
int ndis_close(int ndis_fd);
/********************************************************************
 *
 *         Name:  ndis_qmi_connect
 *  Description:  connect to internet thought ndis net, 
 *  			  if qmi error num==9 invalid qmi handle,do not process it.
 *        Input:  ndis_fd:the ndis net operate file handle
 * 				  apn      : apn name.
 *                username : user name 
 *  			  passwd   : password
 *	    		  auth     : auth type: 1----PAP;2----CHAP;3----PAP and CHAP
 *                           default: 0
 *       Output:  null
 *       Return:  >0 :  qmi error code-----dail failed.
 *                0  :  call success.
 *                <0 :  error code
 *        Notes:  
 ********************************************************************/
int ndis_qmi_connect(int ndis_fd, char *apn, char *username, char *passwd, int auth);
/********************************************************************
 *
 *         Name:  ndis_go_active
 *  Description:  set link active
 *        Input:  null
 *       Output:  null
 *       Return:  -1 : call faled.
 *                0  : call success.
 *        Notes:  
 ********************************************************************/
int ndis_go_active(int ndis_fd);
/********************************************************************
 *
 *         Name:  ndis_get_clientID
 *  Description:  get client ID 
 *        Input:  null
 *       Output:  null
 *       Return:  -1 : call faled.
 *                0  : call success.
 *        Notes:  
 ********************************************************************/
 int ndis_get_clientID(int ndis_fd);
 /********************************************************************
 *
 *         Name:  ndis_exit
 *  Description:  ndis dail close
 *        Input:  ndis_fd:the ndis net operate file handle
 *       Output:  null
 *       Return:  -1 : call faled.
 *                0  : call success.
 *        Notes:  
 ********************************************************************/
int ndis_exit(int ndis_fd);
/********************************************************************
 *
 *         Name:  ndis_re_open
 *  Description:  open ndis dail port.
 *        Input:  null
 *       Output:  null
 *       Return:  -1 : call faled.
 *                >0 : ndis net dev handle.
 *        Notes:  
 ********************************************************************/
int ndis_re_open(int ndis_fd);
#endif
