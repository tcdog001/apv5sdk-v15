/*====================================================================*
 *   
 *   Copyright (c) 2011, Atheros Communications Inc.
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
 *   psgen.c - generate prescaler values based on trace results; 
 *
 *   This software and documentation is the property of Atheros 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Atheros assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Atheros customers may modify and distribute the software 
 *   without obligation to Atheros. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of it's use. 
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *	Nathaniel Houghton <nathaniel.houghton@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/version.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../tools/types.h"
#include "../tools/symbol.h"
#include "../nda/psgen.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/error.c"
#include "../tools/lookup.c"
#include "../tools/fdchecksum32.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibscalers.c"
#include "../pib/pibfile1.c"
#include "../pib/piblock.c"
#endif

#ifndef MAKEFILE
#include "../nda/psgen_util.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define PSGEN_SILENCE (1 << 0)
#define PSGEN_VERBOSE (1 << 1)
#define PSGEN_FLATTEN (1 << 2)
#define PSGEN_NOTCH   (1 << 3)

/*====================================================================*
 *   notches for USA;
 *--------------------------------------------------------------------*/

struct notch usa_notches [] = 

{
	{
		1806640.625,
		2075195.3125
	},
	{
		3417968.75,
		4077148.4375
	},
	{
		5249023.4375,
		5493164.0625
	},
	{
		6909179.6875,
		7373046.875
	},
	{
		10009765.625,
		10229492.1875
	},
	{
		13916015.625,
		14428710.9375
	},
	{
		17993164.0625,
		18261718.75
	},
	{
		20922851.5625,
		21533203.125
	},
	{
		24804687.5,
		25073242.1875
	}
};


/*====================================================================*
 *   notches for Japan;
 *--------------------------------------------------------------------*/

struct notch japan_notches [] = 

{
	{
		1806640.625,
		2099609.375
	},
	{
		3417968.75,
		4101562.5
	},
	{
		5249023.4375,
		5493164.0625
	},
	{
		5932617.1875,
		6225585.9375
	},
	{
		6884765.625,
		7421875.0
	},
	{
		9472656.25,
		9863281.25
	},
	{
		9985351.5625,
		10278320.3125
	},
	{
		13891601.5625,
		14453125.0
	},
	{
		17993164.0625,
		18261718.75
	},
	{
		20922851.5625,
		21533203.125
	},
	{
		24804687.5,
		25073242.1875
	}
};


/*====================================================================*
 *   notch sets;
 *--------------------------------------------------------------------*/

#define NOTCH_SET_USA   0
#define NOTCH_SET_JAPAN 1
#define NOTCH_SETS      2

struct _code_ notch_set_name [NOTCH_SETS] = 

{
	{
		NOTCH_SET_USA,
		"usa"
	},
	{
		NOTCH_SET_JAPAN,
		"japan"
	}
};

struct notch_set notch_sets [NOTCH_SETS] = 

{
	{
		sizeof (usa_notches) / sizeof (usa_notches [0]),
		-1000,
		usa_notches
	},
	{
		sizeof (japan_notches) / sizeof (japan_notches [0]),
		-1000,
		japan_notches
	}
};


#define DEVICE_SPEC_AR6400  0
#define DEVICE_SPEC_AR7400  1
#define DEVICE_SPEC_PANTHER 2
#define DEVICE_SPEC_LYNX    3
#define DEVICE_SPECS        4

#define AR7400_NAME "ar7400"
#define AR6400_NAME "ar6400"
#define AR7420_NAME "ar7420"
#define AR7421_NAME "ar7421"

struct _code_ device_spec_name [DEVICE_SPECS] = 

{
	{
		DEVICE_SPEC_AR6400,
		AR6400_NAME
	},
	{
		DEVICE_SPEC_AR7400,
		AR7400_NAME
	},
	{
		DEVICE_SPEC_PANTHER,
		AR7420_NAME
	},
	{
		DEVICE_SPEC_LYNX,
		AR7421_NAME
	}
};

struct device_spec device_spec [DEVICE_SPECS] = 

{
	{
		AR6400_NAME,
		1155,
		256,
		511,
		1070,
		freq_to_index,
		index_to_freq,
		amp_to_ps,
		ps_to_amp,
		update_pib,
		set_tx_gain_6400,
		check_tx_gain_6400
	},
	{
		AR7400_NAME,
		2880,
		512,
		1023,
		2690,
		freq_to_index,
		index_to_freq,
		amp_to_ps,
		ps_to_amp,
		update_pib,
		NULL,
		NULL
	},
	{
		AR7420_NAME,
		1345,
		128,
		255,
		-1,
		panther_freq_to_index,
		panther_index_to_freq,
		amp_to_ps,
		ps_to_amp,
		NULL,
		NULL,
		NULL
	},
	{
		AR7421_NAME,
		578,
		128,
		255,
		-1,
		lynx_freq_to_index,
		lynx_index_to_freq,
		amp_to_ps,
		ps_to_amp,
		NULL,
		NULL,
		NULL
	}
};

int main (int argc, const char * argv []) 

