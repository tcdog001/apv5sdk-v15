/*====================================================================*
 *
 *   oHPAVKey.hpp - oHPAVKey class declaration;
 *
 *   implement HomePlug AV compliant password encryption;
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
 *.  Intellon HomePlug AV Application Programming Package;
 *:  Published 2007-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration and evaluation only; Not for production use;
 *
 *   Contributor(s): 
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef ooHPAVKEY_HEADER
#define ooHPAVKEY_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include "../classes/stdafx.hpp"
#include "../classes/oSHA256.hpp"

/*====================================================================*
 *   class constants;
 *--------------------------------------------------------------------*/

#define oHPAVKEY_CHAR_MIN 0x20
#define oHPAVKEY_CHAR_MAX 0x7E
#define oHPAVKEY_PHRASE_MIN 12
#define oHPAVKEY_PHRASE_MAX 64

#define oHPAVKEY_N_SHA 0
#define oHPAVKEY_N_DAK 1
#define oHPAVKEY_N_NMK 2
#define oHPAVKEY_N_NID 3

#define oHPAVKEY_B_VERBOSE (1 << 0)
#define oHPAVKEY_B_SILENCE (1 << 1)
#define oHPAVKEY_B_ENFORCE (1 << 2)

/*====================================================================*
 *   class interface;
 *--------------------------------------------------------------------*/

class __declspec (dllexport) oHPAVKey: private oSHA256 

{
public:
	oHPAVKey ();
	~oHPAVKey ();
	byte * Digest () const;
	bool IllegalPassPhrase (char const * phrase) const;
	bool IllegalCharValue (unsigned c) const;
	oHPAVKey & ExportKey (void * memory);
	oHPAVKey & ComputeNMK (char const * string);
	oHPAVKey & ComputeDAK (char const * string);
	oHPAVKey & ComputeNID (byte level);
	static unsigned const DigestLength;
	static unsigned const DAKLength;
	static unsigned const NMKLength;
	static unsigned const NIDLength;
	static unsigned const MinPhraseLength;
	static unsigned const MaxPhraseLength;
	static byte const MinCharValue;
	static byte const MaxCharValue;
	oHPAVKey & Print (char const * phrase);
private:
	oHPAVKey & Hash (unsigned count);
	byte * mdigest;
	size_t mlength;
};

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif 

