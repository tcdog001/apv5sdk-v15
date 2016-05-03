#include "apnp_util.h"
#include "apnp_log.h"

#define DEFAULT_WEB_DIR		"/tmp/web"
#define DESC_URL_SIZE		200
#define MONITORSERVTYPE		"urn:schemas-upnp-org:service:tvcontrol:1"
							


extern UpnpDevice_Handle device_handle;

ithread_mutex_t MoniterMutex;

//extern int UpnpSdkInit = 0;

int MonitorPowerOn(void) 
{
	int ret = 0;

	ApnpDebugLog("Monitor Power On\n");
#if 0
	char cmd[256] = {0};

	snprintf(cmd, sizeof(cmd)-1, 
			 "sudo /home/pi/demo1.0/mjpgd start");
	ret = system(cmd);
	ret = WEXITSTATUS(ret);

	fprintf(stderr, "cmd=%s, ret=%d\n", cmd, ret);
#endif
	return ret;
}

int MonitorPowerOff(void) 
{
	int ret = 0;

	ApnpDebugLog("Monitor Power On\n");
#if 0
	char cmd[256] = {0};

	snprintf(cmd, sizeof(cmd)-1, 
			 "sudo /home/pi/demo1.0/mjpgd stop");
	ret = system(cmd);
	ret = WEXITSTATUS(ret);

	fprintf(stderr, "cmd=%s, ret=%d\n", cmd, ret);
#endif
	return ret;
}


char *Util_GetFirstDocumentItem(IXML_Document *doc, const char *item)
{
	IXML_NodeList *nodeList = NULL;
	IXML_Node *textNode = NULL;
	IXML_Node *tmpNode = NULL;
	char *ret = NULL;

	nodeList = ixmlDocument_getElementsByTagName(doc, (char *)item);
	if (nodeList) {
		tmpNode = ixmlNodeList_item(nodeList, 0);
		if (tmpNode) {
			textNode = ixmlNode_getFirstChild(tmpNode);
			if (!textNode) {
			//	SampleUtil_Print("%s(%d): (BUG) ixmlNode_getFirstChild(tmpNode) returned NULL\n",
			//		__FILE__, __LINE__); 
				ret = strdup("");
				goto epilogue;
			}
			ret = strdup(ixmlNode_getNodeValue(textNode));
			if (!ret) {
			//	SampleUtil_Print("%s(%d): ixmlNode_getNodeValue returned NULL\n",
				//	__FILE__, __LINE__); 
				ret = strdup("");
			}
		} //else
			//SampleUtil_Print("%s(%d): ixmlNodeList_item(nodeList, 0) returned NULL\n",
			//	__FILE__, __LINE__);
	} //else
		//SampleUtil_Print("%s(%d): Error finding %s in XML Node\n",
			//__FILE__, __LINE__, item);

epilogue:
	if (nodeList)
		ixmlNodeList_free(nodeList);

	return ret;
}
static void Monitor_get_ap_ip(char* value,char* str)
{
	get_ap_ip(str);
	apnp_strdeal(value,str);
}

static void Monitor_get_ap_mac(char* value,char* str)
{
	get_ap_mac(str);
	apnp_strdeal(value,str);
}

static void Monitor_get_ap_name(char* value,char* str)
{
	get_ap_name(str);
	apnp_strdeal(value,str);
}

static void Monitor_get_ap_apname(char* value,char* str)
{
	get_ap_apname(str);
	apnp_strdeal(value,str);
}

static void Monitor_get_ap_soft_version(char* value,char* str)
{
	get_ap_soft_version(str);
	apnp_strdeal(value,str);
}

static void Monitor_get_ap_radio_count(char* value,char* str)
{
	get_ap_radio_count(str);
	apnp_strdeal(value,str);
}

static void Monitor_get_ap_stacount(char* value,char* str)
{
	get_ap_stacount(str);
	apnp_strdeal(value,str);
}

static void Monitor_get_ap_radioif(char* value,char* str)
{
	get_ap_radioif(str);
	apnp_strdeal(value,str);
}






