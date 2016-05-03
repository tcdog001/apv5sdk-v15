#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <stdlib.h>
#include <assert.h>
//#include <libxml/xmlreader.h>
#include <libxml/xmlsave.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <libxml/xpathInternals.h>


#include "ixml.h"
#include "apnp_config.h"
#include "apnp_type.h"
#include "apnp_list.h"
#include "apnp_log.h"

#define DL_APNP_FREE(obj_name)		{if(obj_name){dlsym_function("FreeBuffer",(obj_name),NULL); (obj_name) = NULL;}}
#define FREE_ACTION(p) {if((p)) {free((p));(p)=NULL;}}
#define ARG_NUM	6

extern void *dl_handle;


//apnp_ap ap_conf;
apnp_ap device_xml_conf;
char *pbuf;

#if 0
static void conf_ap_init()
{
	ap_conf = (apnp_ap)malloc(sizeof(*ap_conf));
	memset(ap_conf,0,sizeof(*ap_conf));
	char str[256] = {0};

	if(ap_conf == NULL)
	{
		printf("AP initialization failed !\n");
		return;
	}

	get_ap_apname(str);	
	ap_conf->apname = (char*)malloc(strlen(str)+1);
	memset(ap_conf->apname,0,strlen(str)+1);
	if(ap_conf->apname)
	{
		apnp_strncpy(ap_conf->apname,str,strlen(str)+1);
		memset(str,0,256);
	}
	else
	{
		printf("AP initialization failed !\n");
		APNP_FREE(ap_conf);
		return;
	}

	get_ap_ip(str);
	ap_conf->ip= (char*)malloc(strlen(str)+1);
	memset(ap_conf->ip,0,strlen(str)+1);
	if(ap_conf->ip)
	{
		apnp_strncpy(ap_conf->ip,str,strlen(str)+1);
		memset(str,0,256);
	}
	else
	{
		printf("AP initialization failed !\n");
		APNP_FREE(ap_conf->apname);
		APNP_FREE(ap_conf);
		return;
	}

	get_ap_mac(str);
	ap_conf->mac= (char*)malloc(strlen(str)+1);
	memset(ap_conf->mac,0,strlen(str)+1);
	if(ap_conf->mac)
	{
		apnp_strncpy(ap_conf->mac,str,strlen(str)+1);
		memset(str,0,256);
	}
	else
	{
		printf("AP initialization failed !\n");
		APNP_FREE(ap_conf->apname);
		APNP_FREE(ap_conf->ip);
		APNP_FREE(ap_conf);
		return;
	}
	
	get_ap_name(str);
	ap_conf->name = (char*)malloc(strlen(str)+1);
	memset(ap_conf->name,0,strlen(str)+1);
	if(ap_conf->name)
	{
		apnp_strncpy(ap_conf->name,str,strlen(str)+1);
		memset(str,0,256);
	}
	else
	{
		printf("AP initialization failed !\n");
		APNP_FREE(ap_conf->apname);
		APNP_FREE(ap_conf->ip);
		APNP_FREE(ap_conf->mac);		
		APNP_FREE(ap_conf);
		return;
	}

	get_ap_radioif(str);
	ap_conf->radioif= (char*)malloc(strlen(str)+1);
	memset(ap_conf->radioif,0,strlen(str)+1);
	if(ap_conf->radioif)
	{
		apnp_strncpy(ap_conf->radioif,str,strlen(str)+1);
		memset(str,0,256);
	}
	else
	{
		printf("AP initialization failed !\n");
		APNP_FREE(ap_conf->apname);
		APNP_FREE(ap_conf->ip);
		APNP_FREE(ap_conf->mac);
		APNP_FREE(ap_conf->name);
		APNP_FREE(ap_conf);
		return;
	}

	get_ap_radio_count(str);
	ap_conf->radio_count= (char*)malloc(strlen(str)+1);
	memset(ap_conf->radio_count,0,strlen(str)+1);
	if(ap_conf->radio_count)
	{
		apnp_strncpy(ap_conf->radio_count,str,strlen(str)+1);
		memset(str,0,256);
	}
	else
	{
		printf("AP initialization failed !\n");
		APNP_FREE(ap_conf->apname);
		APNP_FREE(ap_conf->ip);
		APNP_FREE(ap_conf->mac);
		APNP_FREE(ap_conf->name);
		APNP_FREE(ap_conf->radioif);
		APNP_FREE(ap_conf);
		return;
	}

	get_ap_soft_version(str);
	ap_conf->soft_version= (char*)malloc(strlen(str)+1);
	memset(ap_conf->soft_version,0,strlen(str)+1);
	if(ap_conf->soft_version)
	{
		apnp_strncpy(ap_conf->soft_version,str,strlen(str)+1);
		memset(str,0,256);
	}
	else
	{
		printf("AP initialization failed !\n");
		APNP_FREE(ap_conf->apname);
		APNP_FREE(ap_conf->ip);
		APNP_FREE(ap_conf->mac);
		APNP_FREE(ap_conf->name);
		APNP_FREE(ap_conf->radioif);
		APNP_FREE(ap_conf->radio_count);
		APNP_FREE(ap_conf);
		return;
	}

	get_ap_stacount(str);
	ap_conf->stacount= (char*)malloc(strlen(str)+1);
	memset(ap_conf->stacount,0,strlen(str)+1);
	if(ap_conf->stacount)
	{
		apnp_strncpy(ap_conf->stacount,str,strlen(str)+1);
		memset(str,0,256);
	}
	else
	{
		printf("AP initialization failed !\n");
		APNP_FREE(ap_conf->apname);
		APNP_FREE(ap_conf->ip);
		APNP_FREE(ap_conf->mac);
		APNP_FREE(ap_conf->name);
		APNP_FREE(ap_conf->radioif);
		APNP_FREE(ap_conf->radio_count);
		APNP_FREE(ap_conf->soft_version);
		APNP_FREE(ap_conf);
		return;
	}

	
	INIT_LIST_HEAD(&(ap_conf->head));
	

}

