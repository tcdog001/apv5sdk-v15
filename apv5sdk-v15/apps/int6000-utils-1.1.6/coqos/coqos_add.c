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
 *   coqos_add.c - Add MCMSA Stream
 *
 *   this program sends and receives raw ethernet frames and so needs
 *   root priviledges; if you install it using "chmod 555" and "chown 
 *   root:root" then you must login as root to run it; otherwise, you 
 *   can install it using "chmod 4555" and "chown root:root" so that 
 *   anyone can run it; the program will refuse to run until you get
 *   things right;
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
 *	Bill Wike <bill.wike@intellon.com>
 *	Charles Maier <charles.maier@intellon.com>
 *      Abdel Younes <younes@leacom.fr>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*"
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/symbol.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../int6k/int6k.h"
#include "../coqos/coqos.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/basespec.c"
#include "../tools/uintspec.c"
#include "../tools/hexdump.c"
#include "../tools/hexencode.c"
#include "../tools/bytespec.c"
#include "../tools/ipv4spec.c"
#include "../tools/ipv6spec.c"
#include "../tools/hexdecode.c"
#include "../tools/todigit.c"
#include "../tools/typename.c"
#include "../tools/endian.c"
#include "../tools/error.c"
#include "../tools/synonym.c"
#include "../tools/lookup.c"
#include "../tools/assist.c"
#include "../tools/expect.c"
#endif

#ifndef MAKEFILE
#include "../int6k/Request.c"
#include "../int6k/Confirm.c"
#include "../int6k/Failure.c"
#include "../int6k/Display.c"
#include "../int6k/ReadMME.c"
#include "../int6k/SendMME.c"
#include "../int6k/devices.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#endif

#ifndef MAKEFILE
#include "../ether/channel.c"
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#endif

#ifndef MAKEFILE
#include "../mme/MMECode.c"
#endif

/*====================================================================*
 *   constants;    
 *--------------------------------------------------------------------*/

#define CONTROLS 2
#define VOLATILITIES 2
#define ACTIONS 4
#define OPERANDS 2
#define OPERATORS 2
#define FIELDS 21

#define CONTROL "control"
#if 0
#define VOLATILITY "volatility"
#endif
#define ACTION "action"
#define OPERAND "operand"
#define FIELD "field"
#define OPERATOR "operator"

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

#define CONTROL_ADD 0x00
#define CONTROL_REM 0x01

static struct _code_ controls [CONTROLS] = 

{
	{
		CONTROL_ADD,
		"Add" 
	},
	{
		CONTROL_REM,
		"Rem" 
	}
};

#if 0
#define VOLATILITY_TEMP 0x00
#define VOLATILITY_PERM 0x01

static struct _code_ volatilities [VOLATILITIES] = 

{
	{
		VOLATILITY_TEMP,
		"Temp" 
	},
	{
		VOLATILITY_PERM,
		"Perm" 
	}
};
#endif

#define ACTION_CAP0 0x00
#define ACTION_CAP1 0x01
#define ACTION_CAP2 0x02
#define ACTION_CAP3 0x03

static struct _code_ actions [ACTIONS] = 

{
	{
		ACTION_CAP0,
		"CAP0" 
	},
	{
		ACTION_CAP1,
		"CAP1" 
	},
	{
		ACTION_CAP2,
		"CAP2" 
	},
	{
		ACTION_CAP3,
		"CAP3" 
	},
};

#define OPERAND_ALL 0x00
#define OPERAND_ANY 0x01

static struct _code_ operands [OPERANDS] = 

{
	{
		OPERAND_ALL,
		"All" 
	},
	{
		OPERAND_ANY,
		"Any" 
	}
};

#define FIELD_ETH_DA 0x00
#define FIELD_ETH_SA 0x01
#define FIELD_VLAN_UP 0x02
#define FIELD_VLAN_ID 0x03
#define FIELD_IPV4_TOS 0x04
#define FIELD_IPV4_PROT 0x05
#define FIELD_IPV4_SA 0x06
#define FIELD_IPV4_DA 0x07
#define FIELD_IPV6_TC 0x08
#define FIELD_IPV6_FL 0x09
#define FIELD_IPV6_SA 0x0A
#define FIELD_IPV6_DA 0x0B
#define FIELD_TCP_SP 0x0C
#define FIELD_TCP_DP 0x0D
#define FIELD_UDP_SP 0x0E
#define FIELD_UDP_DP 0x0F
#define FIELD_IP_SP 0xE0
#define FIELD_IP_DP 0xE1
#define FIELD_HPAV_MME 0xE2
#define FIELD_ETH_TYPE 0xE3
#define FIELD_TCP_ACK 0xE4

static struct _code_ fields [FIELDS] = 