static void Monitor_get_radio_id(IXML_Document * in,char* value,char* str)
{
	const char* radio = NULL;
	if (!(radio = Util_GetFirstDocumentItem(in, "Radio"))) {
		return ;
	}
	else
		get_radio_id(radio,str);
		apnp_strdeal(value,str);
		free(radio);
	
}

static void Monitor_get_radio_channel(IXML_Document * in,char* value,char* str)
{
	const char* radio = NULL;
	if (!(radio = Util_GetFirstDocumentItem(in, "Radio"))) {
		return ;
	}
	else
		get_radio_channel(radio,str);
		apnp_strdeal(value,str);
		free(radio);
	
}

static void Monitor_get_radio_maxassoc(IXML_Document * in,char* value,char* str)
{
	const char* radio = NULL;
	if (!(radio = Util_GetFirstDocumentItem(in, "Radio"))) {
		return ;
	}
	else
		get_radio_maxassoc(radio,str);
		apnp_strdeal(value,str);
		free(radio);
	
}

static void Monitor_get_radio_mode(IXML_Document * in,char* value,char* str)
{
	const char* radio = NULL;
	if (!(radio = Util_GetFirstDocumentItem(in, "Radio"))) {
		return ;
	}
	else
		get_radio_mode(radio,str);
		apnp_strdeal(value,str);
		free(radio);
	
}

static void Monitor_get_radio_power(IXML_Document * in,char* value,char* str)
{
	const char* radio = NULL;
	if (!(radio = Util_GetFirstDocumentItem(in, "Radio"))) {
		return ;
	}
	else
		get_radio_power(radio,str);
		apnp_strdeal(value,str);
		free(radio);
	
}

static void Monitor_get_radio_rate(IXML_Document * in,char* value,char* str)
{
	const char* radio = NULL;
	if (!(radio = Util_GetFirstDocumentItem(in, "Radio"))) {
		return ;
	}
	else
		get_radio_rate(radio,str);
		apnp_strdeal(value,str);
		free(radio);
	
}

static void Monitor_get_radio_wlancount(IXML_Document * in,char* value,char* str)
{
	const char* radio = NULL;
	if (!(radio = Util_GetFirstDocumentItem(in, "Radio"))) {
		return ;
	}
	else
		get_radio_wlancount(radio,str);
		apnp_strdeal(value,str);
		free(radio);
	
}

static void Monitor_get_radio_wlanif(IXML_Document * in,char* value,char* str)
{
	const char* radio = NULL;
	if (!(radio = Util_GetFirstDocumentItem(in, "Radio"))) {
		return ;
	}
	else
		get_radio_wlanif(radio,str);
		apnp_strdeal(value,str);
		free(radio);
	
}

static void Monitor_get_wlan_essid(IXML_Document * in,char* value,char* str)
{
	const char* wlan = NULL;
	if (!(wlan = Util_GetFirstDocumentItem(in, "Wlan"))) {
		return ;
	}
	else
		get_wlan_essid(wlan,str);
		apnp_strdeal(value,str);
		free(wlan);
	
}

static void Monitor_get_wlan_name(IXML_Document * in,char* value,char* str)
{
	const char* wlan = NULL;
	if (!(wlan = Util_GetFirstDocumentItem(in, "Wlan"))) {
		return ;
	}
	else
		get_wlan_name(wlan,str);
		apnp_strdeal(value,str);
		free(wlan);
	
}

static void Monitor_get_wlan_securitykey(IXML_Document * in,char* value,char* str)
{
	const char* wlan = NULL;
	if (!(wlan = Util_GetFirstDocumentItem(in, "Wlan"))) {
		return ;
	}
	else
		get_wlan_securitykey(wlan,str);
		apnp_strdeal(value,str);
		free(wlan);
	
}

static void Monitor_get_wlan_securitytype(IXML_Document * in,char* value,char* str)
{
	const char* wlan = NULL;
	if (!(wlan = Util_GetFirstDocumentItem(in, "Wlan"))) {
		return ;
	}
	else
		get_wlan_securitytype(wlan,str);
		apnp_strdeal(value,str);
		free(wlan);
	
}

