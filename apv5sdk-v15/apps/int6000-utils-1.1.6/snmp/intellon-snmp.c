/*====================================================================*
 *   
 *   Copyright (c) 2010, Atheros Communications Inc.
 *   
 *   Permission to use, copy, modify, and/or distribute this software 
 *   for any purpose with or without fee is hereby granted, provided 
 *   that the above copyright notice and this permission notice appear 
 *   in all copies.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL 
 *   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED 
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL  
 *   THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR 
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 *   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
 *   NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *   
 *--------------------------------------------------------------------*/

/*====================================================================*
 *   
 *   intellon-snmp.c
 *   
 *   Implement the IHPAV-OPERATIONS MIB.
 *
 *   This software and documentation is the property of Intellon 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Intellon assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Intellon customers may modify and distribute the software 
 *   without obligation to Intellon. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of it's use. 
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *      Help also from mib2c.
 *
 *--------------------------------------------------------------------*/

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../snmp/intellon-snmp.h"

/*====================================================================*
 *   local functions;
 *--------------------------------------------------------------------*/

struct operationsTable_entry *operationsTable_head;
extern struct int6k int6k;
static int dev_count;

/*====================================================================*
 *
 *   void init_intellon (void);
 *
 *--------------------------------------------------------------------*/

void init_intellon (void) 

{
	init_deviceCount ();
	init_table_operationsTable ();
}

/*====================================================================*
 *
 *   void init_deviceCount (void);
 *
 *--------------------------------------------------------------------*/

void init_deviceCount (void) 

{
	static oid dev_count_oid [] = 
	{
		1,
		3,
		6,
		1,
		4,
		1,
		32019,
		1,
		1,
		1,
		1,
		0 
	};
	netsnmp_register_int_instance ("deviceCount", dev_count_oid, OID_LENGTH (dev_count_oid), &dev_count, NULL);
}

/*====================================================================*
 *
 *   void init_table_operationsTable (void);
 *
 *--------------------------------------------------------------------*/

void init_table_operationsTable (void) 

{
	static oid operationsTable_oid [] = 
	{
		1,
		3,
		6,
		1,
		4,
		1,
		32019,
		1,
		1,
		1,
		2 
	};
	size_t operationsTable_oid_len = OID_LENGTH (operationsTable_oid);
	netsnmp_handler_registration *reg;
	netsnmp_iterator_info *iinfo;
	netsnmp_table_registration_info *table_info;
	reg = netsnmp_create_handler_registration ("operationsTable", operationsTable_handler, operationsTable_oid, operationsTable_oid_len, HANDLER_CAN_RWRITE);
	table_info = SNMP_MALLOC_TYPEDEF (netsnmp_table_registration_info);

/* index: deviceIndex */

	netsnmp_table_helper_add_indexes (table_info, ASN_INTEGER, 0);
	table_info->min_column = COLUMN_DEVICEMACADDRESS;
	table_info->max_column = COLUMN_DEVICERECEIVEDFECBADCOUNT;
	iinfo = SNMP_MALLOC_TYPEDEF (netsnmp_iterator_info);
	iinfo->get_first_data_point = operationsTable_get_first_data_point;
	iinfo->get_next_data_point = operationsTable_get_next_data_point;
	iinfo->table_reginfo = table_info;
	netsnmp_register_table_iterator (reg, iinfo);
}

/*====================================================================*
 *
 *   struct operationsTable_entry * operationsTable_createEntry (long deviceIndex, struct device *d);
 *
 *    create a new row in the (unsorted) table;
 *
 *--------------------------------------------------------------------*/

struct operationsTable_entry * operationsTable_createEntry (long deviceIndex, struct device *d) 

