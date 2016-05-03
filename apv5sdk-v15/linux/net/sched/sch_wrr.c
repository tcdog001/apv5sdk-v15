/*
 * copyright (c) 2009 Atheros Communications, Inc.
 * All rights reserved
 *
 *  Weighted round-robin algorithm.
 * 
 *	The qdisc contains number of queues and every of them
 *	is assigned weight. The available bandwidth is distributed
 *	among classes proportionaly to the weights.
 * 
 *  Input Parameters:
 *		1) Number of queues. The default is 4.
 * 		2) fmark: it is assigned to each class for indicating a flow.
 *
 *  It assumes that the filtering data will be send to the root of WRR.
 *
 *
 *                         1:0 (htb)
 *                          |
 *                         1:2 (htb)
 *                          |		                        
 *                         2:0 (wrr)
 *                     /    |   |   \
 *                   2:11 2:12 2:13 2:14
 *
 *	Author:		Tos Xu   tosx@wicresoft.com
 *	Date: 		2008/12/10
 */

#include <linux/config.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/bitops.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/in.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/if_ether.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/notifier.h>
#include <net/ip.h>
#include <net/route.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <net/pkt_sched.h>

#define TOTALQUEUES	4

enum
{
	TCA_WRR_UNSPEC,
	TCA_WRR_INIT,
	TCA_WRR_PARAMS,
	__TCA_WRR_MAX,
};

#define TCA_WRR_MAX (__TCA_WRR_MAX-1)
#define WRR_DEBUG 0

struct wrr_class
{
	u32			classid;
	int			quantum;	/* Max bytes transmited at once */
	u32			nfmark;
	struct Qdisc		*q;		/* Elementary queueing discipline */

	struct tc_stats		stats;
	int			deficit;
	int			refcnt;

    	struct gnet_stats_basic bstats;
    	struct gnet_stats_queue qstats;
    	struct gnet_stats_rate_est rate_est;
};



struct wrr_sched_data
{
	int key;
	int total_classes;
	int total_queues;			
	struct wrr_class * classes[TOTALQUEUES];
	int filters;
};

struct wrr_gopt 
{
	int total_queues;
};

struct wrr_class_opt
{
	int quantum;
	int handle;
};


static __inline__ struct wrr_class *
wrr_find(u32 handle, struct Qdisc *sch)
{
    int i = 0;
    struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);
    struct wrr_class ** cl = q->classes;

    for(i=0;i< TOTALQUEUES;i++)
    {
	if(cl[i]==NULL) continue;
	if(cl[i]->classid==handle) return cl[i];
    }

    return NULL;

}

static struct wrr_class *
wrr_clasify(struct sk_buff *skb, struct Qdisc *sch)
{
    struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);

    struct wrr_class **cl = q->classes;

    int no;

    no = skb->nfmark % TOTALQUEUES;	
	
    if(cl[no]==NULL) return NULL;

    if(cl[no]->nfmark==skb->nfmark)
	return cl[no];
    else
	return NULL;
}

static int
wrr_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	struct wrr_class *cl = wrr_clasify(skb,sch);

#if WRR_DEBUG
	printk("wrr_enqueue nfmark=%d.\n",cl->nfmark);	    
#endif
	if (!cl) {
		kfree_skb(skb);
		sch->qstats.drops++;
		return NET_XMIT_DROP;
	}
	
	if (cl->q->enqueue(skb, cl->q) != NET_XMIT_SUCCESS) {
		sch->qstats.drops++;
		cl->stats.drops++;
		return NET_XMIT_DROP;
	}

	sch->q.qlen++;
	sch->bstats.packets++;
	sch->bstats.bytes+=skb->len;
	cl->stats.packets++;
	cl->stats.bytes+=skb->len;

	return NET_XMIT_SUCCESS;
}

static int
wrr_requeue(struct sk_buff *skb, struct Qdisc *sch)
{
	struct wrr_class *cl = wrr_clasify(skb,sch);

#if WRR_DEBUG
	printk("wrr_renqueue mark=%d.\n",cl->nfmark);	    
#endif

	if (!cl) {
		kfree_skb(skb);
		sch->qstats.drops++;
		return NET_XMIT_DROP;
	}
	
	if (cl->q->ops->requeue(skb, cl->q) != NET_XMIT_SUCCESS) {
		sch->qstats.drops++;
		cl->stats.drops++;
		return NET_XMIT_DROP;
	}

	sch->q.qlen++;

	return NET_XMIT_SUCCESS;
}