static int radioNum = 2 ;
static int wlanNum = 8 ;

static void conf_radio_init()
{
	apnp_radio radio_conf[radioNum];
	char radio[256] = {0};
	int i = 0;
	for(i = 0;i!=radioNum;++i)
	{
		sprintf(radio,"%s%d","Radio",i+1);
		radio_conf[i] = (apnp_radio)malloc(sizeof(struct apnp_radio_s));
	
		memset(radio_conf[i],0,sizeof(struct apnp_radio_s));
		char str[256] = {0};
	
		if(radio_conf[i] == NULL)
		{
			printf("Radio%d initialization failed !\n",i+1);
			return;
		}
	
		get_radio_channel(radio,str);
		radio_conf[i]->channel = (char*)malloc(strlen(str)+1);
		memset(radio_conf[i]->channel,0,strlen(str)+1);
		if(radio_conf[i]->channel)
		{
			apnp_strncpy(radio_conf[i]->channel,str,strlen(str)+1);
			memset(str,0,256);
		}
		else
		{
			printf("Radio%d initialization failed !\n",i+1);
			APNP_FREE(radio_conf[i]);
			return;
		}
	
		get_radio_id(radio,str);
		radio_conf[i]->id = (char*)malloc(strlen(str)+1);
		memset(radio_conf[i]->id,0,strlen(str)+1);
		if(radio_conf[i]->id)
		{
			apnp_strncpy(radio_conf[i]->id,str,strlen(str)+1);
			memset(str,0,256);
		}
		else
		{
			printf("Radio%d initialization failed !\n",i+1);
			APNP_FREE(radio_conf[i]->channel);
			APNP_FREE(radio_conf[i]);
			return;
		}
	
		get_radio_maxassoc(radio,str);
		radio_conf[i]->maxassoc = (char*)malloc(strlen(str)+1);
		memset(radio_conf[i]->maxassoc,0,strlen(str)+1);
		if(radio_conf[i]->maxassoc)
		{
			apnp_strncpy(radio_conf[i]->maxassoc,str,strlen(str)+1);
			memset(str,0,256);
		}
		else
		{
			printf("Radio%d initialization failed !\n",i+1);
			APNP_FREE(radio_conf[i]->channel);
			APNP_FREE(radio_conf[i]->id);
			APNP_FREE(radio_conf[i]);
			return;
		}
		
		get_radio_mode(radio,str);
		radio_conf[i]->mode = (char*)malloc(strlen(str)+1);
		memset(ap_conf->name,0,strlen(str)+1);
		if(radio_conf[i]->mode)
		{
			apnp_strncpy(radio_conf[i]->mode,str,strlen(str)+1);
			memset(str,0,256);
		}
		else
		{
			printf("Radio%d initialization failed !\n",i+1);
			APNP_FREE(radio_conf[i]->channel);
			APNP_FREE(radio_conf[i]->id);
			APNP_FREE(radio_conf[i]->maxassoc);		
			APNP_FREE(radio_conf[i]);
			return;
		}
	
		get_radio_power(radio,str);
		radio_conf[i]->power = (char*)malloc(strlen(str)+1);
		memset(radio_conf[i]->power,0,strlen(str)+1);
		if(radio_conf[i]->power)
		{
			apnp_strncpy(radio_conf[i]->power,str,strlen(str)+1);
			memset(str,0,256);
		}
		else
		{
			printf("Radio%d initialization failed !\n",i+1);
			APNP_FREE(radio_conf[i]->channel);
			APNP_FREE(radio_conf[i]->id);
			APNP_FREE(radio_conf[i]->maxassoc);
			APNP_FREE(radio_conf[i]->mode);
			APNP_FREE(radio_conf[i]);
			return;
		}
	
		get_radio_rate(radio,str);
		radio_conf[i]->rate = (char*)malloc(strlen(str)+1);
		memset(radio_conf[i]->rate,0,strlen(str)+1);
		if(radio_conf[i]->rate)
		{
			apnp_strncpy(radio_conf[i]->rate,str,strlen(str)+1);
			memset(str,0,256);
		}
		else
		{
			printf("Radio%d initialization failed !\n",i+1);
			APNP_FREE(radio_conf[i]->channel);
			APNP_FREE(radio_conf[i]->id);
			APNP_FREE(radio_conf[i]->maxassoc);
			APNP_FREE(radio_conf[i]->mode);
			APNP_FREE(radio_conf[i]->power);
			APNP_FREE(radio_conf[i]);
			return;
		}
	
		get_radio_wlancount(radio,str);
		radio_conf[i]->wlancount = (char*)malloc(strlen(str)+1);
		memset(radio_conf[i]->wlancount,0,strlen(str)+1);
		if(radio_conf[i]->wlancount)
		{
			apnp_strncpy(radio_conf[i]->wlancount,str,strlen(str)+1);
			memset(str,0,256);
		}
		else
		{
			printf("Radio%d initialization failed !\n",i+1);
			APNP_FREE(radio_conf[i]->channel);
			APNP_FREE(radio_conf[i]->id);
			APNP_FREE(radio_conf[i]->maxassoc);
			APNP_FREE(radio_conf[i]->mode);
			APNP_FREE(radio_conf[i]->power);
			APNP_FREE(radio_conf[i]->rate);
			APNP_FREE(radio_conf[i]);
			return;
		}
	
		get_radio_wlanif(radio,str);
		radio_conf[i]->wlanif = (char*)malloc(strlen(str)+1);
		memset(radio_conf[i]->wlanif,0,strlen(str)+1);
		if(radio_conf[i]->wlanif)
		{
			apnp_strncpy(radio_conf[i]->wlanif,str,strlen(str)+1);
			memset(str,0,256);
		}
		else
		{
			printf("Radio%d initialization failed !\n",i+1);
			APNP_FREE(radio_conf[i]->channel);
			APNP_FREE(radio_conf[i]->id);
			APNP_FREE(radio_conf[i]->maxassoc);
			APNP_FREE(radio_conf[i]->mode);
			APNP_FREE(radio_conf[i]->power);
			APNP_FREE(radio_conf[i]->rate);
			APNP_FREE(radio_conf[i]->wlancount);
			APNP_FREE(radio_conf[i]);
			return;
		}
		list_add(&(radio_conf[i]->radio_list),&(ap_conf->head));
		INIT_LIST_HEAD(&(radio_conf[i]->wlan_list));
	}
}
	