{
	struct operationsTable_entry *entry;
	entry = SNMP_MALLOC_TYPEDEF (struct operationsTable_entry);
	if (!entry) return NULL;
	memcpy (entry->deviceMacAddress, d->mac, strlen (d->mac) + 1);
	entry->deviceMacAddress_len = strlen (entry->deviceMacAddress);
	memcpy (entry->deviceID, d->id, strlen (d->id) + 1);
	entry->deviceID_len = strlen (entry->deviceID);
	memcpy (entry->deviceFirmwareRevision, d->version, strlen (d->version) + 1);
	entry->deviceFirmwareRevision_len = strlen (entry->deviceFirmwareRevision);
	entry->deviceIndex = deviceIndex;
	entry->next = operationsTable_head;
	operationsTable_head = entry;
	++dev_count;
	return entry;
}

/*====================================================================*
 *
 *   void operationsTable_remove (struct operationsTable_entry *entry);
 *
 *   remove a row from the table;
 *
 *--------------------------------------------------------------------*/

void operationsTable_remove (struct operationsTable_entry *entry) 

{
	struct operationsTable_entry *ptr,
	*prev;
	if (!entry) return;

/* Nothing to remove */

	for (ptr = operationsTable_head, prev = NULL; ptr != NULL; prev = ptr, ptr = ptr->next) 
	{
		if (ptr == entry) break;
	}
	if (!ptr) return;

/* Can't find it */

	if (prev == NULL) operationsTable_head = ptr->next;
	else prev->next = ptr->next;
	SNMP_FREE (entry);
	--dev_count;
}

netsnmp_variable_list * operationsTable_get_first_data_point (void **my_loop_context, void **my_data_context, netsnmp_variable_list *put_index_data, netsnmp_iterator_info *mydata) 

{
	*my_loop_context = operationsTable_head;
	return operationsTable_get_next_data_point (my_loop_context, my_data_context, put_index_data, mydata);
}

netsnmp_variable_list * operationsTable_get_next_data_point (void **my_loop_context, void **my_data_context, netsnmp_variable_list *put_index_data, netsnmp_iterator_info *mydata) 

{
	struct operationsTable_entry *entry = (struct operationsTable_entry *)*my_loop_context;
	netsnmp_variable_list *idx = put_index_data;
	if (entry) 
	{
		snmp_set_var_typed_integer (idx, ASN_INTEGER, entry->deviceIndex);
		idx = idx->next_variable;
		*my_data_context = (void *)entry;
		*my_loop_context = (void *)entry->next;
		return put_index_data;
	}
	else 
	{
		return NULL;
	}
}

/*====================================================================*
 *
 *   handles requests for the operationsTable table;
 *
 *
 *--------------------------------------------------------------------*/

int operationsTable_handler (netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests) 