static struct sk_buff *
wrr_dequeue(struct Qdisc *sch)
{
	struct sk_buff *skb;
	struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);
	struct wrr_class **cl = q->classes;
	int i,j;
	int done;

	j = q->key;

	do
	{	
		done = 1;

		for(i=0;i<TOTALQUEUES;i++)
		{
			if(cl[j]==NULL)
			{
				j = (j+1)% TOTALQUEUES;
				continue;
			}

			if(cl[j]->deficit <= 0) 
			{
				if(cl[j]->q->q.qlen) done = 0;
				cl[j]->deficit += cl[j]->quantum;
				j = (j+1)% TOTALQUEUES;
				continue;	
			}		
	
			if ((skb = cl[j]->q->dequeue(cl[j]->q)) == NULL)
			{
				j = (j+1)% TOTALQUEUES; 
				continue;
			}
			cl[j]->deficit -= skb->len;

			if (cl[j]->deficit <= 0) 
			{
				j = (j+1)% TOTALQUEUES;			
			}
	
			q->key = j;
			sch->q.qlen--;
#if WRR_DEBUG
			printk("wrr_dequeue nfmark=%d.\n",skb->nfmark);
#endif
			return skb;
		}

	}while(!done);

	q->key = 0;
	return NULL;
}

static unsigned int 
wrr_drop(struct Qdisc* sch)
{
	struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);
	struct wrr_class **cl = q->classes;
	int i,len;	
	
    	for(i=0;i< TOTALQUEUES;i++)
    	{
		if(cl[i]==NULL) continue;
		if (cl[i]->q->ops->drop && (len = cl[i]->q->ops->drop(cl[i]->q)))
			return len;
    	}

	return 0;

}

static void
wrr_reset(struct Qdisc* sch)
{
	struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);
	struct wrr_class **cl = q->classes;
	int i;

	for(i=0;i< TOTALQUEUES;i++)
	{
		if(cl[i]==NULL) continue;

		qdisc_reset(cl[i]->q);
		cl[i]->deficit = cl[i]->quantum;
	}
	sch->q.qlen = 0;
}

static int 
wrr_init(struct Qdisc *sch, struct rtattr *opt)
{
	struct wrr_sched_data *q = (struct wrr_sched_data*)qdisc_priv(sch);
	struct wrr_gopt * wrr_gopt;
	struct rtattr *tb[TCA_WRR_MAX];

	memset(q, 0, sizeof(*q));
	
	q->total_queues = TOTALQUEUES;

	if(opt==NULL||rtattr_parse_nested(tb,TCA_WRR_MAX,opt))
		goto SETDEFAULT;

	if(tb[TCA_WRR_INIT-1])
	{
		if(RTA_PAYLOAD(tb[TCA_WRR_INIT-1])<sizeof(*wrr_gopt))
			goto SETDEFAULT;

		wrr_gopt = RTA_DATA(tb[TCA_WRR_INIT-1]);

		if(wrr_gopt==NULL) {
			printk("Total of default queues=%x.\n",TOTALQUEUES);
		}else
			q->total_queues = wrr_gopt->total_queues;

	}	

SETDEFAULT:
	
	if(q->total_queues <0 ||q->total_queues >8)
		return -1;
#if WRR_DEBUG
    	printk("wrr_init sch:%p, handle:%X,total queues:%x.\n",sch,sch->handle,q->total_queues);
#endif

	return 0;
}

static int 
wrr_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct wrr_sched_data *q = (struct wrr_sched_data*)qdisc_priv(sch);
	unsigned char	 *b = skb->tail;
	struct wrr_gopt gopt;
	struct rtattr *rta;

#if WRR_DEBUG
    	printk("wrr_dump sch=%p, handle=%X\n",sch,sch->handle);
#endif
	gopt.total_queues = q->total_queues;

	rta = (struct rtattr *)b;
	RTA_PUT(skb, TCA_OPTIONS, 0, NULL);
	RTA_PUT(skb, TCA_WRR_INIT, sizeof(gopt), &gopt);
	rta->rta_len = skb->tail-b;

	return skb->len;
rtattr_failure:
	skb_trim(skb, b - skb->data);
	return -1;
}

static int
wrr_dump_class(struct Qdisc *sch, unsigned long arg,
	       struct sk_buff *skb, struct tcmsg *tcm)
{
	struct wrr_class *cl = (struct wrr_class*)arg;
	unsigned char	 *b = skb->tail;
	struct wrr_class_opt copt;
	struct rtattr *rta;

#if WRR_DEBUG
    	printk("wrr_dclass sch=%p, handle=%x\n",sch,sch->handle);
#endif
	if(cl==NULL|| tcm==NULL) return -1;