static void conf_wlan_init()
{
	apnp_wlan wlan_conf[wlanNum];
	char wlan_str[256] = {0};
	struct list_head* p;
	apnp_radio entry;
	int i = 0;
	int j = 0;
	p =	&(ap_conf->head);
	for(j=radioNum;j!=0;--j)
	{	
		
		p = p->next;
		entry = list_entry(p,struct apnp_radio_s,radio_list);
		for(i = 0;i!=wlanNum;++i)
		{	

			char str[256] = {0};
			memset(wlan_str,0,256);
			sprintf(wlan_str,"%s%d-%d","Wlan",j-1,i+1);
			wlan_conf[i] = (apnp_wlan)malloc(sizeof(struct apnp_wlan_s));
	
			memset(wlan_conf[i],0,sizeof(struct apnp_wlan_s));
			
	
			
			if(wlan_conf[i] == NULL)
			{
				printf("Wlan%d-%d initialization failed !\n",j-1,i+1);
				return;
			}
			
			get_wlan_name(wlan_str,str);
			wlan_conf[i]->name = (char*)malloc(strlen(str)+1);
			memset(wlan_conf[i]->name,0,strlen(str)+1);
			if(wlan_conf[i]->name)
			{
				apnp_strncpy(wlan_conf[i]->name,str,strlen(str)+1);
				memset(str,0,256);
			}
			else
			{
				printf("Wlan%d-%d initialization failed !\n",j-1,i+1);
				APNP_FREE(wlan_conf[i]);
				return;
			}
			
			get_wlan_essid(wlan_str,str);
			wlan_conf[i]->essid= (char*)malloc(strlen(str)+1);
			memset(wlan_conf[i]->essid,0,strlen(str)+1);
			if(wlan_conf[i]->essid)
			{
				apnp_strncpy(wlan_conf[i]->essid,str,strlen(str)+1);
				memset(str,0,256);
			}
			else
			{
				printf("Wlan%d-%d initialization failed !\n",j-1,i+1);
				APNP_FREE(wlan_conf[i]->name);
				APNP_FREE(wlan_conf[i]);
				return;
			}
	
			get_wlan_securitytype(wlan_str,str);
			wlan_conf[i]->securitytype= (char*)malloc(strlen(str)+1);
			memset(wlan_conf[i]->securitytype,0,strlen(str)+1);
			if(wlan_conf[i]->securitytype)
			{
				apnp_strncpy(wlan_conf[i]->securitytype,str,strlen(str)+1);
				memset(str,0,256);
			}
			else
			{
				printf("Wlan%d-%d initialization failed !\n",j-1,i+1);
				APNP_FREE(wlan_conf[i]->name);
				APNP_FREE(wlan_conf[i]->essid);
				APNP_FREE(wlan_conf[i]);
				return;
			}

			get_wlan_securitykey(wlan_str,str);
			wlan_conf[i]->securitykey= (char*)malloc(strlen(str)+1);
			memset(wlan_conf[i]->securitykey,0,strlen(str)+1);
			if(wlan_conf[i]->securitykey)
			{
				apnp_strncpy(wlan_conf[i]->securitykey,str,strlen(str)+1);
				memset(str,0,256);
			}
			else
			{
				printf("Wlan%d-%d initialization failed !\n",j-1,i+1);
				APNP_FREE(wlan_conf[i]->name);
				APNP_FREE(wlan_conf[i]->essid);
				APNP_FREE(wlan_conf[i]->securitytype);
				APNP_FREE(wlan_conf[i]);
				return;
			}

			list_add(&(wlan_conf[i]->wlan_list),&(entry->wlan_list));
		}
	}
		
}