{
	netsnmp_request_info *request;
	netsnmp_table_request_info *table_info;
	struct operationsTable_entry *table_entry;
	int ret;
	switch (reqinfo->mode) 
	{

/*
 * Read-support (also covers GetNext requests)
 */

	case MODE_GET:
		for (request=requests; request; request=request->next) 
		{
			table_entry = (struct operationsTable_entry *) netsnmp_extract_iterator_context (request);
			table_info = netsnmp_extract_table_info (request);
			switch (table_info->colnum) 
			{
			case COLUMN_DEVICEMACADDRESS:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_value (request->requestvb, ASN_OCTET_STR, (u_char*)table_entry->deviceMacAddress, table_entry->deviceMacAddress_len);
				break;
			case COLUMN_DEVICEID:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_value (request->requestvb, ASN_OCTET_STR, (u_char*)table_entry->deviceID, table_entry->deviceID_len);
				break;
			case COLUMN_DEVICEFIRMWAREREVISION:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_value (request->requestvb, ASN_OCTET_STR, (u_char*)table_entry->deviceFirmwareRevision, table_entry->deviceFirmwareRevision_len);
				break;
			case COLUMN_DEVICERESET:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->deviceReset);
				break;
			case COLUMN_DEVICEINITIALIZE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->deviceInitialize);
				break;
			case COLUMN_DEVICEPRIORITYVLANTAGSENABLE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityVLANTagsEnable);
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG0:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityVLANTag0);
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG1:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityVLANTag1);
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG2:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityVLANTag2);
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG3:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityVLANTag3);
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG4:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityVLANTag4);
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG5:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityVLANTag5);
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG6:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityVLANTag6);
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG7:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityVLANTag7);
				break;
			case COLUMN_DEVICEPRIORITYTRAFFICCLASSENABLE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTrafficClassEnable);
				break;
			case COLUMN_DEVICEPRIORITYTOSBITSENABLE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTOSBitsEnable);
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS0:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTOSBits0);
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS1:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTOSBits1);
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS2:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTOSBits2);
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS3:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTOSBits3);
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS4:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTOSBits4);
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS5:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTOSBits5);
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS6:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTOSBits6);
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS7:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTOSBits7);
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP0:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTTLCap0);
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP1:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTTLCap1);
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP2:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTTLCap2);
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP3:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityTTLCap3);
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTIGMP:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityDefaultIGMP);
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTUNICAST:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityDefaultUnicast);
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTIGMPMANAGEDMULTICAST:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityDefaultIGMPManagedMulticast);
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTMULTICASTBROADCAST:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityDefaultMulticastBroadcast);
				break;
			case COLUMN_DEVICEPRIORITYDSCPENABLE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->devicePriorityDSCPEnable);
				break;
			case COLUMN_DEVICENMK:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_value (request->requestvb, ASN_OCTET_STR, (u_char*)table_entry->deviceNMK, table_entry->deviceNMK_len);
				break;
			case COLUMN_DEVICERESETTOFACTORYDEFAULT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->deviceResettoFactoryDefault);
				break;
			case COLUMN_DEVICEHFID:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_value (request->requestvb, ASN_OCTET_STR, (u_char*)table_entry->deviceHFID, table_entry->deviceHFID_len);
				break;
			case COLUMN_DEVICEMDUENABLE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->deviceMDUEnable);
				break;
			case COLUMN_DEVICEMDUROLE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->deviceMDURole);
				break;
			case COLUMN_DEVICEAVNETWORKCCOSELECTION:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->deviceAVNetworkCCoSelection);
				break;
			case COLUMN_DEVICEFIRMWAREUPGRADE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_INTEGER, table_entry->deviceFirmwareUpgrade);
				break;
			case COLUMN_DEVICETRANSMITTEDPHYRATE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_GAUGE, table_entry->deviceTransmittedPhyRate);
				break;
			case COLUMN_DEVICERECEIVEDPHYRATE:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_GAUGE, table_entry->deviceReceivedPhyRate);
				break;
			case COLUMN_DEVICETRANSMITTEDTONEMAPS:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_GAUGE, table_entry->deviceTransmittedToneMaps);
				break;
			case COLUMN_DEVICERECEIVEDTONEMAPS:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_GAUGE, table_entry->deviceReceivedToneMaps);
				break;
			case COLUMN_DEVICETRANSMITTEDMPDUGOODCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceTransmittedMPDUGoodCount);
				break;
			case COLUMN_DEVICERECEIVEDMPDUGOODCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceReceivedMPDUGoodCount);
				break;
			case COLUMN_DEVICETRANSMITTEDMPDUBADCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceTransmittedMPDUBadCount);
				break;
			case COLUMN_DEVICERECEIVEDMPDUBADCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceReceivedMPDUBadCount);
				break;
			case COLUMN_DEVICETRANSMITTEDMPDUCOLLISIONCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceTransmittedMPDUCollisionCount);
				break;
			case COLUMN_DEVICETRANSMITTEDPDGOODCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceTransmittedPDGoodCount);
				break;
			case COLUMN_DEVICERECEIVEDPDGOODCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceReceivedPDGoodCount);
				break;
			case COLUMN_DEVICETRANSMITTEDPDBADCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceTransmittedPDBadCount);
				break;
			case COLUMN_DEVICERECEIVEDPDBADCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceReceivedPDBadCount);
				break;
			case COLUMN_DEVICERECEIVEDFECGOODCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceReceivedFECGoodCount);
				break;
			case COLUMN_DEVICERECEIVEDFECBADCOUNT:
				if (!table_entry) 
				{
					netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
					continue;
				}
				snmp_set_var_typed_integer (request->requestvb, ASN_COUNTER, table_entry->deviceReceivedFECBadCount);
				break;
			default:
				netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHOBJECT);
				break;
			}
		}
		break;

