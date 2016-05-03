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
 *   psgen_util.c - helpers for the psgen program
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

#ifndef PSGEN_UTIL_SOURCE
#define PSGEN_UTIL_SOURCE

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/files.h"
#include "../tools/error.h"
#include "../nda/psgen.h"
#include "../pib/pib.h"

/*
 * Read a double in from string s, scaling with a suffix;
 *
 * Returns pointer character where parsing stopped.
 */

const char * strtodouble (const char *s, double *d) 

{
	const char *p;
	int sign = 1;
	double div;
	div = 0;
	*d = 0;
	p = s;
	if (*p == '-') 
	{
		sign = -1;
		++p;
	}
	else if (*p == '+') 
	{
		sign = 1;
		++p;
	}
	while (*p != '\0') 
	{
		if (isdigit (*p)) 
		{
			if (div == 0) 
			{
				*d = *d * 10 + (*p - '0');
			}
			else 
			{
				*d = *d + (*p - '0') / div;
				div *= 10;
			}
		}
		else if (*p == '.' && div == 0) 
		{
			div = 10;
		}
		else 
		{
			break;
		}
		++p;
	}
	*d *= sign;
	switch (*p) 
	{
	case 'M':
		*d *= 1000000;
		++p;
		break;
	case 'K':
	case 'k':
		*d *= 1000;
		++p;
		break;
	}
	return p;
}

uint32_t freq_to_index (double freq, struct device_spec *d) 

{
	return rint ((freq * 40.96 / 1000000.0) - 74.0);
}

double index_to_freq (uint32_t index, struct device_spec *d) 

{
	return 1000000 * (index + 74.0) / 40.96;
}

double ps_to_amp (uint32_t ps, struct device_spec *d) 

{
	return 20 * log10 (ps / (double) d->prescaler_unity);
}

uint32_t amp_to_ps (double amp, struct device_spec *d) 

{
	double val = rint (pow (10.0, amp / 20.0) * d->prescaler_unity);
	if (val > d->prescaler_max) return d->prescaler_max + 1;
	return (uint32_t) val;
}

uint32_t lynx_freq_to_index (double freq, struct device_spec *d) 

{
	return rint ((freq / 24400 - 74.0) / 2);
}

double lynx_index_to_freq (uint32_t index, struct device_spec *d) 

{
	return (index * 2 + 74.0) * 24400;
}

uint32_t panther_freq_to_index (double freq, struct device_spec *d) 

{
	return rint (((freq / 24400) - 74.0) / 2);
}

double panther_index_to_freq (uint32_t index, struct device_spec *d) 

{
	return (index * 2 + 74.0) * 24400;
}

int create_trace_fsl3 (struct fsl3_file *fsl3_file, struct trace *trace) 

{
	int i;
	int j;
	const char *p;
	for (i = 0; i < fsl3_file->item_count; ++i) 
	{
		if (!strcasecmp (fsl3_file->item [i].name, "Values")) 
		{
			fsl3_file->trace_start = i + 1;
			fsl3_file->trace_end = fsl3_file->trace_start + atoi (fsl3_file->item [i].value) - 1;
			if (fsl3_file->trace_end > fsl3_file->item_count) 
			{
				error (1, 0, "trace extends beyond end of file");
			}
			break;
		}
	}
	if (i == fsl3_file->item_count) 
	{
		error (1, 0, "no trace data found in input file");
	}
	memset (trace, 0, sizeof (*trace));
	trace->count = fsl3_file->trace_end - fsl3_file->trace_start + 1;
	trace->freq = calloc (trace->count, sizeof (*trace->freq));
	if (trace->freq == NULL) 
	{
		error (1, 0, "could not allocate memory for trace frequency data");
	}
	trace->value = calloc (trace->count, sizeof (*trace->value));
	if (trace->value == NULL) 
	{
		error (1, 0, "could not allocate memory for trace value data");
	}
	for (i = 0, j = fsl3_file->trace_start; i < trace->count; ++i, ++j) 
	{
		p = strtodouble (fsl3_file->item [j].name, &trace->freq [i]);
		if (*p != '\0') 
		{
			error (1, 0, "invalid frequency value in input: %s", fsl3_file->item [j].name);
		}
		p = strtodouble (fsl3_file->item [j].value, &trace->value [i]);
		if (*p != '\0') 
		{
			error (1, 0, "invalid amplitude value in input: %s", fsl3_file->item [j].value);
		}
	}
	return 0;
}