static void conf_wlan_free()
{
	struct list_head* p;
	struct list_head* q;
	apnp_radio entry;
	apnp_wlan entry_wlan;
	list_for_each(p,&(ap_conf->head))
	{
		entry = list_entry(p,struct apnp_radio_s,radio_list);
		list_for_each(q,&(entry->wlan_list))
		{
				entry_wlan = list_entry(q,struct apnp_wlan_s,wlan_list);
				printf("%s\n",entry_wlan->essid);
				printf("%s\n",entry_wlan->name);
				printf("%s\n",entry_wlan->securitykey);
				printf("%s\n",entry_wlan->securitytype);	
		}
	
		APNP_FREE(entry_wlan->essid);
		APNP_FREE(entry_wlan->name);
		APNP_FREE(entry_wlan->securitykey);
		APNP_FREE(entry_wlan->securitytype);
		APNP_FREE(entry_wlan);
	}
}


static void conf_radio_free()
{
	struct list_head* p;
	apnp_radio entry;
	list_for_each(p,&(ap_conf->head))
	{
		entry = list_entry(p,struct apnp_radio_s,radio_list);
		printf("%s\n",entry->channel);
		printf("%s\n",entry->id);
		printf("%s\n",entry->maxassoc);
		printf("%s\n",entry->mode);
		printf("%s\n",entry->power);
		printf("%s\n",entry->rate);
		printf("%s\n",entry->wlancount);
		printf("%s\n",entry->wlanif);
	
		
		APNP_FREE(entry->channel);
		APNP_FREE(entry->id);
		APNP_FREE(entry->maxassoc);
		APNP_FREE(entry->mode);
		APNP_FREE(entry->power);
		APNP_FREE(entry->rate);
		APNP_FREE(entry->wlancount);
		APNP_FREE(entry->wlanif);
		APNP_FREE(entry);
	}
}

#endif
void conf_ap_free()
{
	if(device_xml_conf)
	{
		DL_APNP_FREE(device_xml_conf->ip);
		DL_APNP_FREE(device_xml_conf->mac);
		DL_APNP_FREE(device_xml_conf->sn);
		DL_APNP_FREE(device_xml_conf->model);
		DL_APNP_FREE(device_xml_conf);

	}
/*
	if(ap_conf)
	{	
		conf_wlan_free();
		conf_radio_free();
		APNP_FREE(ap_conf->apname);
		//APNP_FREE(ap_conf->ip);
		//APNP_FREE(ap_conf->mac);
		APNP_FREE(ap_conf->name);
		//APNP_FREE(ap_conf->model);
		//APNP_FREE(ap_conf->sn);
		APNP_FREE(ap_conf->radioif);
		APNP_FREE(ap_conf->radio_count);
		APNP_FREE(ap_conf->soft_version);
		APNP_FREE(ap_conf->stacount);
		APNP_FREE(ap_conf);
	}
	*/
}



