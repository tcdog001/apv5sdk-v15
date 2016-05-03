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
 *   signed LinkStatistics (struct int6k * int6k);
 *   
 *   int6k.h
 * 
 *   this plugin for int6kstat reads device link statistics using a 
 *   VS_LNK_STATS message and displays information on stdout; since
 *   the output may not suite all requirements, users are encouraged
 *   to modify as needed; there is way too much data displayed here;
 *
 *   some code is duplicated for the sake of modularity;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual
 *   for more information;
 *   
 *   This software and documentation is the property of Intellon 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Intellon assumes no responsibility or liability for 
 *   error_rate or omissions in the software or documentation and 
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
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef LINKSTATISTICS_SOURCE
#define LINKSTATISTICS_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/error.h"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define PAD ' '
#define DIGITS 16

/*====================================================================*
 *   variables;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push,1)
#endif

struct __packed transmit 

{
	uint64_t NUMTXMPDU_ACKD;
	uint64_t NUMTXMPDU_COLL;
	uint64_t NUMTXMPDU_FAIL;
	uint64_t NUMTXPBS_PASS;
	uint64_t NUMTXPBS_FAIL;
}

transmit;
struct __packed receive 

{
	uint64_t NUMRXMPDU_ACKD;
	uint64_t NUMRXMPDU_FAIL;
	uint64_t NUMRXPBS_PASS;
	uint64_t NUMRXPBS_FAIL;
	uint64_t SUMTURBOBER_PASS;
	uint64_t SUMTURBOBER_FAIL;
	uint8_t NUMRXINTERVALS;
	uint8_t RXINTERVALSTATS [1];
}

receive;
struct __packed interval 

{
	uint8_t RXPHYRATE_MBPS_0;
	uint64_t NUMRXPBS_PASS;
	uint64_t NUMRXPBS_FAIL;
	uint64_t SUMTURBOBER_PASS;
	uint64_t SUMTURBOBER_FAIL;
}

interval;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *
 *   unsigned error_rate (uint64_t passed, uint64_t failed);
 *
 *   compute error rate for a given quantity; the error rate is the
 *   ratio of failures to attempts;
 *
 *--------------------------------------------------------------------*/

float error_rate (uint64_t passed, uint64_t failed) 

{
	if ((passed) || (failed)) 
	{
		return ((float)(failed * 100) / (float)(passed + failed));
	}
	return (0);
}

/*====================================================================*
 *
 *   float fec_bit_error_rate (struct receive * receive);
 *
 *   compute the FEC-BER from the VS_LINK_STATS when DIRECTION=1 and
 *   LID=0xF8;
 *
 *--------------------------------------------------------------------*/

float fec_bit_error_rate (struct receive * receive) 

{
	float FECBitErrorRate = 0;
	if (receive->SUMTURBOBER_PASS || receive->SUMTURBOBER_FAIL) 
	{
		float TotalSumOfBitError = 100 * (float)(receive->SUMTURBOBER_PASS + receive->SUMTURBOBER_FAIL);
		float TotalSumOfBits = 8 * 520 * (float)(receive->NUMRXPBS_PASS + receive->NUMRXPBS_FAIL);
		FECBitErrorRate = TotalSumOfBitError / TotalSumOfBits;
	}
	return (FECBitErrorRate);
}

/*====================================================================*
 *
 *   void transmit (struct transmit * transmit);
 *
 *   display transmit statistics in fixed field format;
 *
 *--------------------------------------------------------------------*/

static void Transmit (struct transmit * transmit) 

{
	printf ("    TX");
	printf (" %20lld", transmit->NUMTXPBS_PASS);
	printf (" %20lld", transmit->NUMTXPBS_FAIL);
	printf (" %6.2f%%", error_rate (transmit->NUMTXPBS_PASS, transmit->NUMTXPBS_FAIL));
	printf (" %20lld", transmit->NUMTXMPDU_ACKD);
	printf (" %20lld", transmit->NUMTXMPDU_FAIL);

// printf (" %20lld", transmit->NUMTXMPDU_COLL);

	printf (" %6.2f%%", error_rate (transmit->NUMTXMPDU_ACKD, transmit->NUMTXMPDU_FAIL));
	printf ("\n");
	return;
}

/*====================================================================*
 *
 *   void Receive (struct receive * receive);
 *
 *   display receive statistics in fixed field format;
 *
 *--------------------------------------------------------------------*/

static void Receive (struct receive * receive) 

{
	printf ("    RX");
	printf (" %20lld", receive->NUMRXPBS_PASS);
	printf (" %20lld", receive->NUMRXPBS_FAIL);
	printf (" %6.2f%%", error_rate (receive->NUMRXPBS_PASS, receive->NUMRXPBS_FAIL));
	printf (" %20lld", receive->NUMRXMPDU_ACKD);
	printf (" %20lld", receive->NUMRXMPDU_FAIL);
	printf (" %6.2f%%", error_rate (receive->NUMRXMPDU_ACKD, receive->NUMRXMPDU_FAIL));
	printf ("\n");
	return;
}