int load_fsl3 (const char *path, struct fsl3_file *fsl3_file) 

{
	int c;
	off_t fsize;
	char *data;
	char *p;
	char *start;
	int count;
	FILE *fp;
	fp = fopen (path, "r");
	if (fp == NULL) 
	{
		error (1, 0, "could not open %s", path);
	}
	memset (fsl3_file, 0, sizeof (*fsl3_file));
	if (fseek (fp, 0, SEEK_END) == -1) 
	{
		error (1, errno, "failed to load input file");
	}
	fsize = ftell (fp);
	if (fseek (fp, 0, SEEK_SET) == -1) 
	{
		error (1, errno, "failed to load input file");
	}
	if (fsize == 0) 
	{
		error (1, 0, "input file is empty");
	}
	data = malloc (fsize);
	if (data == NULL) 
	{
		error (1, errno, "failed to load input file");
	}
	p = data;
	while ((c = fgetc (fp)) != EOF) 
	{
		if (c == '\n') 
		{
			++fsl3_file->item_count;
		}
		*p = c;
		++p;
	}
	fclose (fp);
	fsl3_file->item = calloc (fsl3_file->item_count, sizeof (struct fsl3_item));
	if (fsl3_file->item == NULL) 
	{
		free (data);
		error (1, errno, "failed to load input file");
	}
	p = data;
	count = 0;
	while (count < fsl3_file->item_count) 
	{

/* collect name */

		start = p;
		while (*p != ';' && *p != '\n') 
		{
			++p;
		}
		if (*p == '\n') 
		{
			error (1, errno, "unexpected end of line %d", count + 1);
		}
		*p = '\0';
		++p;
		fsl3_file->item [count].name = start;

/* collect value */

		start = p;
		while (*p != ';' && *p != '\n') 
		{
			++p;
		}
		if (*p == '\n') 
		{
			error (1, errno, "unexpected end of line %d", count + 1);
		}
		*p = '\0';
		++p;
		fsl3_file->item [count].value = start;

/* collect unit */

		start = p;
		while (*p != ';' && *p != '\n') 
		{
			++p;
		}
		if (*p == ';') 
		{
			error (1, errno, "unexpected item on line %d", count + 1);
		}
		*p = '\0';
		++p;
		fsl3_file->item [count].unit = start;
		++count;
	}
	return 0;
}

void free_fsl3_file (struct fsl3_file *fsl3_file) 

{
	free (fsl3_file->item [0].name);
	free (fsl3_file->item);
}

void free_tweaks (struct tweak *t) 

{
	if (t == NULL) 
	{
		return;
	}
	free_tweaks (t->next);
	free (t);
}

void free_dev_config (struct dev_config *dconf) 

{
	free (dconf->prescaler);
	free (dconf);
}

int parse_tweak (struct tweak *t, const char *s) 

{
	const char *p;
	p = s;
	p = strtodouble (p, &t->sf);
	if (*p != ',') 
	{
		error (1, 0, "error in tweak starting at %s", p);
	}
	++p;
	p = strtodouble (p, &t->ef);
	if (*p != ',') 
	{
		error (1, 0, "error in tweak starting at %s", p);
	}
	++p;
	p = strtodouble (p, &t->sv);
	if (*p != ',') 
	{
		error (1, 0, "error in tweak starting at %s", p);
	}
	++p;
	p = strtodouble (p, &t->ev);
	if (*p != '\0') 
	{
		error (1, 0, "error in tweak starting at %s", p);
	}
	return 0;
}