	tcm->tcm_parent = TC_H_ROOT;
	tcm->tcm_handle = cl->classid;
	tcm->tcm_info = cl->q->handle;

	copt.handle = cl->nfmark;
	copt.quantum = cl->quantum;

	rta = (struct rtattr *)b;
	RTA_PUT(skb, TCA_OPTIONS, 0, NULL);
	RTA_PUT(skb, TCA_WRR_PARAMS, sizeof(copt), &copt);
	rta->rta_len = skb->tail-b;

	return skb->len;
rtattr_failure:
	skb_trim(skb, b - skb->data);
	return -1;
}

static int 
wrr_graft(struct Qdisc *sch, unsigned long arg, struct Qdisc *new,
		     struct Qdisc **old)
{
	struct wrr_class *cl = (struct wrr_class*)arg;

	if (cl) {
		if (new == NULL) {
			if ((new = qdisc_create_dflt(sch->dev, &pfifo_qdisc_ops)) == NULL)
				return -ENOBUFS;
		} 
		if ((*old = xchg(&cl->q, new)) != NULL)
			qdisc_reset(*old);
		return 0;
	}
	return -ENOENT;
}

static struct Qdisc *
wrr_leaf(struct Qdisc *sch, unsigned long arg)
{
	struct wrr_class *cl = (struct wrr_class*)arg;

	return cl ? cl->q : NULL;
}

static unsigned long 
wrr_get(struct Qdisc *sch, u32 classid)
{
 	struct wrr_class *cl = wrr_find(classid,sch);
 	if (cl) cl->refcnt++;
	return (unsigned long)cl;
}

static void 
wrr_destroy_filters(struct wrr_sched_data *q)
{

}

static void 
wrr_destroy_class(struct wrr_class *cl)
{
	qdisc_destroy(cl->q);
	kfree(cl);
}

static void
wrr_destroy(struct Qdisc* sch)
{
	struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);
 	struct wrr_class **cl = q->classes;
	struct wrr_class * p;
	int i;

#if WRR_DEBUG
	printk("wrr_destroy.\n");
#endif
	for(i=0;i< TOTALQUEUES;i++)
	{
		if(cl[i]==NULL) continue;
		p = cl[i];
		cl[i]=NULL;
		wrr_destroy_class(p);
		q->total_classes --;
	}

#if WRR_DEBUG
	printk("wrr_destroy total queues %u.\n",i);
#endif	
	wrr_destroy_filters(q);
	sch->q.qlen=0;
	q->key = 0;

}

static void 
wrr_put(struct Qdisc *sch, unsigned long arg)
{
	struct wrr_class *cl = (struct wrr_class*)arg;


	if (--cl->refcnt == 0) {
		wrr_destroy_class(cl);
	}

}

static int
wrr_change_class(struct Qdisc *sch, u32 classid, u32 parentid, struct rtattr **tca,
		 unsigned long *arg)
{
	int err;
	struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);
	struct wrr_class *cl = (struct wrr_class*)*arg;
	struct rtattr *opt = tca[TCA_OPTIONS-1];
	struct rtattr *tb[TCA_WRR_MAX];
	struct wrr_class_opt * wrr_copt;

	int no = 0;

	if(opt==NULL||rtattr_parse_nested(tb,TCA_WRR_MAX,opt))
		return -EINVAL;

	if(tb[TCA_WRR_PARAMS-1])
	{
		if(RTA_PAYLOAD(tb[TCA_WRR_PARAMS-1])<sizeof(*wrr_copt))
			return -EINVAL;

		wrr_copt = RTA_DATA(tb[TCA_WRR_PARAMS-1]);

		if(wrr_copt==NULL) 
		{
			printk("wrr_change_class wrr_copt=%p",wrr_copt);
			return -EINVAL;
		}
		if(wrr_copt->quantum < 0 ) wrr_copt->quantum = 1500;
		if(wrr_copt->handle>0x10000 ||wrr_copt->handle <0)
			return -EINVAL; 

	}else
		return -EINVAL; 	

	err = -ENOBUFS;
	cl = kmalloc(sizeof(*cl), GFP_KERNEL);

	if (cl == NULL)
		goto failure;

	memset(cl, 0, sizeof(*cl));

	cl->refcnt = 1;

	if (!(cl->q = qdisc_create_dflt(sch->dev, &pfifo_qdisc_ops)))
	{
		cl->q = &noop_qdisc;
		printk("wrr_change_class qdisc_create_dflt failed at dev %p queue:%p.\n",sch->dev,cl->q);
	}

	cl->classid = classid;
	cl->quantum = wrr_copt->quantum;
	cl->deficit = cl->quantum;
	cl->nfmark = wrr_copt->handle;

	no = cl->nfmark % TOTALQUEUES;

	/* attach to the list */

	q->key = 0;
	q->total_classes ++;

	if(q->classes[no]!=NULL) return err;

	q->classes[no] = cl;	
	
	*arg = (unsigned long)cl;