int MonitorHandleActionRequest(struct Upnp_Action_Request *ca_event)
{
	if (NULL == ca_event) {
		return 1;
	}
	const char *actionName = NULL;
	char str[256] = {0};
	char value[256] = {0};
	
	actionName = ca_event->ActionName;
	ca_event->ErrCode = 0;
	fprintf(stderr, "action name=%s\n", actionName);

	if (0 == strcmp("PowerOn", actionName)) {
		MonitorPowerOn();
		UpnpAddToActionResponse(&ca_event->ActionResult, "PowerOn",
					    MONITORSERVTYPE,
					    "Power", "1");
	} else if (0 == strcmp("PowerOff", actionName)) {
		MonitorPowerOff();
		UpnpAddToActionResponse(&ca_event->ActionResult, "PowerOff",
					    MONITORSERVTYPE,
					    "Power", "0");
	} else if (0 == strcmp("GetAPIp", actionName)) {
		Monitor_get_ap_ip(value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetAPIp",
					    MONITORSERVTYPE,
					    "Ip", value);
	}
	else if (0 == strcmp("GetAPMac", actionName)) {
		Monitor_get_ap_mac(value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetAPMac",
					    MONITORSERVTYPE,
					    "Mac", value);
	}
	else if (0 == strcmp("GetAPName", actionName)) {
		Monitor_get_ap_name(value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetAPName",
					    MONITORSERVTYPE,
					    "Name", value);
	}
	else if (0 == strcmp("GetAPAPName", actionName)) {
		Monitor_get_ap_apname(value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetAPAPName",
					    MONITORSERVTYPE,
					    "APName", &value[2]);
	}
	else if (0 == strcmp("GetAPVersion", actionName)) {
		Monitor_get_ap_soft_version(value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetAPVersion",
					    MONITORSERVTYPE,
					    "Version", value);
	}
	else if (0 == strcmp("GetAPRadioC", actionName)) {
		Monitor_get_ap_radio_count(value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetAPRadioC",
					    MONITORSERVTYPE,
					    "RadioC", value);
	}
	else if (0 == strcmp("GetAPRadioIf", actionName)) {
		Monitor_get_ap_radioif(value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetAPRadioIf",
					    MONITORSERVTYPE,
					    "RadioIf", value);
	}
	else if (0 == strcmp("GetAPStaC", actionName)) {
		Monitor_get_ap_stacount(value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetAPStaC",
					    MONITORSERVTYPE,
					    "StaC", value);
	}else if (0 == strcmp("GetRadioId", actionName)) {
		Monitor_get_radio_id(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetRadioId",
					    MONITORSERVTYPE,
					    "RadioId", value);
	}else if (0 == strcmp("GetRadioCh", actionName)) {
		Monitor_get_radio_channel(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetRadioCh",
					    MONITORSERVTYPE,
					    "RadioCh", value);
	}else if (0 == strcmp("GetRadioMode", actionName)) {
		Monitor_get_radio_mode(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetRadioMode",
					    MONITORSERVTYPE,
					    "RadioMode", value);
	}
	else if (0 == strcmp("GetRadioRate", actionName)) {
		Monitor_get_radio_rate(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetRadioRate",
					    MONITORSERVTYPE,
					    "RadioRate", value);
	}else if (0 == strcmp("GetRadioPower", actionName)) {
		Monitor_get_radio_power(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetRadioPower",
					    MONITORSERVTYPE,
					    "RadioPower", value);
	}else if (0 == strcmp("GetRadioWlanC", actionName)) {
		Monitor_get_radio_wlancount(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetRadioWlanC",
					    MONITORSERVTYPE,
					    "RadioWlanC", value);
	}
	else if (0 == strcmp("GetRadioMaxAC", actionName)) {
		Monitor_get_radio_maxassoc(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetRadioMaxAC",
					    MONITORSERVTYPE,
					    "RadioMaxAC", value);
	}
	else if (0 == strcmp("GetRadioWlanIf", actionName)) {
		Monitor_get_radio_wlanif(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetRadioWlanIf",
					    MONITORSERVTYPE,
					    "RadioWlanIf", value);
	}else if (0 == strcmp("GetWlanName", actionName)) {
		Monitor_get_wlan_name(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetWlanName",
					    MONITORSERVTYPE,
					    "WlanName", value);
	}else if (0 == strcmp("GetWlanEssid", actionName)) {
		Monitor_get_wlan_essid(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetWlanEssid",
					    MONITORSERVTYPE,
					    "WlanEssid", value);
	}else if (0 == strcmp("GetWlanSeType", actionName)) {
		Monitor_get_wlan_securitytype(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetWlanSeType",
					    MONITORSERVTYPE,
					    "WlanSeType", value);
	}else if (0 == strcmp("GetWlanSeKey", actionName)) {
		Monitor_get_wlan_securitykey(ca_event->ActionRequest,value,str);
		UpnpAddToActionResponse(&ca_event->ActionResult, "GetWlanSeKey",
					    MONITORSERVTYPE,
					    "WlanSeKey", value);
	}
	else {
		fprintf(stderr, "unknown action name(%s)\n", actionName);
		ca_event->ErrCode = 402;
	}
	
	return 0;
}


