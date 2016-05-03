/* iptables module for matching the IPv4 priority field
 *
 * (C) 2008 Tos Xu 
 * This software is distributed under GNU GPL v2, 1991
 * 
*/
#ifndef _IPT_CLASSIFY_MATCH_H
#define _IPT_CLASSIFY_MATCH_H

/* match info */
struct ipt_classify_match_info {
	__u32 priority;
	u_int8_t invert;
};

#endif /* _IPT_CLASSIFY_MATCH_H */
