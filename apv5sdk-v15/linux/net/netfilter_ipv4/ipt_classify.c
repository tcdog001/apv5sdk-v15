/* 
 * Copyright (c) 2009 Atheros Communications, Inc.
 * All rights reserved
 *
 * Author: Tos Xu    tosx@wicresoft.com    2008/12/18
 *
 * IP tables module for matching the value of the IPv4 priority field
 *
 * ipt_prioriy.c,v 1.3 2008/12/18 19:00:21 laforge Exp
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter_ipv4/ip_tables.h>
#include "ipt_classify.h"

MODULE_AUTHOR("Tos Xu");
MODULE_DESCRIPTION("iptables priority matching module");

static int match(const struct sk_buff *skb, const struct net_device *in,
		 const struct net_device *out, const void *matchinfo,
		 int offset, int *hotdrop)
{
	const struct ipt_classify_match_info *info = matchinfo;

	return (skb->priority == info->priority) ^ info->invert; 

}

static int checkentry(const char *tablename, const struct ipt_ip *ip,
		      void *matchinfo, unsigned int matchsize,
		      unsigned int hook_mask)
{
	if (matchsize != IPT_ALIGN(sizeof(struct ipt_classify_match_info)))
		return 0;

	return 1;
}

static struct ipt_match classify_match = {
	.name		= "priority",
	.match		= &match,
	.checkentry	= &checkentry,
	.me		= THIS_MODULE,
};

static int __init init(void)
{
	return ipt_register_match(&classify_match);
}

static void __exit fini(void)
{
	ipt_unregister_match(&classify_match);

}

module_init(init);
module_exit(fini);