/*
 * Write-support
 */

	case MODE_SET_RESERVE1:
		for (request=requests; request; request=request->next) 
		{
			table_entry = (struct operationsTable_entry *) netsnmp_extract_iterator_context (request);
			table_info = netsnmp_extract_table_info (request);
			if (table_entry == NULL) 
			{
				netsnmp_set_request_error (reqinfo, request, SNMP_NOSUCHINSTANCE);
				continue;
			}
			switch (table_info->colnum) 
			{
			case COLUMN_DEVICEMACADDRESS:

/* or possibly 'netsnmp_check_vb_type_and_size' */

				ret = netsnmp_check_vb_type_and_max_size (request->requestvb, ASN_OCTET_STR, sizeof (table_entry->deviceMacAddress));
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICERESET:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEINITIALIZE:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYVLANTAGSENABLE:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG0:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG1:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG2:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG3:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG4:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG5:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG6:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG7:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTRAFFICCLASSENABLE:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTOSBITSENABLE:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS0:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS1:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS2:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS3:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS4:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS5:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS6:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS7:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP0:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP1:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP2:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP3:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTIGMP:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTUNICAST:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTIGMPMANAGEDMULTICAST:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTMULTICASTBROADCAST:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEPRIORITYDSCPENABLE:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICENMK:

/* or possiblc 'netsnmp_check_vb_type_and_size' */

				ret = netsnmp_check_vb_type_and_max_size (request->requestvb, ASN_OCTET_STR, sizeof (table_entry->deviceNMK));
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICERESETTOFACTORYDEFAULT:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEHFID:

/* or possiblc 'netsnmp_check_vb_type_and_size' */

				ret = netsnmp_check_vb_type_and_max_size (request->requestvb, ASN_OCTET_STR, sizeof (table_entry->deviceHFID));
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEMDUENABLE:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEMDUROLE:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEAVNETWORKCCOSELECTION:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			case COLUMN_DEVICEFIRMWAREUPGRADE:

/* or possibly 'netsnmp_check_vb_int_range' */

				ret = netsnmp_check_vb_int (request->requestvb);
				if (ret != SNMP_ERR_NOERROR) 
				{
					netsnmp_set_request_error (reqinfo, request, ret);
					return SNMP_ERR_NOERROR;
				}
				break;
			default:
				netsnmp_set_request_error (reqinfo, request, SNMP_ERR_NOTWRITABLE);
				return SNMP_ERR_NOERROR;
			}
		}
		break;
	case MODE_SET_RESERVE2:
		break;
	case MODE_SET_FREE:
		break;
	case MODE_SET_ACTION:
		for (request=requests; request; request=request->next) 
		{
			table_entry = (struct operationsTable_entry *) netsnmp_extract_iterator_context (request);
			table_info = netsnmp_extract_table_info (request);
			switch (table_info->colnum) 
			{
			case COLUMN_DEVICEMACADDRESS:
				memcpy (table_entry->old_deviceMacAddress, table_entry->deviceMacAddress, sizeof (table_entry->deviceMacAddress));
				table_entry->old_deviceMacAddress_len = table_entry->deviceMacAddress_len;
				memset (table_entry->deviceMacAddress, 0, sizeof (table_entry->deviceMacAddress));
				memcpy (table_entry->deviceMacAddress, request->requestvb->val.string, request->requestvb->val_len);
				table_entry->deviceMacAddress_len = request->requestvb->val_len;
				break;
			case COLUMN_DEVICERESET:
				table_entry->old_deviceReset = table_entry->deviceReset;
				table_entry->deviceReset = *request->requestvb->val.integer;

/* actually reset the device */

				hexencode (int6k.channel->peer, sizeof (int6k.channel->peer), table_entry->deviceMacAddress);
				ResetDevice (&int6k);
				break;
			case COLUMN_DEVICEINITIALIZE:
				table_entry->old_deviceInitialize = table_entry->deviceInitialize;
				table_entry->deviceInitialize = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAGSENABLE:
				table_entry->old_devicePriorityVLANTagsEnable = table_entry->devicePriorityVLANTagsEnable;
				table_entry->devicePriorityVLANTagsEnable = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG0:
				table_entry->old_devicePriorityVLANTag0 = table_entry->devicePriorityVLANTag0;
				table_entry->devicePriorityVLANTag0 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG1:
				table_entry->old_devicePriorityVLANTag1 = table_entry->devicePriorityVLANTag1;
				table_entry->devicePriorityVLANTag1 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG2:
				table_entry->old_devicePriorityVLANTag2 = table_entry->devicePriorityVLANTag2;
				table_entry->devicePriorityVLANTag2 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG3:
				table_entry->old_devicePriorityVLANTag3 = table_entry->devicePriorityVLANTag3;
				table_entry->devicePriorityVLANTag3 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG4:
				table_entry->old_devicePriorityVLANTag4 = table_entry->devicePriorityVLANTag4;
				table_entry->devicePriorityVLANTag4 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG5:
				table_entry->old_devicePriorityVLANTag5 = table_entry->devicePriorityVLANTag5;
				table_entry->devicePriorityVLANTag5 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG6:
				table_entry->old_devicePriorityVLANTag6 = table_entry->devicePriorityVLANTag6;
				table_entry->devicePriorityVLANTag6 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG7:
				table_entry->old_devicePriorityVLANTag7 = table_entry->devicePriorityVLANTag7;
				table_entry->devicePriorityVLANTag7 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTRAFFICCLASSENABLE:
				table_entry->old_devicePriorityTrafficClassEnable = table_entry->devicePriorityTrafficClassEnable;
				table_entry->devicePriorityTrafficClassEnable = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITSENABLE:
				table_entry->old_devicePriorityTOSBitsEnable = table_entry->devicePriorityTOSBitsEnable;
				table_entry->devicePriorityTOSBitsEnable = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS0:
				table_entry->old_devicePriorityTOSBits0 = table_entry->devicePriorityTOSBits0;
				table_entry->devicePriorityTOSBits0 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS1:
				table_entry->old_devicePriorityTOSBits1 = table_entry->devicePriorityTOSBits1;
				table_entry->devicePriorityTOSBits1 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS2:
				table_entry->old_devicePriorityTOSBits2 = table_entry->devicePriorityTOSBits2;
				table_entry->devicePriorityTOSBits2 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS3:
				table_entry->old_devicePriorityTOSBits3 = table_entry->devicePriorityTOSBits3;
				table_entry->devicePriorityTOSBits3 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS4:
				table_entry->old_devicePriorityTOSBits4 = table_entry->devicePriorityTOSBits4;
				table_entry->devicePriorityTOSBits4 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS5:
				table_entry->old_devicePriorityTOSBits5 = table_entry->devicePriorityTOSBits5;
				table_entry->devicePriorityTOSBits5 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS6:
				table_entry->old_devicePriorityTOSBits6 = table_entry->devicePriorityTOSBits6;
				table_entry->devicePriorityTOSBits6 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS7:
				table_entry->old_devicePriorityTOSBits7 = table_entry->devicePriorityTOSBits7;
				table_entry->devicePriorityTOSBits7 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP0:
				table_entry->old_devicePriorityTTLCap0 = table_entry->devicePriorityTTLCap0;
				table_entry->devicePriorityTTLCap0 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP1:
				table_entry->old_devicePriorityTTLCap1 = table_entry->devicePriorityTTLCap1;
				table_entry->devicePriorityTTLCap1 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP2:
				table_entry->old_devicePriorityTTLCap2 = table_entry->devicePriorityTTLCap2;
				table_entry->devicePriorityTTLCap2 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP3:
				table_entry->old_devicePriorityTTLCap3 = table_entry->devicePriorityTTLCap3;
				table_entry->devicePriorityTTLCap3 = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTIGMP:
				table_entry->old_devicePriorityDefaultIGMP = table_entry->devicePriorityDefaultIGMP;
				table_entry->devicePriorityDefaultIGMP = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTUNICAST:
				table_entry->old_devicePriorityDefaultUnicast = table_entry->devicePriorityDefaultUnicast;
				table_entry->devicePriorityDefaultUnicast = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTIGMPMANAGEDMULTICAST:
				table_entry->old_devicePriorityDefaultIGMPManagedMulticast = table_entry->devicePriorityDefaultIGMPManagedMulticast;
				table_entry->devicePriorityDefaultIGMPManagedMulticast = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTMULTICASTBROADCAST:
				table_entry->old_devicePriorityDefaultMulticastBroadcast = table_entry->devicePriorityDefaultMulticastBroadcast;
				table_entry->devicePriorityDefaultMulticastBroadcast = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEPRIORITYDSCPENABLE:
				table_entry->old_devicePriorityDSCPEnable = table_entry->devicePriorityDSCPEnable;
				table_entry->devicePriorityDSCPEnable = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICENMK:
				memcpy (table_entry->old_deviceNMK, table_entry->deviceNMK, sizeof (table_entry->deviceNMK));
				table_entry->old_deviceNMK_len = table_entry->deviceNMK_len;
				memset (table_entry->deviceNMK, 0, sizeof (table_entry->deviceNMK));
				memcpy (table_entry->deviceNMK, request->requestvb->val.string, request->requestvb->val_len);
				table_entry->deviceNMK_len = request->requestvb->val_len;
				break;
			case COLUMN_DEVICERESETTOFACTORYDEFAULT:
				table_entry->old_deviceResettoFactoryDefault = table_entry->deviceResettoFactoryDefault;
				table_entry->deviceResettoFactoryDefault = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEHFID:
				memcpy (table_entry->old_deviceHFID, table_entry->deviceHFID, sizeof (table_entry->deviceHFID));
				table_entry->old_deviceHFID_len = table_entry->deviceHFID_len;
				memset (table_entry->deviceHFID, 0, sizeof (table_entry->deviceHFID));
				memcpy (table_entry->deviceHFID, request->requestvb->val.string, request->requestvb->val_len);
				table_entry->deviceHFID_len = request->requestvb->val_len;
				break;
			case COLUMN_DEVICEMDUENABLE:
				table_entry->old_deviceMDUEnable = table_entry->deviceMDUEnable;
				table_entry->deviceMDUEnable = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEMDUROLE:
				table_entry->old_deviceMDURole = table_entry->deviceMDURole;
				table_entry->deviceMDURole = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEAVNETWORKCCOSELECTION:
				table_entry->old_deviceAVNetworkCCoSelection = table_entry->deviceAVNetworkCCoSelection;
				table_entry->deviceAVNetworkCCoSelection = *request->requestvb->val.integer;
				break;
			case COLUMN_DEVICEFIRMWAREUPGRADE:
				table_entry->old_deviceFirmwareUpgrade = table_entry->deviceFirmwareUpgrade;
				table_entry->deviceFirmwareUpgrade = *request->requestvb->val.integer;
				break;
			}
		}
		break;
	case MODE_SET_UNDO:
		for (request=requests; request; request=request->next) 
		{
			table_entry = (struct operationsTable_entry *) netsnmp_extract_iterator_context (request);
			table_info = netsnmp_extract_table_info (request);
			switch (table_info->colnum) 
			{
			case COLUMN_DEVICEMACADDRESS:
				memcpy (table_entry->deviceMacAddress, table_entry->old_deviceMacAddress, sizeof (table_entry->deviceMacAddress));
				memset (table_entry->old_deviceMacAddress, 0, sizeof (table_entry->deviceMacAddress));
				table_entry->deviceMacAddress_len = table_entry->old_deviceMacAddress_len;
				break;
			case COLUMN_DEVICERESET:
				table_entry->deviceReset = table_entry->old_deviceReset;
				table_entry->old_deviceReset = 0;
				break;
			case COLUMN_DEVICEINITIALIZE:
				table_entry->deviceInitialize = table_entry->old_deviceInitialize;
				table_entry->old_deviceInitialize = 0;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAGSENABLE:
				table_entry->devicePriorityVLANTagsEnable = table_entry->old_devicePriorityVLANTagsEnable;
				table_entry->old_devicePriorityVLANTagsEnable = 0;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG0:
				table_entry->devicePriorityVLANTag0 = table_entry->old_devicePriorityVLANTag0;
				table_entry->old_devicePriorityVLANTag0 = 0;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG1:
				table_entry->devicePriorityVLANTag1 = table_entry->old_devicePriorityVLANTag1;
				table_entry->old_devicePriorityVLANTag1 = 0;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG2:
				table_entry->devicePriorityVLANTag2 = table_entry->old_devicePriorityVLANTag2;
				table_entry->old_devicePriorityVLANTag2 = 0;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG3:
				table_entry->devicePriorityVLANTag3 = table_entry->old_devicePriorityVLANTag3;
				table_entry->old_devicePriorityVLANTag3 = 0;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG4:
				table_entry->devicePriorityVLANTag4 = table_entry->old_devicePriorityVLANTag4;
				table_entry->old_devicePriorityVLANTag4 = 0;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG5:
				table_entry->devicePriorityVLANTag5 = table_entry->old_devicePriorityVLANTag5;
				table_entry->old_devicePriorityVLANTag5 = 0;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG6:
				table_entry->devicePriorityVLANTag6 = table_entry->old_devicePriorityVLANTag6;
				table_entry->old_devicePriorityVLANTag6 = 0;
				break;
			case COLUMN_DEVICEPRIORITYVLANTAG7:
				table_entry->devicePriorityVLANTag7 = table_entry->old_devicePriorityVLANTag7;
				table_entry->old_devicePriorityVLANTag7 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTRAFFICCLASSENABLE:
				table_entry->devicePriorityTrafficClassEnable = table_entry->old_devicePriorityTrafficClassEnable;
				table_entry->old_devicePriorityTrafficClassEnable = 0;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITSENABLE:
				table_entry->devicePriorityTOSBitsEnable = table_entry->old_devicePriorityTOSBitsEnable;
				table_entry->old_devicePriorityTOSBitsEnable = 0;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS0:
				table_entry->devicePriorityTOSBits0 = table_entry->old_devicePriorityTOSBits0;
				table_entry->old_devicePriorityTOSBits0 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS1:
				table_entry->devicePriorityTOSBits1 = table_entry->old_devicePriorityTOSBits1;
				table_entry->old_devicePriorityTOSBits1 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS2:
				table_entry->devicePriorityTOSBits2 = table_entry->old_devicePriorityTOSBits2;
				table_entry->old_devicePriorityTOSBits2 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS3:
				table_entry->devicePriorityTOSBits3 = table_entry->old_devicePriorityTOSBits3;
				table_entry->old_devicePriorityTOSBits3 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS4:
				table_entry->devicePriorityTOSBits4 = table_entry->old_devicePriorityTOSBits4;
				table_entry->old_devicePriorityTOSBits4 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS5:
				table_entry->devicePriorityTOSBits5 = table_entry->old_devicePriorityTOSBits5;
				table_entry->old_devicePriorityTOSBits5 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS6:
				table_entry->devicePriorityTOSBits6 = table_entry->old_devicePriorityTOSBits6;
				table_entry->old_devicePriorityTOSBits6 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTOSBITS7:
				table_entry->devicePriorityTOSBits7 = table_entry->old_devicePriorityTOSBits7;
				table_entry->old_devicePriorityTOSBits7 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP0:
				table_entry->devicePriorityTTLCap0 = table_entry->old_devicePriorityTTLCap0;
				table_entry->old_devicePriorityTTLCap0 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP1:
				table_entry->devicePriorityTTLCap1 = table_entry->old_devicePriorityTTLCap1;
				table_entry->old_devicePriorityTTLCap1 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP2:
				table_entry->devicePriorityTTLCap2 = table_entry->old_devicePriorityTTLCap2;
				table_entry->old_devicePriorityTTLCap2 = 0;
				break;
			case COLUMN_DEVICEPRIORITYTTLCAP3:
				table_entry->devicePriorityTTLCap3 = table_entry->old_devicePriorityTTLCap3;
				table_entry->old_devicePriorityTTLCap3 = 0;
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTIGMP:
				table_entry->devicePriorityDefaultIGMP = table_entry->old_devicePriorityDefaultIGMP;
				table_entry->old_devicePriorityDefaultIGMP = 0;
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTUNICAST:
				table_entry->devicePriorityDefaultUnicast = table_entry->old_devicePriorityDefaultUnicast;
				table_entry->old_devicePriorityDefaultUnicast = 0;
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTIGMPMANAGEDMULTICAST:
				table_entry->devicePriorityDefaultIGMPManagedMulticast = table_entry->old_devicePriorityDefaultIGMPManagedMulticast;
				table_entry->old_devicePriorityDefaultIGMPManagedMulticast = 0;
				break;
			case COLUMN_DEVICEPRIORITYDEFAULTMULTICASTBROADCAST:
				table_entry->devicePriorityDefaultMulticastBroadcast = table_entry->old_devicePriorityDefaultMulticastBroadcast;
				table_entry->old_devicePriorityDefaultMulticastBroadcast = 0;
				break;
			case COLUMN_DEVICEPRIORITYDSCPENABLE:
				table_entry->devicePriorityDSCPEnable = table_entry->old_devicePriorityDSCPEnable;
				table_entry->old_devicePriorityDSCPEnable = 0;
				break;
			case COLUMN_DEVICENMK:
				memcpy (table_entry->deviceNMK, table_entry->old_deviceNMK, sizeof (table_entry->deviceNMK));
				memset (table_entry->old_deviceNMK, 0, sizeof (table_entry->deviceNMK));
				table_entry->deviceNMK_len = table_entry->old_deviceNMK_len;
				break;
			case COLUMN_DEVICERESETTOFACTORYDEFAULT:
				table_entry->deviceResettoFactoryDefault = table_entry->old_deviceResettoFactoryDefault;
				table_entry->old_deviceResettoFactoryDefault = 0;
				break;
			case COLUMN_DEVICEHFID:
				memcpy (table_entry->deviceHFID, table_entry->old_deviceHFID, sizeof (table_entry->deviceHFID));
				memset (table_entry->old_deviceHFID, 0, sizeof (table_entry->deviceHFID));
				table_entry->deviceHFID_len = table_entry->old_deviceHFID_len;
				break;
			case COLUMN_DEVICEMDUENABLE:
				table_entry->deviceMDUEnable = table_entry->old_deviceMDUEnable;
				table_entry->old_deviceMDUEnable = 0;
				break;
			case COLUMN_DEVICEMDUROLE:
				table_entry->deviceMDURole = table_entry->old_deviceMDURole;
				table_entry->old_deviceMDURole = 0;
				break;
			case COLUMN_DEVICEAVNETWORKCCOSELECTION:
				table_entry->deviceAVNetworkCCoSelection = table_entry->old_deviceAVNetworkCCoSelection;
				table_entry->old_deviceAVNetworkCCoSelection = 0;
				break;
			case COLUMN_DEVICEFIRMWAREUPGRADE:
				table_entry->deviceFirmwareUpgrade = table_entry->old_deviceFirmwareUpgrade;
				table_entry->old_deviceFirmwareUpgrade = 0;
				break;
			}
		}
		break;
	case MODE_SET_COMMIT:
		break;
	}
	return SNMP_ERR_NOERROR;
}