int create_trace_copy (struct trace *dst, struct trace *src) 

{
	memcpy (dst, src, sizeof (*dst));
	dst->freq = calloc (dst->count, sizeof (*dst->freq));
	if (dst->freq == NULL) 
	{
		return -1;
	}
	dst->value = calloc (dst->count, sizeof (*dst->value));
	if (dst->value == NULL) 
	{
		free (dst->freq);
		return -1;
	}
	memcpy (dst->freq, src->freq, dst->count * sizeof (*dst->freq));
	memcpy (dst->value, src->value, dst->count * sizeof (*dst->value));
	return 0;
}

int reshape_trace (struct trace *dst, struct trace *src, struct device_spec *dspec) 

{
	int i;
	dst->count = dspec->prescaler_count;
	dst->freq = calloc (dst->count, sizeof (*dst->freq));
	if (dst->freq == NULL) 
	{
		return -1;
	}
	dst->value = calloc (dst->count, sizeof (*dst->value));
	if (dst->value == NULL) 
	{
		free (dst->freq);
		return -1;
	}
	for (i = 0; i < dst->count; ++i) 
	{
		dst->freq [i] = dspec->index_to_freq (i, dspec);
		dst->value [i] = estimate_trace_value (src, dst->freq [i]);
	}
	return 0;
}

void free_trace_data (struct trace *t) 

{
	free (t->freq);
	free (t->value);
	memset (t, 0, sizeof (*t));
}


/*
 * Perform a binary search on the trace data and return
 * an index close to the requested frequency (+/-1).
 */

int find_trace_index (struct trace *t, double freq) 

{
	int min;
	int max;
	int cur;
	if (t->count == 0) 
	{
		return -1;
	}
	min = 0;
	max = t->count - 1;
	if (freq < t->freq [min] || freq > t->freq [max]) 
	{
		return -1;
	}
	while (min < max) 
	{
		cur = (min + max) / 2;
		if (t->freq [cur] > freq) 
		{
			max = cur - 1;
		}
		else if (t->freq [cur] < freq) 
		{
			min = cur + 1;
		}
		else 
		{
			return cur;
		}
	}
	return min;
}

double estimate_trace_value (struct trace *t, double freq) 

{
	int freq_index;
	double ret;
	double slope;
	freq_index = find_trace_index (t, freq);
	if (freq_index == -1) 
	{
		error (1, 0, "could not find trace value for frequency %f", freq);
	}
	if (t->freq [freq_index] > freq) 
	{
		slope = (t->value [freq_index] - t->value [freq_index - 1]) / (t->freq [freq_index] - t->freq [freq_index - 1]);
		ret = t->value [freq_index - 1] + slope * (freq - t->freq [freq_index - 1]);
	}
	else if (t->freq [freq_index] < freq) 
	{
		slope = (t->value [freq_index + 1] - t->value [freq_index]) / (t->freq [freq_index + 1] - t->freq [freq_index]);
		ret = t->value [freq_index] + slope * (freq - t->freq [freq_index]);
	}
	else 
	{
		ret = t->value [freq_index];
	}
	return ret;
}

int apply_tweak (struct trace *trace, struct tweak *tweak, int mode) 

