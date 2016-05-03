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
 *   const char * MMEName (uint16_t MMTYPE);
 *
 *   mme.h
 *
 *   Return HomePlug or Intellon Management Message name for a given 
 *   MMTYPE; this function is not needed but it could be useful when
 *   developing and debugging applications;
 *
 *   If you add or remove items in this list then update constant
 *   MMTYPES to reflect the number of list members;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual 
 *   for more information about Management Messages;
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
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef MMENAME_SOURCE
#define MMENAME_SOURCE

#include "../int6k/homeplug.h"
#include "../mme/mme.h"

#define MMENAMES 106

static struct 

{
	uint16_t type;
	const char * name;
}

MMENames [MMENAMES] = 

{
	{
		0x0000,
		"CC_CCO_APPOINT" 
	},
	{
		0x0004,
		"CC_BACKUP_APPOINT" 
	},
	{
		0x0008,
		"CC_LINK_INFO" 
	},
	{
		0x000C,
		"CC_HANDOVER" 
	},
	{
		0x0010,
		"CC_HANDOVER_INFO" 
	},
	{
		0x0014,
		"CC_DISCOVER_LIST" 
	},
	{
		0x0018,
		"CC_LINK_NEW" 
	},
	{
		0x001C,
		"CC_LINK_MOD" 
	},
	{
		0x0020,
		"CC_LINK_SQZ" 
	},
	{
		0x0024,
		"CC_LINK_REL" 
	},
	{
		0x0028,
		"CC_DETECT_REPORT" 
	},
	{
		0x002C,
		"CC_WHO_RU" 
	},
	{
		0x0030,
		"CC_ASSOC" 
	},
	{
		0x0034,
		"CC_LEAVE" 
	},
	{
		0x0038,
		"CC_SET_TEI_MAP" 
	},
	{
		0x003C,
		"CC_RELAY" 
	},
	{
		0x0040,
		"CC_BEACON_RELIABILITY" 
	},
	{
		0x0044,
		"CC_ALLOC_MOVE" 
	},
	{
		0x0048,
		"CC_ACCESS_NEW" 
	},
	{
		0x004C,
		"CC_ACCESS_REL" 
	},
	{
		0x0050,
		"CC_DCPPC" 
	},
	{
		0x0054,
		"CC_HP1_DET" 
	},
	{
		0x0058,
		"CC_BLE_UPDATE" 
	},
	{
		0x2000,
		"CP_PROXY_APPOINT" 
	},
	{
		0x2004,
		"PH_PROXY_APPOINT" 
	},
	{
		0x2008,
		"CP_PROXY_WAKE" 
	},
	{
		0x4000,
		"NN_INL" 
	},
	{
		0x4004,
		"NN_NEW_NET" 
	},
	{
		0x4008,
		"NN_ADD_ALLOC" 
	},
	{
		0x400C,
		"NN_REL_ALLOC" 
	},
	{
		0x4010,
		"NN_REL_NET" 
	},
	{
		0x6000,
		"CM_ASSOCIATED_STA" 
	},
	{
		0x6004,
		"CM_ENCRYPTED_PAYLOAD" 
	},
	{
		0x6008,
		"CM_SET_KEY" 
	},
	{
		0x600C,
		"CM_GET_KEY" 
	},
	{
		0x6010,
		"CM_SC_JOIN" 
	},
	{
		0x6014,
		"CM_CHAN_EST" 
	},
	{
		0x6018,
		"CM_TM_UPDATE" 
	},
	{
		0x601C,
		"CM_AMP_MAP" 
	},
	{
		0x6020,
		"CM_BRG_INFO" 
	},
	{
		0x6024,
		"CM_CONN_NEW" 
	},
	{
		0x6028,
		"CM_CONN_REL" 
	},
	{
		0x602C,
		"CM_CONN_MOD" 
	},
	{
		0x6030,
		"CM_CONN_INFO" 
	},
	{
		0x6034,
		"CM_STA_CAP" 
	},
	{
		0x6038,
		"CM_NW_INFO" 
	},
	{
		0x603C,
		"CM_GET_BEACON" 
	},
	{
		0x6040,
		"CM_HFID" 
	},
	{
		0x6044,
		"CM__ERROR" 
	},
	{
		0x6048,
		"CM_NW_STATS" 
	},
	{
		0x8000,
		"MS_PB_ENC" 
	},
	{
		0xA000,
		"VS_SW_VER" 
	},
	{
		0xA004,
		"VS_WR_MEM" 
	},
	{
		0xA008,
		"VS_RD_MEM" 
	},
	{
		0xA00C,
		"VS_ST_MAC" 
	},
	{
		0xA010,
		"VS_GET_NVM" 
	},
	{
		0xA014,
		"VS_RSVD_1" 
	},
	{
		0xA018,
		"VS_RSVD_2" 
	},
	{
		0xA01C,
		"VS_RS_DEV" 
	},
	{
		0xA020,
		"VS_WR_MOD" 
	},
	{
		0xA024,
		"VS_RD_MOD" 
	},
	{
		0xA028,
		"VS_MOD_NVM" 
	},
	{
		0xA02C,
		"VS_WD_RPT" 
	},
	{
		0xA030,
		"VS_LNK_STATS" 
	},
	{
		0xA034,
		"VS_SNIFFER" 
	},
	{
		0xA038,
		"VS_NW_INFO" 
	},
	{
		0xA03C,
		"VS_RSVD_3" 
	},
	{
		0xA040,
		"VS_CP_RPT" 
	},
	{
		0xA044,
		"VS_ARPC" 
	},
	{
		0xA048,
		"VS_FR_LBK" 
	},
	{
		0xA04C,
		"VS_LBK_STAT" 
	},
	{
		0xA050,
		"VS_SET_KEY" 
	},
	{
		0xA054,
		"VS_MFG_STRING" 
	},
	{
		0xA058,
		"VS_RD_CBLOCK" 
	},
	{
		0xA05C,
		"VS_SET_SDRAM" 
	},
	{
		0xA060,
		"VS_HOST_ACTION" 
	},
	{
		0xA064,
		"VS_RSVD_7" 
	},
	{
		0xA068,
		"VS_OP_ATTRIBUTES" 
	},
	{
		0xA06C,
		"VS_ENET_SETTINGS" 
	},
	{
		0xA070,
		"VS_TONE_MAP_CHAR" 
	},
	{
		0xA074,
		"VS_NW_INFO_STATS" 
	},
	{
		0xA078,
		"VS_SLAVE_MEM" 
	},
	{
		0xA07C,
		"VS_FAC_DEFAULTS" 
	},
	{
		0xA080,
		"VS_PTS_NVM" 
	},
	{
		0xA084,
		"VS_MCAST_INFO" 
	},
	{
		0xA088,
		"VS_CLASSIFICATION" 
	},
	{
		0xA08C,
		"VS_DIAG_STAGE" 
	},
	{
		0xA090,
		"VS_RX_TONE_MAP_CHAR" 
	},
	{
		0xA094,
		"VS_SET_LED_BEHAVIOR" 
	},
	{
		0xA098,
		"VS_SET_SDRAM_DENALI" 
	},
	{
		0xA09C,
		"VS_MDIO_CMD" 
	},
	{
		0xA0A0,
		"VS_SLAVE_REG" 
	},
	{
		0xA0A4,
		"VS_BANDWIDTH_LIMITING" 
	},
	{
		0xA0A8,
		"VS_SNID_OPERATION" 
	},
	{
		0xA0AC,
		"VS_NN_MITIGATE" 
	},
	{
		0xA0B0,
		"VS_MODULE_OPERATION" 
	},
	{
		0xA0B4,
		"VS_DIAG_NETWORK_PROBE" 
	},
	{
		0xA0B8,
		"VS_PL_LINK_STATUS" 
	},
	{
		0xA0BC,
		"VS_GPIO_STATE_CHANGE",
	},
	{
		0xA0C0,
		"VS_CONN_ADD" 
	},
	{
		0xA0C4,
		"VS_CONN_MOD" 
	},
	{
		0xA0C8,
		"VS_CONN_REL" 
	},
	{
		0xA0CC,
		"VS_CONN_INFO" 
	},
	{
		0xA0D0,
		"VS_MULTIPORT_LNK_STA" 
	},
	{
		0xA0D0,
		"VS_CUSTOM_INFO" 
	},
	{
		0xA0D0,
		"VS_EM_ID_TABLE" 
	}
};

const char *MMEName (uint16_t MMTYPE) 

{
	size_t lower = 0;
	size_t upper = MMENAMES;
	MMTYPE &= MMTYPE_MASK;
	while (lower < upper) 
	{
		size_t index = (lower + upper) >> 1;
		signed order = MMTYPE - MMENames [index].type;
		if (order < 0) 
		{
			upper = index - 0;
			continue;
		}
		if (order > 0) 
		{
			lower = index + 1;
			continue;
		}
		return (MMENames [index].name);
	}
	return ("UNKNOWN_MME");
}

#endif