static int conf_xml_do(const char* filePath)
{
	ApnpDebugLog("enter conf_xml_do\n");
	IXML_Document* doc = NULL;
	DOMString s;
	FILE* file = fopen(filePath,"w");
	if(file)
	{						
		doc = ixmlDocument_createDocument();
		if(doc)
		{
			IXML_Element * root = ixmlDocument_createElement(doc,"root");							
			ixmlElement_setAttributeNS(root,"http://www.w3.org/2000/xmlns/","xmlns","urn:schemas-upnp-org:device-1-0");
			ixmlNode_appendChild(doc,root);
		
			IXML_Element * specVersion = ixmlDocument_createElement(doc,"specVersion");
			ixmlNode_appendChild(root,specVersion);
			
			IXML_Element * major = ixmlDocument_createElement(doc,"major");
			IXML_Node * txtMajor = ixmlDocument_createTextNode(doc,"1");
			ixmlNode_appendChild(major,txtMajor);
			ixmlNode_appendChild(specVersion,major);

			IXML_Element * minor = ixmlDocument_createElement(doc,"minor");
			IXML_Node * txtMinor = ixmlDocument_createTextNode(doc,"0");
			ixmlNode_appendChild(minor,txtMinor);
			ixmlNode_appendChild(specVersion,minor);
	
			IXML_Element * device = ixmlDocument_createElement(doc,"device");
			ixmlNode_appendChild(root,device);
			
			IXML_Element * deviceType = ixmlDocument_createElement(doc,"deviceType");
			IXML_Node * txtDeviceType = ixmlDocument_createTextNode(doc,"urn:schemas-upnp-org:device:tvdevice:1");
			ixmlNode_appendChild(deviceType,txtDeviceType);
			ixmlNode_appendChild(device,deviceType);
						
			IXML_Element * friendlyName = ixmlDocument_createElement(doc,"friendlyName");
			IXML_Node * txtFriendlyName = ixmlDocument_createTextNode(doc,"ST");
			ixmlNode_appendChild(friendlyName,txtFriendlyName);
			ixmlNode_appendChild(device,friendlyName);
				
			IXML_Element * manufacturer = ixmlDocument_createElement(doc,"manufacturer");
			IXML_Node * txtManufacturer = ixmlDocument_createTextNode(doc,"AuteLAN");
			ixmlNode_appendChild(manufacturer,txtManufacturer);
			ixmlNode_appendChild(device,manufacturer);
						
			IXML_Element * manufacturerURL = ixmlDocument_createElement(doc,"manufacturerURL");
			IXML_Node * txtManufacturerURL = ixmlDocument_createTextNode(doc,"http://www.manufacturer.com");
			ixmlNode_appendChild(manufacturerURL,txtManufacturerURL);
			ixmlNode_appendChild(device,manufacturerURL);

			IXML_Element * modelDescription = ixmlDocument_createElement(doc,"modelDescription");
			IXML_Node * txtmodelDescription = ixmlDocument_createTextNode(doc,device_xml_conf->mac);
			ixmlNode_appendChild(modelDescription,txtmodelDescription);
			ixmlNode_appendChild(device,modelDescription);
			
			IXML_Element * modelName = ixmlDocument_createElement(doc,"modelName");
			IXML_Node * txtmodelName = ixmlDocument_createTextNode(doc,device_xml_conf->model);
			ixmlNode_appendChild(modelName,txtmodelName);
			ixmlNode_appendChild(device,modelName);
		
			IXML_Element * modelNumber = ixmlDocument_createElement(doc,"modelNumber");
			IXML_Node * txtmodelNumber= ixmlDocument_createTextNode(doc,"1.0");
			ixmlNode_appendChild(modelNumber,txtmodelNumber);
			ixmlNode_appendChild(device,modelNumber);
		
			IXML_Element * modelURL = ixmlDocument_createElement(doc,"modelURL");
			IXML_Node * txtmodelURL = ixmlDocument_createTextNode(doc,"http://www.autelan.com/monitor/");
			ixmlNode_appendChild(modelURL,txtmodelURL);
			ixmlNode_appendChild(device,modelURL);
			
			IXML_Element * serialNumber = ixmlDocument_createElement(doc,"serialNumber");
			IXML_Node * txtserialNUmber = ixmlDocument_createTextNode(doc,device_xml_conf->sn);
			ixmlNode_appendChild(serialNumber,txtserialNUmber);
			ixmlNode_appendChild(device,serialNumber);
			
			IXML_Element * UDN = ixmlDocument_createElement(doc,"UDN");
			IXML_Node * txtUDN = ixmlDocument_createTextNode(doc,"uuid:Upnp-TVEmulator-1_0-1234567890001");
			ixmlNode_appendChild(UDN,txtUDN);
			ixmlNode_appendChild(device,UDN);			

			IXML_Element * UPC= ixmlDocument_createElement(doc,"UPC");
			IXML_Node * txtUPC = ixmlDocument_createTextNode(doc,"123456789");
			ixmlNode_appendChild(UPC,txtUPC);
			ixmlNode_appendChild(device,UPC);
			
			IXML_Element * serviceList = ixmlDocument_createElement(doc,"serviceList");
			ixmlNode_appendChild(device,serviceList);
						
			IXML_Element * service = ixmlDocument_createElement(doc,"service");
			ixmlNode_appendChild(serviceList,service);
			
			IXML_Element * serviceType = ixmlDocument_createElement(doc,"serviceType");
			IXML_Node * txtserviceType = ixmlDocument_createTextNode(doc,"urn:schemas-upnp-org:service:tvcontrol:1");
			ixmlNode_appendChild(serviceType,txtserviceType);
			ixmlNode_appendChild(service,serviceType);
			
			IXML_Element * serviceId = ixmlDocument_createElement(doc,"serviceId");
			IXML_Node * txtserviceId = ixmlDocument_createTextNode(doc,"urn:upnp-org:serviceId:tvcontrol1");
			ixmlNode_appendChild(serviceId,txtserviceId);
			ixmlNode_appendChild(service,serviceId);
			
			IXML_Element * controlURL = ixmlDocument_createElement(doc,"controlURL");
			IXML_Node * txtcontrolURL = ixmlDocument_createTextNode(doc,"/upnp/control/tvcontrol1");
			ixmlNode_appendChild(controlURL,txtcontrolURL);
			ixmlNode_appendChild(service,controlURL);

			IXML_Element * eventSubURL = ixmlDocument_createElement(doc,"eventSubURL");
			IXML_Node * txteventSubURL = ixmlDocument_createTextNode(doc,"/upnp/event/tvcontrol1");
			ixmlNode_appendChild(eventSubURL,txteventSubURL);
			ixmlNode_appendChild(service,eventSubURL);
			
			IXML_Element * SCPDURL = ixmlDocument_createElement(doc,"SCPDURL");
			IXML_Node * txtSCPDURL = ixmlDocument_createTextNode(doc,"/tvcontrolSCPD_test.xml");
			ixmlNode_appendChild(SCPDURL,txtSCPDURL);
			ixmlNode_appendChild(service,SCPDURL);
			
			IXML_Element * presentationURL = ixmlDocument_createElement(doc,"presentationURL");
			IXML_Node * txtpresentationURL= ixmlDocument_createTextNode(doc,device_xml_conf->ip);
			ixmlNode_appendChild(presentationURL,txtpresentationURL);
			ixmlNode_appendChild(device,presentationURL);
										
			s = ixmlDocumenttoString(doc);
			if(fwrite(s,strlen(s),1,file) != 1)					
			{
				printf("XML write error !\n");
				ixmlFreeDOMString (s);
				ixmlDocument_free (doc);
				fclose(file);
				return 0;
			}
			else
			{	
				fclose(file);
				ixmlFreeDOMString (s);
				ixmlDocument_free (doc);
				return 1;
			}
		}
		else
			return 0;
	}	
	else
		return 0;
}