{
	int index_sf;
	int index_ef;
	int i;
	double slope;
	index_sf = find_trace_index (trace, tweak->sf);
	if (index_sf == -1) 
	{
		error (1, 0, "could not find trace frequency %f", tweak->sf);
	}
	index_ef = find_trace_index (trace, tweak->ef);
	if (index_ef == -1) 
	{
		error (1, 0, "could not find trace frequency %f", tweak->ef);
	}
	slope = (tweak->ev - tweak->sv) / (index_ef - index_sf);
	switch (mode) 
	{
	case TWEAK_ABSOLUTE:
		for (i = index_sf; i <= index_ef; ++i) 
		{
			trace->value [i] = tweak->sv + slope * (i - index_sf);
		}
		break;
	case TWEAK_RELATIVE:
		for (i = index_sf; i <= index_ef; ++i) 
		{
			trace->value [i] += tweak->sv + slope * (i - index_sf);
		}
		break;
	default:
		return -1;
	}
	return 0;
}

struct dev_config * generate_config (struct trace *tr_old, struct trace *tr_new, struct device_spec *dspec, int gain_adj) 

{
	unsigned i;
	double old_val;
	double new_val;
	double diff;
	uint32_t ps_val;
	struct dev_config *dconf;
	dconf = malloc (sizeof (*dconf));
	if (dconf == NULL) 
	{
		error (0, 0, "could not allocate memory");
		return NULL;
	}
	dconf->prescaler = calloc (sizeof (dconf->prescaler), dspec->prescaler_count);
	if (dconf->prescaler == NULL) 
	{
		error (0, 0, "could not allocate memory");
		free (dconf);
		return NULL;
	}
	dconf->dspec = dspec;
	dconf->gain_adj = gain_adj;
	dconf->truncated = 0;
	dconf->average = 0;
	for (i = 0; i < dspec->prescaler_count; ++i) 
	{
		old_val = estimate_trace_value (tr_old, dspec->index_to_freq (i, dspec));
		new_val = estimate_trace_value (tr_new, dspec->index_to_freq (i, dspec)) - dconf->gain_adj;
		diff = new_val - old_val;
		ps_val = dspec->amp_to_ps (diff, dspec);
		if (ps_val > dspec->prescaler_max) 
		{
			ps_val = dspec->prescaler_max;
			++dconf->truncated;
		}
		dconf->prescaler [i] = ps_val;
		dconf->average += ps_val;
	}
	dconf->average /= dspec->prescaler_count;
	return dconf;
}

int print_config (struct dev_config *dconf) 

{
	unsigned i;
	for (i = 0; i < dconf->dspec->prescaler_count; ++i) 
	{
		printf ("%08d %08x\n", i, dconf->prescaler [i]);
	}
	printf ("# average is %f\n", dconf->average);
	printf ("# %d carriers truncated\n", dconf->truncated);
	printf ("# PIB output power adjust: %d\n", dconf->gain_adj);
	return 0;
}


#define GAIN_ENABLE_OFFSET_6400 0x1F6E

int set_tx_gain_6400 (struct _file_ *pib, int gain) 

{
	uint8_t gain_value;
	uint8_t gain_enable;
	if (lseek (pib->file, GAIN_ENABLE_OFFSET_6400, SEEK_SET) != GAIN_ENABLE_OFFSET_6400) 
	{
		return -1;
	}
	switch (gain) 
	{
	case -6:
		gain_value = 0x05;
		break;
	case -4:
		gain_value = 0x03;
		break;
	case -2:
		gain_value = 0x01;
		break;
	case 0:
		gain_value = 0x00;
		break;
	case 2:
	case 4:
	case 6:
		gain_value = gain;
		break;
	default:
		error (0, 0, "invalid gain value");
		return -1;
	}
	if (gain_value == 0) 
	{
		gain_enable = 0;
	}
	else 
	{
		gain_enable = 1;
	}
	if (write (pib->file, &gain_enable, sizeof (gain_enable)) != 1) 
	{
		error (1, errno, "could not enable output power level adjustment");
	}
	if (write (pib->file, &gain_value, sizeof (gain_value)) != 1) 
	{
		error (1, errno, "could set output power level adjustment");
	}
	return 0;
}

int update_pib (const char *path, struct dev_config *dconf) 