{
	static const char * optv [] = 
	{
		"d:F:G:i:N:P:T:v",
		"input_file [> output file]",
		"Prescaler Generator",
		"d s\tTarget device is s [ar6400]",
		"F n\tFlatten trace to n dB",
		"G n\tOutput power level gain adjustment is n dB",
		"i s\tInput format is s [fsl3]",
		"N s\tNotch to match regulatory requirements for s (usa, japan)",
		"P f\tSave prescalers and gain to pib file f",
		"T sf,ef,sd,ed\tTweak prescalers from sf to ef (start freq, end freq) by sd (start delta) at sf, linearly changing to ed (end delta) at ef",
		"v\tverbose messages",
		(const char *) (0)
	};
	flag_t flags = (flag_t)(0);
	signed c;
	double flatten;
	int gain_adj = 0;
	char * notch_type = "usa";
	int notch_set;
	char * target_device = "ar6400";
	int device_type;
	char * input_format = "fsl3";
	char * pib_path = NULL;
	struct fsl3_file fsl3_file;
	struct trace trace;
	struct trace orig_trace;
	struct trace shaped_trace;
	struct tweak * tweak = NULL;
	struct tweak * tweak_tail = NULL;
	struct dev_config * dconf;
	struct device_spec * dspec;
	const char * p;
	struct tweak * t;
	int i;
	optind = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 'd':
			target_device = optarg;
			break;
		case 'F':
			_setbits (flags, PSGEN_FLATTEN);
			p = strtodouble (optarg, &flatten);
			if (*p != '\0') 
			{
				error (1, 0, "invalid value to option -F: %s", optarg);
			}
			break;
		case 'G':
			gain_adj = atoi (optarg);
			break;
		case 'i':
			input_format = optarg;
			break;
		case 'N':
			_setbits (flags, PSGEN_NOTCH);
			notch_type = optarg;
			break;
		case 'P':
			pib_path = optarg;
			break;
		case 'T':
			{
				struct tweak * new_tweak;
				new_tweak = malloc (sizeof (*new_tweak));
				if (new_tweak == NULL) 
				{
					error (1, 0, "too many tweaks -- ran out of memory");
				}
				memset (new_tweak, 0, sizeof (*new_tweak));
				if (parse_tweak (new_tweak, optarg) == -1) 
				{
					error (1, 0, "bad tweak request: %s", optarg);
				}
				if (tweak_tail != NULL) 
				{
					tweak_tail->next = new_tweak;
				}
				if (tweak == NULL) 
				{
					tweak = new_tweak;
				}
				tweak_tail = new_tweak;
			}
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (argc != 1) 
	{
		error (1, 0, "must provide one argument for the filename");
	}
	device_type = lookup (target_device, device_spec_name, DEVICE_SPECS);
	if (device_type == -1) 
	{
		error (1, 0, "unknown device type \"%s\"", target_device);
	}
	dspec = &device_spec [device_type];
	notch_set = lookup (notch_type, notch_set_name, NOTCH_SETS);
	if (notch_set == -1) 
	{
		error (1, 0, "unknown notch set \"%s\"", notch_type);
	}
	if (strcasecmp (input_format, "fsl3")) 
	{
		error (1, 0, "only input format fsl3 is currently supported");
	}
	if (gain_adj != 0) 
	{
		if (dspec->check_tx_gain == NULL) 
		{
			error (1, 0, "gain adjustment is not (yet) supported for device type %s", target_device);
		}
		if (dspec->check_tx_gain (gain_adj) == -1) 
		{
			error (1, 0, "invalid gain adjustment provided");
		}
	}
	if (load_fsl3 (argv [0], &fsl3_file) == -1) 
	{
		error (1, 0, "failed to load %s", argv [0]);
	}
	if (create_trace_fsl3 (&fsl3_file, &orig_trace) == -1) 
	{
		error (1, 0, "could not convert input file into internal format");
	}
	if (reshape_trace (&shaped_trace, &orig_trace, dspec) == -1) 
	{
		error (1, 0, "could not reshape trace to match prescalers");
	}

/* make copy of shaped trace */

	if (create_trace_copy (&trace, &shaped_trace) == -1) 
	{
		error (1, 0, "could not copy trace");
	}

/* flatten trace */

	if (_anyset (flags, PSGEN_FLATTEN)) 
	{
		for (i = 0; i < trace.count; ++i) 
		{
			trace.value [i] = flatten;
		}
	}

/* apply notching */

	if (_anyset (flags, PSGEN_NOTCH)) 
	{
		for (i = 0; i < notch_sets [notch_set].count; ++i) 
		{
			struct tweak tw;
			tw.sf = notch_sets [notch_set].notch [i].sf;
			tw.ef = notch_sets [notch_set].notch [i].ef;
			tw.ev = tw.sv = notch_sets [notch_set].depth;
			apply_tweak (&trace, &tw, TWEAK_ABSOLUTE);
		}

/* remove the tail (if any) */

		if (dspec->tail_start != -1) 
		{
			for (i = dspec->tail_start; i < trace.count; ++i) 
			{
				trace.value [i] = -1000;
			}
		}
	}

/* apply tweaks */

	for (t = tweak; t != NULL; t = t->next) 
	{
		apply_tweak (&trace, t, TWEAK_RELATIVE);
	}
	dconf = generate_config (&shaped_trace, &trace, dspec, gain_adj);
	if (dconf == NULL) 
	{
		error (1, 0, "could not generate device configuration");
	}

/* generate prescalers based on device spec, two traces, and gain adjustment */

	if (pib_path == NULL) 
	{
		print_config (dconf);
	}
	else 
	{
		if (dspec->update_pib == NULL) 
		{
			error (1, 0, "a PIB is not yet supported for device type %s", dspec->name);
		}
		if (dspec->update_pib (pib_path, dconf) == -1) 
		{
			error (1, 0, "could not save configuration to PIB");
		}
	}
	free_fsl3_file (&fsl3_file);
	free_trace_data (&trace);
	free_trace_data (&orig_trace);
	free_trace_data (&shaped_trace);
	free_dev_config (dconf);
	free_tweaks (tweak);
	return (0);
}