int MonitorCallbackEvent(Upnp_EventType EventType, void *Event,
				 void *Cookie)
{
	
	switch (EventType) {
	/* ignore these cases, since this is not a control point */
	case UPNP_EVENT_SUBSCRIPTION_REQUEST:
	case UPNP_CONTROL_GET_VAR_REQUEST:
		break;
	case UPNP_CONTROL_ACTION_REQUEST:
		MonitorHandleActionRequest((struct Upnp_Action_Request *)
					    Event);
		break;
	case UPNP_DISCOVERY_ADVERTISEMENT_ALIVE:
	case UPNP_DISCOVERY_SEARCH_RESULT:
	case UPNP_DISCOVERY_SEARCH_TIMEOUT:
	case UPNP_DISCOVERY_ADVERTISEMENT_BYEBYE:
	case UPNP_CONTROL_ACTION_COMPLETE:
	case UPNP_CONTROL_GET_VAR_COMPLETE:
	case UPNP_EVENT_RECEIVED:
	case UPNP_EVENT_RENEWAL_COMPLETE:
	case UPNP_EVENT_SUBSCRIBE_COMPLETE:
	case UPNP_EVENT_UNSUBSCRIBE_COMPLETE:
	default:
		break;		
	}
	return 0;
	Cookie = Cookie;
}




int device_start(char *hostip, uint16_t port)
{
	int ret = UPNP_E_SUCCESS;
	char desc_doc_url[DESC_URL_SIZE] = {0};
	char *desc_doc_name = "";
	char *web_dir_path = "";

	
	ithread_mutex_init(&MoniterMutex, NULL);
	
	ret = UpnpInit(hostip, port);
	if (ret != UPNP_E_SUCCESS) {
		ApnpDebugLog("UpnpInit with HostIP=%s, DestPort=%d.\n", 
			hostip ? hostip : "", (int)port);
		UpnpFinish();
		return ret;
	}
	hostip = UpnpGetServerIpAddress();
	port = UpnpGetServerPort();

	ApnpDebugLog("UpnpInit with HostIP=%s, DestPort=%d.\n", 
			hostip ? hostip : "", (int)port);
	
	desc_doc_name = "LTE-FI_devdesc.xml";
	web_dir_path = DEFAULT_WEB_DIR;
	snprintf(desc_doc_url, DESC_URL_SIZE, "http://%s:%d/%s", hostip,
		 port, desc_doc_name);
	ret = UpnpSetWebServerRootDir(web_dir_path);
	if (ret != UPNP_E_SUCCESS) {
		UpnpFinish();
		return ret;
	}
	ret = UpnpRegisterRootDevice(desc_doc_url, MonitorCallbackEvent,
				     &device_handle, &device_handle);
	if (ret != UPNP_E_SUCCESS) {
		UpnpFinish();
		return ret;
	}
	ret = UpnpSendAdvertisement(device_handle, 1800);
	if (ret != UPNP_E_SUCCESS) {
		UpnpFinish();
		return ret;
	}
	return UPNP_E_SUCCESS;
}

int device_stop()
{
		ApnpDebugLog("device_stop\n");
		if(device_handle > 0){
			UpnpUnRegisterRootDevice(device_handle);
		}
		UpnpFinish();
		ithread_mutex_destroy(&MoniterMutex);
	return UPNP_E_SUCCESS;
}