{
	struct _file_ pib;
	pib.name = path;
	pib.file = open (path, O_BINARY | O_RDWR);
	if (pib.file == -1) 
	{
		error (1, 0, "could not open PIB file %s for updating", path);
	}
	if (pibfile1 (&pib)) 
	{
		error (1, 0, "invalid PIB file");
	}
	if (psin (&pib, dconf)) 
	{
		error (1, 0, "could not update PIB");
	}
	if (piblock (&pib)) 
	{
		error (1, 0, "could not lock PIB");
	}
	if (dconf->dspec->set_tx_gain != NULL) 
	{
		if (dconf->dspec->set_tx_gain (&pib, dconf->gain_adj) == -1) 
		{
			error (1, 0, "could not update TX gain");
		}
	}
	if (piblock (&pib)) 
	{
		error (1, 0, "could not lock PIB");
	}
	close (pib.file);
	printf ("# average is %f\n", dconf->average);
	printf ("# %d carriers truncated\n", dconf->truncated);
	printf ("# PIB output power adjust: %d\n", dconf->gain_adj);
	return 0;
}

int ar7x00_psin (struct _file_ *pib, uint32_t value, uint32_t index) 

{
	off_t offset = AMP_PRESCALER_OFFSET + (index * 10 / 8);
	uint8_t bit_offset = (index * 10) % 8;
	uint16_t tmp;
	if (lseek (pib->file, offset, SEEK_SET) != offset) 
	{
		return -1;
	}
	if (read (pib->file, &tmp, sizeof (tmp)) != sizeof (tmp)) 
	{
		return -1;
	}
	if (lseek (pib->file, offset, SEEK_SET) != offset) 
	{
		return -1;
	}
	value &= 0x03FF;
	tmp = LE16TOH (tmp);
	tmp &= ~(0x03FF << bit_offset);
	tmp |= value << bit_offset;
	tmp = HTOLE16 (tmp);
	if (write (pib->file, &tmp, sizeof (tmp)) != sizeof (tmp)) 
	{
		return -1;
	}
	return 0;
}


#define AMP_CARRIERS 2880
#define INT_CARRIERS 1155

int psin (struct _file_ *pib, struct dev_config *dconf) 

{
	uint32_t value;
	unsigned limit;
	unsigned i;
	limit = pibscalers (pib);
	if ((limit != INT_CARRIERS) && (limit != AMP_CARRIERS)) 
	{
		error (1, 0, "Don't understand this PIB's prescaler format");
	}
	if (limit == INT_CARRIERS) 
	{
		if (lseek (pib->file, INT_PRESCALER_OFFSET, SEEK_SET) != INT_PRESCALER_OFFSET) 
		{
			error (1, errno, "Can't seek %s", pib->name);
		}
	}
	if (dconf->dspec->prescaler_count != limit) 
	{
		error (1, 0, "PIB file type does not match device target type");
	}
	for (i = 0; i < limit; ++i) 
	{
		value = dconf->prescaler [i];
		if (limit == INT_CARRIERS) 
		{
			if (write (pib->file, &value, sizeof (value)) != sizeof (value)) 
			{
				error (1, errno, "Can't save %s", pib->name);
			}
		}
		else if (limit == AMP_CARRIERS) 
		{
			if (value & ~0x03FF) 
			{
				error (1, errno, "Position %d has invalid prescaler value", i);
			}
			if (ar7x00_psin (pib, value, i)) 
			{
				error (1, errno, "Can't update %s", pib->name);
			}
		}
	}
	return 0;
}

int check_tx_gain_6400 (int gain_adj) 

{
	if ((gain_adj & 1) || gain_adj < -6 || gain_adj > 6) 
	{
		error (1, 0, "invalid gain adjustment, must be one of -6, -4, -2, 0, 2, 4, 6");
		return -1;
	}
	return 0;
}


#ifdef WIN32

double rint (double x) 

{
	return floor (x + 0.5);
}


#endif

#endif