{
	{
		FIELD_ETH_DA,
		"EthDA" 
	},
	{
		FIELD_ETH_SA,
		"EthSA" 
	},
	{
		FIELD_VLAN_UP,
		"VLANUP" 
	},
	{
		FIELD_VLAN_ID,
		"VLANID" 
	},
	{
		FIELD_IPV4_TOS,
		"IPv4TOS" 
	},
	{
		FIELD_IPV4_PROT,
		"IPv4PROT" 
	},
	{
		FIELD_IPV4_SA,
		"IPv4SA" 
	},
	{
		FIELD_IPV4_DA,
		"IPv4DA" 
	},
	{
		FIELD_IPV6_TC,
		"IPv6TC" 
	},
	{
		FIELD_IPV6_FL,
		"IPv6FL" 
	},
	{
		FIELD_IPV6_SA,
		"IPv6SA" 
	},
	{
		FIELD_IPV6_DA,
		"IPv6DA" 
	},
	{
		FIELD_TCP_SP,
		"TCPSP" 
	},
	{
		FIELD_TCP_DP,
		"TCPDP" 
	},
	{
		FIELD_UDP_SP,
		"UDPSP" 
	},
	{
		FIELD_UDP_DP,
		"UDPDP" 
	},
	{
		FIELD_IP_SP,
		"IPSP" 
	},
	{
		FIELD_IP_DP,
		"IPDP" 
	},
	{
		FIELD_HPAV_MME,
		"MME" 
	},
	{
		FIELD_ETH_TYPE,
		"ET" 
	},
	{
		FIELD_TCP_ACK,
		"TCPACK" 
	}
};

#define OPERATOR_IS 0x00
#define OPERATOR_NOT 0x01

static struct _code_ operators [OPERATORS] = 

{
	{
		OPERATOR_IS,
		"Is" 
	},
	{
		OPERATOR_NOT,
		"Not" 
	}
};

/*====================================================================*
 *
 *   signed add_conn (struct int6k * int6k, struct connection * connection);
 *
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Bill Wike <bill.wike@intellon.com>
 *
 *--------------------------------------------------------------------*/

static signed add_conn (struct int6k * int6k, struct connection * connection) 