static void conf_xml_init()
{	
	ApnpDebugLog("enter conf_xml_init\n");
	const char* filePath = "/var/LTE-FI_devdesc.xml";	
	int errNo = -1;
	if ( /*access (filePath, F_OK) == -1 */1)
	{
		errNo = conf_xml_do(filePath);
		if(errNo == 0)
			ApnpDebugLog("Failed to configure XML	!\n");
		else
			ApnpDebugLog("Configure the XML success !\n");
	}
	else
	{
		//conf_xml_do(filePath) ;
			ApnpDebugLog("XML already exists !\n");			
			int rc;
			IXML_Document* doc = NULL;
			DOMString s;
			char* p;
			rc = ixmlLoadDocumentEx (filePath, &doc);
			//rc=UpnpDownloadXmlDoc(filePath,&doc);
			if(rc == IXML_SUCCESS)
			{
				IXML_Document *doc_desc;
				
				if(doc==NULL)
					ApnpDebugLog("doc false\n");
				IXML_NodeList *node_list = ixmlDocument_getElementsByTagName(doc,"serviceId");
				if(node_list==NULL)
					ApnpDebugLog("fail\n");
				IXML_Node *node= ixmlNodeList_item(node_list,0);
				node= ixmlNode_getFirstChild(node);
				if(node==NULL)
					ApnpDebugLog(" node null");
				else
					ApnpDebugLog("serverID:%s",ixmlNode_getNodeValue(node));
				ixmlNodeList_free (node_list);
				ixmlDocument_free(doc_desc);
			}

	}
				

}
static void *add_action_xml(xmlNodePtr actionList,char *function_name,char *argument)
{
	ApnpDebugLog("enter add_action_xml\n");
	if( !function_name || !argument){
		ApnpDebugLog("add_action_xml: function or argument NULL\n");
		return NULL;
	}
	xmlNodePtr action = xmlNewNode(NULL,"action");
	xmlAddChild(actionList,action);
	xmlNewTextChild(action, NULL, BAD_CAST"function", BAD_CAST function_name);
	xmlNodePtr argumentlist = xmlNewNode(NULL,"argumentList");
	xmlAddChild(action,argumentlist);
	
	xmlNewTextChild(argumentlist,NULL,BAD_CAST"value",BAD_CAST argument);

}

