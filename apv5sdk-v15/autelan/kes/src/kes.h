#ifndef _KES_H 
#define _KES_H
#include <linux/version.h>

#define KES_MEM_BLOCK_NAME "__kes_mem"
#define KES_PROC_NAME "kes_proc"
#define KES_FLAG_NAME "kes_flag"
/*<Begin:caizhibang add for kes 2013-02-19 */
#define KES_DMSG_NAME  "kes_dmsg"
#define KES_DEBUG_NAME      "kes_debug"
#define KES_TRAPS_NAME      "kes_traps"
#define KES_DMSG_SWITCH_NAME  "kes_dmsg_switch"
#define KES_DEBUG_SWITCH_NAME  "kes_debug_switch"
#define KES_DEBUG_FLAG_NAME      "kes_debug_flag"

#define KES_DMSG_BLOCK_SIZE  1 * 1024 * 1024
#define KES_DEBUG_BLOCK_SIZE  768*1024
#define KES_TRAPS_BLOCK_SIZE 256*1024
#define KES_MEM_BLOCK_SIZE 1*1024*1024
/*End:caizhibang add for kes 2013-02-19> */
#define CVT_BUF_MAX 1023
//#define KES_MEM_SHOW_LEN 64 * 1024
#define KES_MEM_HEADER_LEN 32
#define KES_PEOTECT_LEN 16
#define KES_MAGIC_LEN 8
#define KES_ISENABLE_LEN 8
typedef struct
{
	unsigned char protect[KES_PEOTECT_LEN];
	unsigned char magic[KES_MAGIC_LEN];
	unsigned char isenable[KES_ISENABLE_LEN];
}kes_mem_header_type;

extern int dump_msg_to_kes_mem(char *buff, int size );
extern int print_msg_to_kes_traps(const char * fmt, ...);
extern int print_msg_to_kes_debug(const char * fmt, ...);
extern int print_msg_to_kes_dmsg(char *buff, int size );

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)

#define SECS_PER_HOUR	(60 * 60)
#define SECS_PER_DAY	(SECS_PER_HOUR * 24)
static const unsigned short __mon_yday[2][13] = {
	/* Normal years. */
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
	/* Leap years. */
	{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};

struct tm {
	/*
	 * the number of seconds after the minute, normally in the range
	 * 0 to 59, but can be up to 60 to allow for leap seconds
	 */
	int tm_sec;
	/* the number of minutes after the hour, in the range 0 to 59*/
	int tm_min;
	/* the number of hours past midnight, in the range 0 to 23 */
	int tm_hour;
	/* the day of the month, in the range 1 to 31 */
	int tm_mday;
	/* the number of months since January, in the range 0 to 11 */
	int tm_mon;
	/* the number of years since 1900 */
	long tm_year;
	/* the number of days since Sunday, in the range 0 to 6 */
	int tm_wday;
	/* the number of days since January 1, in the range 0 to 365 */
	int tm_yday;
};


static int __isleap(long year)
{
	return (year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0);
}

/* do a mathdiv for long type */
static long math_div(long a, long b)
{
	return a / b - (a % b < 0);
}

/* How many leap years between y1 and y2, y1 must less or equal to y2 */
static long leaps_between(long y1, long y2)
{
	long leaps1 = math_div(y1 - 1, 4) - math_div(y1 - 1, 100)
		+ math_div(y1 - 1, 400);
	long leaps2 = math_div(y2 - 1, 4) - math_div(y2 - 1, 100)
		+ math_div(y2 - 1, 400);
	return leaps2 - leaps1;
}

static void time_to_tm(time_t totalsecs, int offset, struct tm *result)
{
	long days, rem, y;
	const unsigned short *ip;

	days = totalsecs / SECS_PER_DAY;
	rem = totalsecs % SECS_PER_DAY;
	rem += offset;
	while (rem < 0) {
		rem += SECS_PER_DAY;
		--days;
	}
	while (rem >= SECS_PER_DAY) {
		rem -= SECS_PER_DAY;
		++days;
	}

	result->tm_hour = rem / SECS_PER_HOUR;
	rem %= SECS_PER_HOUR;
	result->tm_min = rem / 60;
	result->tm_sec = rem % 60;

	/* January 1, 1970 was a Thursday. */
	result->tm_wday = (4 + days) % 7;
	if (result->tm_wday < 0)
		result->tm_wday += 7;

	y = 1970;

	while (days < 0 || days >= (__isleap(y) ? 366 : 365)) {
		/* Guess a corrected year, assuming 365 days per year. */
		long yg = y + math_div(days, 365);

		/* Adjust DAYS and Y to match the guessed year. */
		days -= (yg - y) * 365 + leaps_between(y, yg);
		y = yg;
	}

	result->tm_year = y - 1900;

	result->tm_yday = days;

	ip = __mon_yday[__isleap(y)];
	for (y = 11; days < ip[y]; y--)
		continue;
	days -= ip[y];

	result->tm_mon = y;
	result->tm_mday = days + 1;
}


#endif


#endif