{

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_add_conn_req 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		struct connection connection;
	}
	* request = (struct vs_add_conn_req *)(int6k->message);
	struct __packed vs_add_conn_cnf 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint32_t REQ_ID;
		uint8_t MSTATUS;
		uint16_t ERR_REC_CODE;
		uint32_t RSVD;
		uint16_t CID;
		uint8_t REJECT_MAC [ETHER_ADDR_LEN];
		uint16_t CSPEC_VERSION;
		uint8_t CONN_CAP;
		uint8_t CONN_COQOS_PRIO;
		uint16_t CONN_RATE;
		uint32_t CONN_TTL;
	}
	* confirm = (struct vs_add_conn_cnf *)(int6k->message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Request (int6k, "Add COQOS connection");
	memset (int6k->message, 0, sizeof (struct message));
	EthernetHeader (&int6k->message->ethernet, int6k->channel->peer, int6k->channel->host);
	IntellonHeader (&int6k->message->intellon, (VS_ADD_CONN | MMTYPE_REQ));
	memcpy (&request->connection, connection, sizeof (struct connection));
	memcpy (&request->connection.rules.CLASSIFIERS [request->connection.rules.NUM_CLASSIFIERS], &request->connection.cspec, sizeof (request->connection.cspec));
	int6k->packetsize = sizeof (struct vs_add_conn_req);
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTSEND);
		return (-1);
	}
	while (ReadMME (int6k, (VS_ADD_CONN | MMTYPE_CNF)) <= 0) 
	{
		if (confirm->MSTATUS) 
		{
			Failure (int6k, "Device won't add connection");
			return (-1);
		}
		Confirm (int6k, "Setting ...");
	}
	return (0);
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv[]);
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern struct channel channel;
	extern const struct _term_ devices [];
	static const char *optv [] = 
	{
		"ei:qv",
		"action priority destination rate ttl operand condition [...] [device] [...]\n\n          where condition is field operator value",
		"CoQos Stream Utility",
		"e\tredirect stderr messages to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"q\tquiet - suppress routine messages",
		"v\tverbose messages",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

	struct connection connection;
	struct rule * rule = (struct rule *)(&connection.rules.CLASSIFIERS);
	uint16_t * word;
	uint8_t * byte;
	signed code;
	signed c;
	if (getenv (PLCDEVICE)) 
	{
#if defined (WINPCAP) || defined (LIBPCAP)
		channel.id = atoi (getenv (PLCDEVICE));
#else
		channel.name = strdup (getenv (PLCDEVICE));
#endif
	}
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (int6k.flags, INT6K_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;

#ifndef WIN32

/*
 *      raw packets require root priviledges on linux; one does not have to be
 *      root when this program is installed setuid using 'chown root:root' and 
 *      'chmod 4555';
 */

	if (geteuid ()) 
	{
		error (1, EPERM, INT6K_NEEDROOT);
	}

#endif

	memset (&connection, 0, sizeof (connection));
	if ((code = lookup (*argv++, actions, ACTIONS)) == -1) 
	{
		assist (*--argv, ACTION, actions, ACTIONS, stderr);
	}
	connection.cspec.CONN_CAP = (uint8_t)(code);
	argc--;
	if (!argc) 
	{
		error (1, ECANCELED, "Expected Priority: 0-15");
	}
	connection.cspec.CONN_COQOS_PRIO = (uint8_t)(uintspec (*argv++, 0, 15));
	argc--;
	if (!argc) 
	{
		error (1, ECANCELED, "Expected Destination MAC Address");
	}
	if (!hexencode (connection.APP_DA, sizeof (connection.APP_DA), synonym (*argv++, devices, DEVICES))) 
	{
		error (1, errno, "Invalid MAC=[%s]", *--argv);
	}
	argc--;
	if (!argc) 
	{
		error (1, ECANCELED, "Expected Data Rate: 10-9000 (kbps)");
	}
	connection.cspec.CONN_RATE = (uint16_t)(uintspec (*argv++, 1, 9000));
	argc--;
	if (!argc) 
	{
		error (1, ECANCELED, "Expected TTL: 10000-2000000 (microseconds)");
	}
	connection.cspec.CONN_TTL = (uint32_t)(uintspec (*argv++, 10000, 2000000));
	argc--;
	if ((code = lookup (*argv++, operands, OPERANDS)) == -1) 
	{
		assist (*--argv, OPERAND, operands, OPERANDS, stderr);
	}
	connection.rules.MOPERAND = (uint8_t)(code);
	argc--;
	while ((*argv) && (lookup (*argv, controls, CONTROLS) == -1)) 
	{
		if ((code = lookup (*argv++, fields, FIELDS)) == -1) 
		{
			assist (*--argv, FIELD, fields, FIELDS, stderr);
		}
		rule->CR_ID = (uint8_t)(code);
		argc--;
		if ((code = lookup (*argv++, operators, OPERATORS)) == -1) 
		{
			assist (*--argv, OPERATOR, operators, OPERATORS, stderr);
		}
		rule->CR_OPERAND = (uint8_t)(code);
		argc--;
		if (!argc || !*argv) 
		{
			error (1, ENOTSUP, "Have %s '%s' without any value", OPERATOR, *--argv);
		}
		switch (rule->CR_ID) 
		{
		case FIELD_ETH_SA:
		case FIELD_ETH_DA:
			bytespec (*argv++, rule->CR_VALUE, ETHER_ADDR_LEN);
			break;
		case FIELD_IPV4_SA:
		case FIELD_IPV4_DA:
			ipv4spec (*argv++, rule->CR_VALUE);
			break;
		case FIELD_IPV6_SA:
		case FIELD_IPV6_DA:
			ipv6spec (*argv++, rule->CR_VALUE);
			break;
		case FIELD_VLAN_UP:
		case FIELD_IPV4_TOS:
		case FIELD_IPV4_PROT:
			byte = (uint8_t *)(rule->CR_VALUE);
			*byte = (uint8_t)(basespec (*argv++, 0, sizeof (uint8_t)));
			break;
		case FIELD_VLAN_ID:
		case FIELD_TCP_SP:
		case FIELD_TCP_DP:
		case FIELD_UDP_SP:
		case FIELD_UDP_DP:
		case FIELD_IP_SP:
		case FIELD_IP_DP:
			word = (uint16_t *)(rule->CR_VALUE);
			*word = (uint16_t)(basespec (*argv++, 0, sizeof (uint16_t)));
			*word = htons (*word);
			break;
		case FIELD_ETH_TYPE:
			word = (uint16_t *)(rule->CR_VALUE);
			*word = (uint16_t)(basespec (*argv++, 0, sizeof (uint16_t)));
			*word = HTOLE16 (*word);
			break;
		case FIELD_HPAV_MME:
			bytespec (*argv++, rule->CR_VALUE, sizeof (uint8_t) + sizeof (uint16_t));
			byte = (uint8_t *)(rule->CR_VALUE);
			endian (++byte, sizeof (uint16_t));
			break;
		case FIELD_IPV6_TC:
		case FIELD_IPV6_FL:
		case FIELD_TCP_ACK:
		default:
			error (1, ENOTSUP, "Field '%s' (0x%02X)", argv [-2], rule->CR_ID);
			break;
		}
		connection.rules.NUM_CLASSIFIERS++;
		if (connection.rules.NUM_CLASSIFIERS > 3) 
		{
			error (1, ENOTSUP, "Too many classification rules (3 max)");
		}
		rule++;
		argc--;
	}
	connection.cspec.CSPEC_VERSION = 0x0001;
	openchannel (&channel);
	if (!(int6k.message = malloc (sizeof (struct message)))) 
	{
		error (1, errno, INT6K_ERR_MEMORY);
	}
	if (!argc) 
	{
		add_conn (&int6k, &connection);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		add_conn (&int6k, &connection);
		argc--;
		argv++;
	}
	free (int6k.message);
	closechannel (&channel);
	exit (0);
}