static xmlNodePtr create_data_xml(xmlDocPtr *doc,char *uuid)
{
	*doc = xmlNewDoc(BAD_CAST"1.0");
	if(!(*doc)){
		ApnpDebugLog("create data xml error\n");
		return NULL;
	}
	xmlNodePtr root = xmlNewNode(NULL,BAD_CAST"response");
	xmlDocSetRootElement(*doc,root);

	xmlNewTextChild(root, NULL, BAD_CAST"uuid", BAD_CAST uuid);
	
	xmlNodePtr actionlist = xmlNewNode(NULL,BAD_CAST"actionList");
	xmlAddChild(root,actionlist);
	return actionlist;
}
static int read_service_xml(char **buff)
{
	xmlDocPtr sdoc;
	int size;
	char *service_xml_path="/var/actionList.xml";
	sdoc = xmlParseFile(service_xml_path);
//	sdoc = xmlReadFile(xml_path,"utf-8",256);
	if(sdoc == NULL){
		ApnpDebugLog("\nread_service_xml:  xmlReadFile error!\n");
		return 0;
	}
	xmlDocDumpMemory(sdoc, buff, &size);
	ApnpDebugLog("read_service_xml size:%d-buff:%s\n",size,*buff);
	return 1;

}
char* read_mem_xml(char *udpbuff,int udpsize)
{

	ApnpDebugLog("size:%d -- udpbuff:\n%s\n",udpsize,udpbuff);

	
	xmlChar *buff=NULL;
	char *dl_data=NULL;
	int size=0;
	
	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlNodePtr tmp;
	
	char *uuid_tmp=NULL;
	char *uuid=NULL;
	xmlChar *fun=NULL;
	char *ch_fun=NULL;
	xmlChar *arg;
	char *ch_arg[ARG_NUM];
	int i=0;
	/*sdoc = xmlReadFile(xml_path,"utf-8",256);
	if(sdoc == NULL){
		printf("\nread_mem_xml:  xmlReadFile error!\n");
		return 0;
	}
	xmlDocDumpMemory(sdoc, &buff, &size);
	printf("sdoc buff:%s\n",buff);
	*/
	xmlKeepBlanksDefault(0);
	doc=xmlParseMemory(udpbuff,udpsize);
	if(doc == NULL){
		ApnpDebugLog("\nread_mem_xml:  xmlParseMemory error!\n");
		return 0;
	}
	//xmlKeepBlanksDefault(0);
	cur = xmlDocGetRootElement(doc);
	if(cur == NULL){
		ApnpDebugLog("read_mem_xml: get root element error!\n");
		xmlFreeDoc(doc);
		return 0;
	}

	if(xmlStrcmp(cur->name,(const xmlChar *)"service")){
		ApnpDebugLog("xml file(root) not matching\n");
		xmlFreeDoc(doc);
		return 0;
	}

	cur = cur->children;
	if(cur == NULL){
		ApnpDebugLog("read_mem_xml: get root element error!\n");
		return 0;
	}
	/*add uuid*/
	if(!xmlStrcmp(cur->name,(const xmlChar *)"uuid"))
	{
		uuid_tmp= xmlNodeGetContent(cur->children);
		if(!uuid_tmp){
			ApnpDebugLog("uuid is NULL\n");
			return NULL;
		}
		ApnpDebugLog("uuid:%s\n",uuid_tmp);
		uuid = (char *)malloc(strlen(uuid_tmp)+1);
		memset(uuid,0,strlen(uuid_tmp)+1);
		memcpy(uuid,uuid_tmp,strlen(uuid_tmp)+1);
	}
	else{
		ApnpDebugLog("no uuid element\n");
		return NULL;
	}
	
	xmlDocPtr new_doc=NULL;
	xmlNodePtr actlist =NULL;
	actlist=create_data_xml(&new_doc,uuid);

	if(!actlist){
		ApnpDebugLog("read_mem_xml: ceate data xml error\n");
		return 0;
	}
	

	cur = cur->next;
	if(!xmlStrcmp(cur->name,(const xmlChar *)"actionList"))
	{
		cur = cur->children;
		tmp = cur;
		for(;cur;tmp = tmp->next,cur = tmp)
		{
			if(!xmlStrcmp(cur->name,(const xmlChar *)"action"))
			{	
				cur = cur->children;
				if(cur && !xmlStrcmp(cur->name,(const xmlChar *)"function"))
				{
						
					fun= xmlNodeGetContent(cur->children);
					if(!fun){
						ApnpDebugLog("function is NULL\n");
						continue;
					}
					ch_fun= (char *)malloc(strlen(fun)+1);// need +1
					memset(ch_fun,0,strlen(fun)+1);
					memcpy(ch_fun,fun,strlen(fun)+1);
					xmlFree(fun);
				}else{
					ApnpDebugLog("read_mem_xml: xml format(function) error\n");
					continue;
				}
				cur = cur->next;
				if(cur && !xmlStrcmp(cur->name,(const xmlChar *)"argumentList"))
				{
					int j=0;
					cur = cur->children;
					for(;cur && j<ARG_NUM;j++)
					{
						arg = xmlNodeGetContent(cur->children);
						if(!arg){
							ApnpDebugLog("arg is NULL\n");
							break;
						}
						ch_arg[j] = (char *)malloc(strlen(arg)+1);
						memset(ch_arg[j],0,strlen(arg)+1);
						memcpy(ch_arg[j],arg,strlen(arg)+1);
						xmlFree(arg);

						cur = cur->next;
						/*fun = xmlNodeListGetString(doc,cur->children,2);*/
					}
					if(j<=ARG_NUM)
					{
						for(;j<ARG_NUM;j++)
						{
							ch_arg[j] = NULL;
						}
					}else{
						ApnpDebugLog("too many arguments \n");
						return -1;
					}
				}
				else{
					int m=0;
					for(;m<ARG_NUM;m++)
					{
						ch_arg[m] = NULL;
					}
				}
				
				int x=0;
				for(x=0;x<ARG_NUM;x++){
					if(ch_arg[x])	
						ApnpDebugLog("ch_arg[%d]:%s\n",x,ch_arg[x]);
					else 
						ApnpDebugLog("ch_arg[%d] addr: %p\n",x,ch_arg[x]);
				}
				
				if(!strcmp(ch_fun,"GetActionXml")){
					if(!read_service_xml(&buff))
						buff = NULL;
					return buff;
				}

				dl_data = dlsym_function(ch_fun,ch_arg[0],ch_arg[1],ch_arg[2],ch_arg[3],ch_arg[4],ch_arg[5]);

				if(dl_data)ApnpDebugLog("re_value :%s\n",dl_data);
				
				add_action_xml(actlist,ch_fun,dl_data);
	
				DL_APNP_FREE(dl_data);
				FREE_ACTION(ch_fun);

				int k=0;
				for(k=0;k<ARG_NUM;k++)
				{
					FREE_ACTION(ch_arg[k]);
				}
			}else{
				ApnpDebugLog("read_mem_xml: xml format(action) error\n");
				return 0;
			}
		}
	}else{
		ApnpDebugLog("read_mem_xml: xml format(actionList) error\n");
		return 0;
	}
	if(new_doc == NULL){
		ApnpDebugLog("read_mem_xml: create new xml error\n");
		return 0;
	}
	FREE_ACTION(uuid);

	xmlDocDumpMemory(new_doc, &(buff),&size);
	//xmlDocDumpFormatMemory(new_doc, &(buff),&size,1);
/*
	int ret_save;
	ret_save=xmlSaveFile("/tmp/new_re_fun.xml",new_doc);
	if(ret_save == -1)
	{
		printf("read_mem_xml: xmlSaveFile error\n");
		return -1;
	}*/
	xmlFreeDoc(doc);
	xmlFreeDoc(new_doc);
	
	if(buff == NULL){
		ApnpDebugLog("read_mem_xml: new_doc xmlDocDumpMemory\n");
		return NULL;
	}
	ApnpDebugLog("read_mem_xml buff:\n%s\n",buff);
	return buff;	
}
static void conf_construct_xml()
{
	//read_mem_xml();

	//data_fill_xml();



}
static char *get_ap_info(char *type)
{

	char *error;
	char * (*dl_function)(void);
	if(!type){
		ApnpDebugLog("info name error\n");
		return NULL;
	}
	dl_function = dlsym(dl_handle,type);
	if((error = dlerror()) != NULL){
		ApnpDebugLog("get_ap_info error:%s\n",error); 
		return NULL;
	}
	char *data=NULL;
	data = (*dl_function)();
	return data;

}
static void conf_device_init()
{
	ApnpDebugLog("enter conf_device_init\n");
	
	device_xml_conf = (apnp_ap)malloc(sizeof(*device_xml_conf)); 
	memset(device_xml_conf,0,sizeof(*device_xml_conf));

	device_xml_conf->ip = get_ap_info("GetApIP");
	
	device_xml_conf->mac = get_ap_info("GetApMAC");
	
	device_xml_conf->model = get_ap_info("GetApType");
	
	device_xml_conf->sn = get_ap_info("GetApSN");


}

 void config_init()
{
		//conf_ap_init();
		//conf_radio_init();
		//conf_wlan_init();

		conf_device_init();
		conf_xml_init();
		//conf_construct_xml();
}