/*====================================================================*
 *
 *   void Receive (struct receive * receive);
 *
 *   display receive statistics in fixed field format for each slot;
 *   the last line sumarizes results for all slots;
 *
 *--------------------------------------------------------------------*/

static void Receive2 (struct receive * receive) 

{
	struct interval * interval = (struct interval *)(receive->RXINTERVALSTATS);
	uint8_t slot = 0;
	while (slot < receive->NUMRXINTERVALS) 
	{
		printf (" %1d", slot);
		printf (" %3d", interval->RXPHYRATE_MBPS_0);
		printf (" %20lld", interval->NUMRXPBS_PASS);
		printf (" %20lld", interval->NUMRXPBS_FAIL);
		printf (" %6.2f%%", error_rate (interval->NUMRXPBS_PASS, interval->NUMRXPBS_FAIL));
		printf (" %20lld", interval->SUMTURBOBER_PASS);
		printf (" %20lld", interval->SUMTURBOBER_FAIL);
		printf (" %6.2f%%", error_rate (interval->SUMTURBOBER_PASS, interval->SUMTURBOBER_FAIL));
		printf ("\n");
		interval++;
		slot++;
	}
	printf ("   ALL");
	printf (" %20lld", receive->NUMRXPBS_PASS);
	printf (" %20lld", receive->NUMRXPBS_FAIL);
	printf (" %6.2f%%", error_rate (receive->NUMRXPBS_PASS, receive->NUMRXPBS_FAIL));
	printf (" %20lld", receive->SUMTURBOBER_PASS);
	printf (" %20lld", receive->SUMTURBOBER_FAIL);
	printf (" %6.2f%%", error_rate (receive->SUMTURBOBER_PASS, receive->SUMTURBOBER_FAIL));
	printf (" %6.2f%%", fec_bit_error_rate (receive));
	printf ("\n");
	return;
}

/*====================================================================*
 *
 *   signed LinkStatistics (struct int6k * int6k);
 *
 *
 *--------------------------------------------------------------------*/

signed LinkStatistics (struct int6k * int6k) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_lnk_stats_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MCONTROL;
		uint8_t DIRECTION;
		uint8_t LID;
		uint8_t MACADDRESS [ETHER_ADDR_LEN];
	}
	* request = (struct vs_lnk_stats_request *) (message);
	struct __packed vs_lnk_stats_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t DIRECTION;
		uint8_t LID;
		uint8_t TEI;
		uint8_t LSTATS [1];
	}
	* confirm = (struct vs_lnk_stats_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_LNK_STATS | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	request->MCONTROL = int6k->pushbutton;
	request->DIRECTION = int6k->modulecode;
	request->LID = int6k->hostaction;
	switch (request->LID) 
	{
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0xF8:
		memcpy (request->MACADDRESS, int6k->RDA, ETHER_ADDR_LEN);
		break;
	}
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_LNK_STATS | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, INT6K_WONTDOIT);
		return (-1);
	}
	if (confirm->DIRECTION == 0) 
	{
		printf ("   DIR");
		printf (" ----------- PBS PASS");
		printf (" ----------- PBS FAIL");
		printf (" PBS ERR");
		printf (" ---------- MPDU ACKD");
		printf (" ---------- MPDU FAIL");
		printf ("\n");
		Transmit ((struct transmit *)(confirm->LSTATS));
		printf ("\n");
	}
	if (confirm->DIRECTION == 1) 
	{
		printf ("   DIR");
		printf (" ----------- PBS PASS");
		printf (" ----------- PBS FAIL");
		printf (" PBS ERR");
		printf (" ---------- MPDU ACKD");
		printf (" ---------- MPDU FAIL");
		printf ("\n");
		Receive ((struct receive *)(confirm->LSTATS));
		printf ("\n");
		printf ("   PHY");
		printf (" ----------- PBS PASS");
		printf (" ----------- PBS FAIL");
		printf (" PBS ERR");
		printf (" ----------- BER PASS");
		printf (" ----------- BER FAIL");
		printf (" BER ERR");
		printf ("\n");
		Receive2 ((struct receive *)(confirm->LSTATS));
		printf ("\n");
	}
	if (confirm->DIRECTION == 2) 
	{
		printf ("   DIR");
		printf (" ----------- PBS PASS");
		printf (" ----------- PBS FAIL");
		printf (" PBS ERR");
		printf (" ---------- MPDU ACKD");
		printf (" ---------- MPDU FAIL");
		printf ("\n");
		Transmit ((struct transmit *)(confirm->LSTATS));
		Receive ((struct receive *)(confirm->LSTATS + sizeof (struct transmit)));
		printf ("\n");
		printf ("   PHY");
		printf (" ----------- PBS PASS");
		printf (" ----------- PBS FAIL");
		printf (" PBS ERR");
		printf (" ----------- BER PASS");
		printf (" ----------- BER FAIL");
		printf (" BER ERR");
		printf ("\n");
		Receive2 ((struct receive *)(confirm->LSTATS + sizeof (struct transmit)));
		printf ("\n");
	}
	return (0);
}

#endif