#if WRR_DEBUG
    	printk("wrr_ch_class sch=%p, handle=%x, clsid=%x, parentid=%x no:%x quantum:%x nfmark: %x\n",
	    sch,sch->handle,classid,parentid,no,cl->quantum,cl->nfmark);
#endif

	return 0;

failure:
	return err;
}

static int 
wrr_delete(struct Qdisc *sch, unsigned long arg)
{
	struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);
	struct wrr_class *cl = (struct wrr_class*)arg;
	int i ;
   
	for(i=0;i< TOTALQUEUES;i++)
		if(q->classes[i]==cl) q->classes[i] = NULL;
  
	if (--cl->refcnt == 0)
		wrr_destroy_class(cl);

	return 0;
}

static struct tcf_proto **
wrr_find_tcf(struct Qdisc *sch, unsigned long arg)
{
#if WRR_DEBUG
	printk("wrr_find_tcf arg = %lx .\n",arg);
#endif
	return NULL;
}

static unsigned long 
wrr_bind_filter(struct Qdisc *sch, unsigned long parent,
				     u32 classid)
{
	struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);
	
#if WRR_DEBUG
	printk("wrr_bind_filter parent %lx classid %x.\n",parent,classid);
#endif
	q->filters++;
	return 0;
}

static void 
wrr_unbind_filter(struct Qdisc *sch, unsigned long arg)
{
	struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);

#if WRR_DEBUG
	printk("wrr_unbind_filter arg = %lx .\n",arg);
#endif
	q->filters--;
}

static void 
wrr_walk(struct Qdisc *sch, struct qdisc_walker *arg)
{
	struct wrr_sched_data *q = (struct wrr_sched_data *)qdisc_priv(sch);
	struct wrr_class **cl = q->classes;
	int i;

	if (arg->stop)
		return;

	for(i=0;i< TOTALQUEUES;i++){
	    if (arg->count < arg->skip) {
		arg->count++;
		continue;
	    }
	    if (arg->fn(sch, (unsigned long)cl[i], arg) < 0) {
		arg->stop = 1;
		return;
	    }
	    arg->count++;

	} 
}

static int 
wrr_dump_class_stats(struct Qdisc *sch, unsigned long arg, struct gnet_dump *d)
{
	struct wrr_class *cl = (struct wrr_class*)arg;

	cl->bstats.bytes = cl->stats.bytes;
	cl->bstats.packets = cl->stats.packets;
	cl->qstats.qlen = sch->q.qlen;
	cl->qstats.drops = cl->stats.drops;

	if (gnet_stats_copy_basic(d, &cl->bstats) < 0 ||
	    gnet_stats_copy_rate_est(d, &cl->rate_est) < 0 ||
	    gnet_stats_copy_queue(d, &cl->qstats) < 0)
		return -1;

	return 0;
}

static struct Qdisc_class_ops wrr_class_ops = {
	.graft		=	wrr_graft,
	.leaf		=	wrr_leaf,
	.get		=	wrr_get,
	.put		=	wrr_put,
	.change		=	wrr_change_class,
	.delete		=	wrr_delete,
	.walk		=	wrr_walk,
	.tcf_chain	=	wrr_find_tcf,
	.bind_tcf	=	wrr_bind_filter,
	.unbind_tcf	=	wrr_unbind_filter,
	.dump		=	wrr_dump_class,
	.dump_stats	=	wrr_dump_class_stats,
};

static struct Qdisc_ops wrr_qdisc_ops = {
	.next		=	NULL,
	.cl_ops		=	&wrr_class_ops,
	.id		=	"wrr",
	.priv_size	=	sizeof(struct wrr_sched_data),
	.enqueue	=	wrr_enqueue,
	.dequeue	=	wrr_dequeue,
	.requeue	=	wrr_requeue,
	.drop		=	wrr_drop,
	.init		=	wrr_init,
	.reset		=	wrr_reset,
	.destroy	=	wrr_destroy,
	.change		=	NULL /* wrr_change */,
	.dump		=	wrr_dump,
	.owner		=	THIS_MODULE,
};


static int __init wrr_module_init(void)
{
    return register_qdisc(&wrr_qdisc_ops);
}
static void __exit wrr_module_exit(void) 
{
    unregister_qdisc(&wrr_qdisc_ops);
}

module_init(wrr_module_init)
module_exit(wrr_module_exit)



