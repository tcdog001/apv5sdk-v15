/**********************************************************************  
FILE NAME : kes.c
Author : libing
Version : V1.0
Date : 201109013 
Description : 

Dependence :
	
Others : 
	
Function List :
1.kes_proc_init()
2.kes_proc_open()
3.kes_seq_start()
4.kes_seq_next()
5.kes_seq_show()
6.kes_seq_stop()
7.kes_addr_get()
8.print_msg_to_kes()
9.dump_msg_to_kes()
10.do_percentm()

History: 
1. Date:20110913
Author: libing
Modification: V1.0
**********************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/time.h>
#include <asm/page.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <linux/fs.h>


#include "kes.h"
//#include "cvmx-bootmem.h"
//#include "octeon-hal-read-write.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("<libing@autelan.com>");
MODULE_DESCRIPTION("Kernel Exception Saver Module");
extern int (*kes_mem_dump_handle)(char *buff, int size);
extern void (*print_current_time_handle)(void);
/*<Begin:caizhibang add for kes 2013-02-19 */
extern int (*kes_traps_print_handle)(const char *fmt, ...);
extern int (*kes_dmsg_print_handle)(char *buff, int size);
extern int (*kes_debug_print_handle)(const char *fmt, ...);
extern unsigned long num_physpages;
#define   MEM_SIZE    ((num_physpages << (PAGE_SHIFT-10))/1024)      //auto get mem size
extern void(*string_test_handle)();
extern void (*kes_debug_print_flag_handle)(const char *s);
/*End:caizhibang add for kes 2013-02-19> */

/*-----------------Begin of the traps section ----------------------*/

static char trapsbuf [CVT_BUF_MAX + 1] = {0};
unsigned char *kes_traps_addr = NULL;
unsigned char *kes_traps_print_addr = NULL;
kes_mem_header_type *kes_traps_header = NULL;

unsigned int kes_traps_offset = 0;
#define KES_MEM_SHOW_LEN  PAGE_SIZE
#define traps_page_count  (KES_TRAPS_BLOCK_SIZE/KES_MEM_SHOW_LEN)
#define TRAPS_START_ADDR (MEM_SIZE<<20)
#define page_to_virt(page)	((((page) - mem_map) << PAGE_SHIFT) + PAGE_OFFSET)

/* Find %m in the input string and substitute an error message string. */
static int do_percentm (char *obuf, const char *ibuf)
{
	const char *s = ibuf;
	char *p = obuf;
	int infmt = 0;
	const char *m;
	int len = 0;

	while (*s) 
	{
		if (infmt)
		{
			if (*s == 'm') 
			{		
				m = "<format error>";
				
				len += strlen (m);
				if (len > CVT_BUF_MAX)
				{
					goto out;
				}
				strcpy (p - 1, m);
				p += strlen (p);
				++s;
			} 
			else 
			{
				if (++len > CVT_BUF_MAX)
					goto out;
				*p++ = *s++;
			}
			infmt = 0;
		}
		else
		{
			if (*s == '%')
				infmt = 1;
			if (++len > CVT_BUF_MAX)
				goto out;
			*p++ = *s++;
		}
	}
      out:
	*p = 0;
	return len;
}

/********************************************************************** 
Function : 
	dump_msg_to_kes_mem
Description : 
	dump message to the kes mem
Input : 
	message buffer and size
Output : 
	none
Return :
	successfully print message length 
Others : 
	none
**********************************************************************/
int dump_msg_to_kes_mem(char *buff, int size )
{
	if(NULL == buff)
	{
		printk(KERN_INFO "kes input buffer pointer is NULL.\n");
		return 0;
	}
	
	if(size > (KES_TRAPS_BLOCK_SIZE - kes_traps_offset - KES_MEM_HEADER_LEN))
	{
		kes_traps_offset = 0;
		return 0;
	}
	
	memcpy((kes_traps_addr + kes_traps_offset + KES_MEM_HEADER_LEN), buff, size);
	kes_traps_offset = kes_traps_offset + size;

	return size;

}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36) 
extern void time_to_tm(time_t totalsecs, int offset, struct tm *result);
#endif
static void print_current_time(char * buf)
{
	struct timeval tv;
	struct tm tm;
	int len = 0;
	
	len = strlen(buf);
	do_gettimeofday(&tv);
	time_to_tm(tv.tv_sec, 480, &tm);
	if(buf[len - 1] == '\n')
	{
		sprintf(&buf[len - 1], "[%d-%d-%d %d:%d:%d]\n",tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
	else
	{
		return;
	}
}

/********************************************************************** 
Function : 
	print_msg_to_kes_mem
Description : 
	print message to the kes_mem
Input : 
	message pointer
Output : 
	none
Return :
	successfully print message length 
Others : 
	none
**********************************************************************/
int print_msg_to_kes_traps(const char * fmt,...)
{
	int msg_size = 0;
	int input_len = 0;
	static int is_first = 0;
	va_list args;
    static int traps_count = 0;
	char logbuf[CVT_BUF_MAX + 1] = {0};

	/*initialize the kes mem and fill the header*/
	if(!is_first)
	{
		memset(kes_traps_addr, 0, KES_TRAPS_BLOCK_SIZE);
		memcpy(kes_traps_header->magic, "autelan", KES_MAGIC_LEN);
		memcpy(kes_traps_header->isenable, "enable", KES_ISENABLE_LEN);
		is_first = 1;
	}

	#if 1 //liuqiang
	{
		strcpy(logbuf, fmt);
		print_current_time(logbuf);
		input_len = do_percentm(trapsbuf, logbuf);
	}
	#endif
       // input_len = do_percentm(trapsbuf, fmt);
    
      if(64 == traps_count)
      {
              traps_count = 0;
      }
      
	if(input_len > (KES_TRAPS_BLOCK_SIZE - KES_MEM_HEADER_LEN - kes_traps_offset- traps_count* 4*1024))
	{
	      traps_count ++;
	      kes_traps_offset = traps_count*4*1024;
             memcpy(kes_traps_print_addr + kes_traps_offset, "#*#*#BEGIN#*#*#",16);
             kes_traps_offset += 16;
	}
    	va_start(args, fmt);
    	msg_size = vsnprintf((char *)(kes_traps_print_addr + kes_traps_offset), KES_TRAPS_BLOCK_SIZE - KES_MEM_HEADER_LEN - kes_traps_offset, trapsbuf, args);
    	va_end(args);
    	
    	kes_traps_offset += msg_size;

    	return msg_size;
}

/********************************************************************** 
Function : 
	kes_mem_seq_start
Description : 
	set the start position of seq file .
Input : 
	seq_file pointer, position pointer
Output : 
	the seq file start position
Return :
	start position pointer
Others : 
	none
**********************************************************************/
int loff;
static void *kes_traps_start(struct seq_file *seq, loff_t *pos)
{
	unsigned char *start = (unsigned char *)kes_traps_addr + 0x10;
	loff=(*pos)*KES_MEM_SHOW_LEN;

	if(*pos >= traps_page_count)
	{
		*pos = 0;
		return NULL;
	}
	else
	{
		return (void *)(start+ (*pos) * KES_MEM_SHOW_LEN);
	}
}

/********************************************************************** 
Function : 
	kes_mem_seq_next
Description : 
	set the next position of seq file .
Input : 
	seq_file pointer,next pointer,  position pointer,
Output : 
	the seq file next position
Return :
	next position pointer
Others : 
	none
**********************************************************************/
static void *kes_traps_next(struct seq_file *seq, void *v, loff_t *pos)
{
	void *ptr_next = NULL;
	
	ptr_next = (void *)((unsigned char *)v + KES_MEM_SHOW_LEN);
	loff=(*pos)*KES_MEM_SHOW_LEN;

	*pos++;
	return ptr_next;
	
}

/********************************************************************** 
Function : 
	kes_mem_seq_show
Description : 
	show the seq file .
Input : 
	seq_file pointer, data pointer,
Output : 
   none
Return :
   0:success; (-1):failed
Others : 
	none
**********************************************************************/
static int kes_traps_show(struct seq_file *seq, void *v)
{
	int i = 0;
	void *pt = v;
	
	
	if(NULL == pt)
	{
		printk(KERN_INFO "kes_traps_mem show data pointer NULL.\n");
		return -1;
	}

	for(i = 0; i < KES_MEM_SHOW_LEN; i++)
	{  
           seq_printf(seq, "%c", *((unsigned char *)pt + i));
	}
	
	return 0;

} 

/********************************************************************** 
 Function : 
	 kes_mem_seq_stop
 Description : 
	 stop to show .
 Input : 
	 seq_file pointer, data pointer,
 Output : 
	none
 Return :
	void
 Others : 
	 none
 **********************************************************************/
static void  kes_traps_stop(struct seq_file *seq, void *v)
{
	return;
}



struct seq_operations kes_traps_seq_ops = {
	.start = kes_traps_start,
	.next  = kes_traps_next,
	.show  = kes_traps_show,
	.stop  = kes_traps_stop,
};

/********************************************************************** 
Function : 
	kes_proc_open
Description : 
	open the kes_mem proc dir entry .
Input : 
	inode, file pointer
Output : 
	void
Return :
	0: success; !(0): failed
Others : 
	none
**********************************************************************/
static int kes_proc_open(struct inode *inode, struct file *file)
{
	int retval = -1;

	if(NULL == file)
	{
		printk(KERN_INFO "kes file pointer is NULL.\n");
		return retval;
	}

	retval = seq_open(file, &kes_traps_seq_ops);
	if(retval)
	{
		printk(KERN_INFO "kes cannot open seq_file.\n");
		remove_proc_entry(KES_TRAPS_NAME, NULL);
	}

	return retval;
}

static ssize_t kes_flag_proc_write(struct file *flip, const char __user *buff, unsigned long len, void *data)
{
	if(len > KES_ISENABLE_LEN)
	{
		printk(KERN_INFO "kes flag buffer is full.\n");
		return -ENOSPC;
	}

	memset(kes_traps_header->isenable, 0, KES_ISENABLE_LEN);

	if(copy_from_user(kes_traps_header->isenable, buff, len))
	{
		printk(KERN_INFO "kes flag copy_from_user error.\n");
		return -EFAULT;
	}

	return len;
	
}

static int kes_flag_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	unsigned char tmp[8] = {0};
	int len = 0;

	if(off > 0)
	{
		*eof = 1;
		return 0;
	}

	memset(page, 0, PAGE_SIZE);

	memcpy(tmp, kes_traps_header->isenable, KES_ISENABLE_LEN);
	len = sprintf(page, "%s", tmp);

	*eof = 1;
	
	return len;
	
}

struct file_operations kes_traps_ops = {
	.owner   = THIS_MODULE,
	.open    = kes_proc_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_private,
};
/*-----------------End of the traps section ----------------------*/


/*-----------------Begin of the dmsg section ----------------------*/
static char dmsgbuf [CVT_BUF_MAX + 1] = {0};
unsigned char *kes_dmsg_addr = NULL;
unsigned char *kes_dmsg_print_addr = NULL;
kes_mem_header_type *kes_dmsg_header = NULL;
unsigned int kes_dmsg_offset = 0;
#define  dmsg_page_count   (KES_DMSG_BLOCK_SIZE/KES_MEM_SHOW_LEN)
unsigned char kes_dmsg_switch[2] = {'0',0};

static ssize_t kes_dmsg_switch_proc_write(struct file *flip, const char __user *buff, unsigned long len, void *data)
{
        if(len > 2) //just a character and a '\0' is enough
        {
             //   printk(KERN_INFO "kes_dmsg_switch buffer is full.\n");
                return -ENOSPC;
        }

        if(copy_from_user(kes_dmsg_switch, buff, 2))
        {
           //     printk(KERN_INFO "kes_dmsg_switch copy_from_user error.\n");
                return -EFAULT;
        }

        if(kes_dmsg_switch[0] == '0')
        {
                memset(kes_dmsg_addr, 0, KES_DMSG_BLOCK_SIZE);
        }
        kes_dmsg_switch[0] = '1';

        return len;
}

static int kes_dmsg_switch_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
        unsigned char tmp[8] = {0};
        int len = 0;

        if(off > 0)
        {
                *eof = 1;
                return 0;
        }

        memset(page, 0, PAGE_SIZE);

        memcpy(tmp, kes_dmsg_switch, 1);
        len = sprintf(page, "%s", tmp);

        *eof = 1;

        return len;
}

/*caizhibang add */


int print_msg_to_kes_dmsg(char *buff, int size )
{
      static int is_first_dmsg = 0;
	char logbuf[CVT_BUF_MAX + 1] = {0};
    
	if(NULL == buff)
	{
	//	printk(KERN_INFO "kes dmsg buffer pointer is NULL.\n");
		return 0;
	}
    #if 0
	strcpy(logbuf, buff);
	print_current_time(logbuf);
	size = strlen(logbuf);
    #endif
      if('1' == kes_dmsg_switch[0])
      {
             if(!is_first_dmsg)
    	      {
        		memset(kes_dmsg_addr, 0, KES_DMSG_BLOCK_SIZE);
        		memcpy(kes_dmsg_header->magic, "autelan", KES_MAGIC_LEN);
        		memcpy(kes_dmsg_header->isenable, "enable", KES_ISENABLE_LEN);
        		is_first_dmsg = 1;
	      }
            	if(size > (KES_DMSG_BLOCK_SIZE - kes_dmsg_offset - KES_MEM_HEADER_LEN))
            	{
            	      kes_dmsg_offset = 0;
                   memset(kes_dmsg_addr, 0 , KES_DMSG_BLOCK_SIZE);
            		return 0;
            	}
            	memcpy((kes_dmsg_addr + kes_dmsg_offset + KES_MEM_HEADER_LEN),buff, size);
            	kes_dmsg_offset += size;
       }
	return size;
}


int loff;
static void *kes_dmsg_start(struct seq_file *seq, loff_t *pos)
{
	unsigned char *start = (unsigned char *)kes_dmsg_addr+ 0x10;
	loff=(*pos)*KES_MEM_SHOW_LEN;

	if(*pos >= dmsg_page_count)
	{
		*pos = 0;
      //       memset(kes_dmsg_addr, 0, KES_DMSG_BLOCK_SIZE);
		return NULL;
	}
	else
	{
		return (void *)(start+ (*pos) * KES_MEM_SHOW_LEN);
	}
}

/********************************************************************** 
Function : 
	kes_mem_seq_next
Description : 
	set the next position of seq file .
Input : 
	seq_file pointer,next pointer,  position pointer,
Output : 
	the seq file next position
Return :
	next position pointer
Others : 
	none
**********************************************************************/
static void *kes_dmsg_next(struct seq_file *seq, void *v, loff_t *pos)
{
	void *ptr_next = NULL;
	
	ptr_next = (void *)((unsigned char *)v + KES_MEM_SHOW_LEN);
	loff=(*pos)*KES_MEM_SHOW_LEN;

	*pos++;
	return ptr_next;
	
}

/********************************************************************** 
Function : 
	kes_mem_seq_show
Description : 
	show the seq file .
Input : 
	seq_file pointer, data pointer,
Output : 
   none
Return :
   0:success; (-1):failed
Others : 
	none
**********************************************************************/
static int kes_dmsg_show(struct seq_file *seq, void *v)
{
	int i = 0;
	void *pt = v;

	if(NULL == pt)
	{
	//	printk(KERN_INFO "kes_mem show data pointer NULL.\n");
     //        printk("kes_mem show data pointer NULL.\n");
		return -1;
	}

	for(i = 0; i < KES_MEM_SHOW_LEN; i++)
	{ 
            if(((unsigned char *)pt + i) >= (kes_dmsg_addr+0x100000) || ((unsigned char *)pt + i) < kes_dmsg_addr)
                  break;
	      seq_printf(seq, "%c", *((unsigned char *)pt + i));
	}

	return 0;

} 

/********************************************************************** 
 Function : 
	 kes_mem_seq_stop
 Description : 
	 stop to show .
 Input : 
	 seq_file pointer, data pointer,
 Output : 
	none
 Return :
	void
 Others : 
	 none
 **********************************************************************/
static void  kes_dmsg_stop(struct seq_file *seq, void *v)
{
	return;
}



struct seq_operations kes_dmsg_seq_ops = {
	.start = kes_dmsg_start,
	.next  = kes_dmsg_next,
	.show  = kes_dmsg_show,
	.stop  = kes_dmsg_stop,
};



static int kes_dmsg_proc_open(struct inode *inode, struct file *file)
{
	int retval = -1;

	if(NULL == file)
	{
		printk(KERN_INFO "kes file pointer is NULL.\n");
		return retval;
	}

	retval = seq_open(file, &kes_dmsg_seq_ops);
	if(retval)
	{
		printk(KERN_INFO "kes cannot open seq_file.\n");
		remove_proc_entry(KES_DMSG_NAME, NULL);
	}

	return retval;
}

struct file_operations kes_dmsg_ops = {
	.owner   = THIS_MODULE,
	.open    = kes_dmsg_proc_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_private,
};

#if 0
void string_function()
{
  //   char s = 'a';
     unsigned int i = 0 ;
     for(i=0;i < 1024*1024;i++)
     {
    //      print_msg_to_kes_dmsg(&s, 1);
       print_msg_to_kes_traps("a");
     }
}
#endif
/*-----------------End of the dmsg section ----------------------*/

/*-----------------Begin of the debug section ----------------------*/

/*<Begin:caizhibang add for kes 2013-02-19 */
static char debugbuf [CVT_BUF_MAX + 1] = {0};
unsigned char *kes_debug_addr = NULL;
unsigned char *kes_debug_print_addr = NULL;
kes_mem_header_type *kes_debug_header = NULL;
unsigned int kes_debug_offset = 0;
#define debug_page_count   (KES_DEBUG_BLOCK_SIZE/KES_MEM_SHOW_LEN)
unsigned char kes_debug_switch[2] = {'0',0};
unsigned char kes_debug_flag[2] = {'0',0};

static ssize_t kes_debug_switch_proc_write(struct file *flip, const char __user *buff, unsigned long len, void *data)
{
        if(len > 2) //just a character and a '\0' is enough
        {
                printk(KERN_INFO "kes_debug_switch buffer is full.\n");
                return -ENOSPC;
        }

        if(copy_from_user(kes_debug_switch, buff, 2))
        {
                printk(KERN_INFO "kes_debug_switch copy_from_user error.\n");
                return -EFAULT;
        }

        if(kes_debug_switch[0] == '0')
        {
                memset(kes_debug_addr, 0, KES_DEBUG_BLOCK_SIZE);
        }
        kes_debug_switch[0] = '1';

        return len;
}

static int kes_debug_switch_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
        unsigned char tmp[8] = {0};
        int len = 0;

        if(off > 0)
        {
                *eof = 1;
                return 0;
        }

        memset(page, 0, PAGE_SIZE);

        memcpy(tmp, kes_debug_switch, 1);
        len = sprintf(page, "%s", tmp);

        *eof = 1;

        return len;
}

////////////////////////////////
static ssize_t kes_debug_flag_proc_write(struct file *flip, const char __user *buff, unsigned long len, void *data)
{
        if(len > 2) //just a character and a '\0' is enough
        {
                printk(KERN_INFO "kes_debug_flag buffer is full.\n");
                return -ENOSPC;
        }

        if(copy_from_user(kes_debug_flag, buff, 2))
        {
                printk(KERN_INFO "kes_debug_switch copy_from_user error.\n");
                return -EFAULT;
        }
       #if 0
        switch(kes_debug_flag[0])
        {
               case 'A':
                       memcpy(&kes_debug_header->isenable[6],"A",1);
                       break;
               case 'K':
                       memcpy(&kes_debug_header->isenable[6],"K",1);
                       break;
               case 'W':
                       memcpy(&kes_debug_header->isenable[6],"W",1);
                       break;
               case 'D':
                       memcpy(&kes_debug_header->isenable[6],"D",1);
                       break;
               default:
                       break;
        }
        #endif
        #if  1
  //      if(kes_debug_flag[0] == 'W')
        {
                memcpy(&kes_debug_header->isenable[6],kes_debug_flag,1);
        }
//        else
//        {
 //             kes_debug_flag[0] = '0';
 //       }
         #endif
        return len;
}

static int kes_debug_flag_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
        unsigned char tmp[2] = {0};
        int len = 0;

        if(off > 0)
        {
                *eof = 1;
                return 0;
        }

        memset(page, 0, PAGE_SIZE);
        
        kes_debug_flag[0] = kes_debug_header->isenable[6];
        memcpy(tmp, kes_debug_flag, 1);
        len = sprintf(page, "%s", tmp);

        *eof = 1;

        return len;
}

////////////////////////////////

int print_msg_to_kes_debug(const char * fmt,...)
{
	int msg_size = 0;
	int input_len = 0;
	char logbuf[CVT_BUF_MAX + 1] = {0};
	static int is_first_debug = 0;
	va_list args;
    
      if('1' == kes_debug_switch[0])
      {
        	/*initialize the kes debug mem and fill the header*/
          
        	if(!is_first_debug)
        	{
        		memset(kes_debug_addr, 0, KES_DEBUG_BLOCK_SIZE);
        		memcpy(kes_debug_header->magic, "autelan", KES_MAGIC_LEN);
        		memcpy(kes_debug_header->isenable, "enable0", KES_ISENABLE_LEN);
        		is_first_debug = 1;
        	}
            
        	#if 1 //liuqiang
        	{
				strcpy(logbuf, fmt);
				print_current_time(logbuf);
        	
        	}
        	
        	input_len = do_percentm(debugbuf, logbuf);
        	#endif
        	if(input_len > (KES_DEBUG_BLOCK_SIZE - KES_MEM_HEADER_LEN - kes_debug_offset))
        	{
        	      memset(kes_debug_print_addr,0,KES_DEBUG_BLOCK_SIZE);
        		kes_debug_offset = 0;
                   return 0;
        	}
            	va_start(args, fmt);
            	msg_size = vsnprintf((char *)(kes_debug_print_addr + kes_debug_offset), (KES_DEBUG_BLOCK_SIZE - KES_MEM_HEADER_LEN - kes_debug_offset), debugbuf, args);
            	va_end(args);
            	 
            	kes_debug_offset += msg_size;
     }

	return msg_size;
}
void print_flag_to_kes_debug(const char *s )
{
      if(s == NULL)
            return;
      memcpy(&kes_debug_header->isenable[6],s,1);
}
int loff;
static void *kes_debug_start(struct seq_file *seq, loff_t *pos)
{
	unsigned char *start = (unsigned char *)kes_debug_addr+ 0x10;
	loff=(*pos)*KES_MEM_SHOW_LEN;
	if(*pos >= debug_page_count)
	{
		*pos = 0;
          //   memset(kes_debug_addr, 0, KES_DEBUG_BLOCK_SIZE);
		return NULL;
	}
	else
	{
		return (void *)(start+ (*pos) * KES_MEM_SHOW_LEN);
	}
}

static void *kes_debug_next(struct seq_file *seq, void *v, loff_t *pos)
{
	void *ptr_next = NULL;
	
	ptr_next = (void *)((unsigned char *)v + KES_MEM_SHOW_LEN);
	loff=(*pos)*KES_MEM_SHOW_LEN;

	*pos++;
	return ptr_next;
	
}

static int kes_debug_show(struct seq_file *seq, void *v)
{
	int i = 0;
	void *pt = v;

//    unsigned char *pt =(unsigned char *)v;
//	if(loff+i > kes_mem_offset)
 //    return 0;
	if(NULL == pt)
	{
		printk(KERN_INFO "kes_debug_mem show data pointer NULL.\n");
		return -1;
	}

	for(i = 0; i < KES_MEM_SHOW_LEN; i++)
	{  
            seq_printf(seq, "%c", *((unsigned char *)pt + i));
	}

	return 0;

} 

static void  kes_debug_stop(struct seq_file *seq, void *v)
{
	return;
}



struct seq_operations kes_debug_seq_ops = {
	.start = kes_debug_start,
	.next  = kes_debug_next,
	.show  = kes_debug_show,
	.stop  = kes_debug_stop,
};

static int kes_debug_proc_open(struct inode *inode, struct file *file)
{
	int retval = -1;

	if(NULL == file)
	{
		printk(KERN_INFO "kes file pointer is NULL.\n");
		return retval;
	}

	retval = seq_open(file, &kes_debug_seq_ops);
	if(retval)
	{
		printk(KERN_INFO "kes cannot open seq_file.\n");
		remove_proc_entry(KES_DEBUG_NAME, NULL);
	}

	return retval;
}

static ssize_t kes_debug_write (struct file * filp, const char __user * buf, size_t count, loff_t * f_pos)
{
	char msg[CVT_BUF_MAX+1] = {0};
	ssize_t msg_size = 0;
	
	if(count > CVT_BUF_MAX+1)
	{
		return -EFAULT;
	}
	if(copy_from_user(msg, buf, count))
	{
		return -EFAULT;
	}
	
	msg_size = print_msg_to_kes_debug(msg);
	return msg_size;
}


struct file_operations kes_debug_ops = {
	.owner   = THIS_MODULE,
	.open    = kes_debug_proc_open,
	.read    = seq_read,
	.write    = kes_debug_write,
	.llseek  = seq_lseek,
	.release = seq_release_private,
};
/*End:caizhibang add for kes 2013-02-19> */
/*-----------------End of the debug section ----------------------*/

/********************************************************************** 
Function : 
	kes_mem_addr_get
Description : 
	get kes mem the uboot alloced
Input : 
	void
Output : 
	kes_mem_addr
Return :
	kes_mem pointer
Others : 
	none
**********************************************************************/
static void *kes_mem_addr_get(void)
{   
       unsigned int pfn;
	 struct page *page;
	 void *vtl_addr;
     #if  1
	 pfn=TRAPS_START_ADDR >> PAGE_SHIFT;
	 page=pfn_to_page(pfn);
	 if(page == NULL)
	 {
	        printk("get the wrong page \n");
		 return NULL;
	 }
	 vtl_addr = page_to_virt(page);
	 kes_traps_addr = vtl_addr;
       printk("kes_traps_addr=%p,%s,%d\n",kes_traps_addr,__func__,__LINE__);
     #endif
       pfn = (TRAPS_START_ADDR + KES_TRAPS_BLOCK_SIZE) >> PAGE_SHIFT;
       page = pfn_to_page(pfn);
	 if(page == NULL)
	 {
	      printk("get the wrong page \n");
		return NULL;
	 }
	 vtl_addr=page_to_virt(page);
	 kes_debug_addr=vtl_addr;
       printk("kes_debug_addr=%p,%s,%d\n",kes_debug_addr,__func__,__LINE__);
     
       pfn =((MEM_SIZE+1)<<20) >> PAGE_SHIFT;
	 page=pfn_to_page(pfn);
	 if(page == NULL)
	 {
		printk("get the wrong page \n");
		return NULL;
	 }
	 vtl_addr=page_to_virt(page);
      kes_dmsg_addr = vtl_addr;
      printk("kes_dmsg_addr=%p,%s,%d\n",kes_dmsg_addr,__func__,__LINE__);
     /*End : zhaoej add 2013-01-06>*/   
	 return kes_dmsg_addr;

}


/********************************************************************** 
Function : 
	kes_proc_init
Description : 
	create the kes proc dir entry .
Input : 
	void
Output : 
	void
Return :
	0: success; (-1): failed
Others : 
	none
**********************************************************************/
static int kes_proc_init(void)
{
	int retval = -1;
	struct proc_dir_entry *kes_traps_entry = NULL;
	struct proc_dir_entry *kes_flag_entry = NULL;
      struct proc_dir_entry *kes_dmsg_entry  = NULL;
      struct proc_dir_entry *kes_debug_entry = NULL;
    
      struct proc_dir_entry *kes_dmsg_swtich_entry  = NULL;
      kes_dmsg_swtich_entry = create_proc_entry(KES_DMSG_SWITCH_NAME, 0666, NULL);
      if(kes_dmsg_swtich_entry)
     {
            kes_dmsg_swtich_entry->read_proc = kes_dmsg_switch_proc_read;
            kes_dmsg_swtich_entry->write_proc = kes_dmsg_switch_proc_write;
     }
     else
     {
            printk(KERN_INFO "kes create %s error.\n", KES_DMSG_SWITCH_NAME);
            remove_proc_entry(KES_DMSG_SWITCH_NAME, NULL);
            retval = -1;
     }

     struct proc_dir_entry *kes_debug_swtich_entry  = NULL;
     kes_debug_swtich_entry = create_proc_entry(KES_DEBUG_SWITCH_NAME, 0666, NULL);
     if(kes_debug_swtich_entry)
     {
            kes_debug_swtich_entry->read_proc = kes_debug_switch_proc_read;
            kes_debug_swtich_entry->write_proc = kes_debug_switch_proc_write;
     }
     else
     {
            printk(KERN_INFO "kes create %s error.\n", KES_DEBUG_SWITCH_NAME);
            remove_proc_entry(KES_DEBUG_SWITCH_NAME, NULL);
            retval = -1;
     }

     struct proc_dir_entry *kes_debug_flag_entry  = NULL;
     kes_debug_flag_entry = create_proc_entry(KES_DEBUG_FLAG_NAME, 0666, NULL);
     if(kes_debug_flag_entry)
     {
            kes_debug_flag_entry->read_proc = kes_debug_flag_proc_read;
            kes_debug_flag_entry->write_proc = kes_debug_flag_proc_write;

   }

   else

   {

          printk(KERN_INFO "kes create %s error.\n", KES_DEBUG_FLAG_NAME);

          remove_proc_entry(KES_DEBUG_FLAG_NAME, NULL);

          retval = -1;

   }

     /*caizhibang add */

    #if 1

	kes_traps_entry = create_proc_entry(KES_TRAPS_NAME, 0, NULL);

	if(kes_traps_entry)

	{

		kes_traps_entry->proc_fops = &kes_traps_ops;

		retval = 0;

	}

	else

	{

		printk(KERN_INFO "kes create %s error.\n", KES_TRAPS_NAME);

		retval = -1;

	}


	kes_flag_entry = create_proc_entry(KES_FLAG_NAME, 0666, NULL);

	if(kes_flag_entry)

	{

		kes_flag_entry->read_proc = kes_flag_proc_read;

		kes_flag_entry->write_proc = kes_flag_proc_write;

		//kes_flag_entry->owner = THIS_MODULE;

	}

	else

	{

		printk(KERN_INFO "kes create %s error.\n", KES_FLAG_NAME);

		remove_proc_entry(KES_PROC_NAME, NULL);

		retval = -1;

	}

    #endif

    /*<Begin:zhaoej add 2013-01-06 */

      kes_dmsg_entry = create_proc_entry(KES_DMSG_NAME, 0, NULL);

	if(kes_dmsg_entry)

	{

		kes_dmsg_entry->proc_fops = &kes_dmsg_ops;

		retval = 0;

	}

	else

	{

		printk(KERN_INFO "kes create %s error.\n", KES_DMSG_NAME);

		retval = -1;

	}


      kes_debug_entry = create_proc_entry(KES_DEBUG_NAME, 0666, NULL);

	if(kes_debug_entry)

	{

		kes_debug_entry->proc_fops = &kes_debug_ops;

		retval = 0;

	}

	else

	{

		printk(KERN_INFO "kes create %s error.\n", KES_DEBUG_NAME);

		retval = -1;

	}

    /*End:zhaoej add 2013-01-06 >*/


	return retval;

	

}

extern char *saved_command_line;
static int __init kes_init(void)

{
      char * pargs = NULL;
	char * pmem = NULL;
	int mem = 0, pos = 0;
	pargs = strstr(saved_command_line, "ramdisk_size");
      if(pargs == NULL)
      {
          return -1;
      }
	pmem = strstr(pargs,"=");
      if(pmem == NULL)
      {
          return -1;
      }
	pmem++;
	
	while(((pos =(*pmem - 0x30))>= 0) && (pos <= 9))
	{
		mem *=10;
		mem += pos;
             pmem++;
	}
    
      printk("kes_dmsg_switch = %c,memory_size = %dM, phy_mem = %dM\n",kes_dmsg_switch[0],MEM_SIZE,mem);
      if(mem - MEM_SIZE == 0)
      {
           return -1;
      }
    
	if( NULL == kes_mem_addr_get() )

	{

		return -1;

	}

     #if 1

	kes_traps_header = (kes_mem_header_type *)kes_traps_addr;

	kes_traps_print_addr = kes_traps_addr + KES_MEM_HEADER_LEN;

      kes_traps_print_handle = print_msg_to_kes_traps;

      kes_mem_dump_handle = dump_msg_to_kes_mem;

	print_current_time_handle = print_current_time;

    #endif 

    /*<Begin:zhaoej add 2013-01-06 */

     kes_dmsg_header = (kes_mem_header_type *)kes_dmsg_addr;

     kes_dmsg_print_addr = kes_dmsg_addr + KES_MEM_HEADER_LEN;

     kes_dmsg_print_handle = print_msg_to_kes_dmsg;

    // string_test_handle = string_function;


     kes_debug_header = (kes_mem_header_type *)kes_debug_addr;

     kes_debug_print_addr = kes_debug_addr + KES_MEM_HEADER_LEN;

     kes_debug_print_handle = print_msg_to_kes_debug;

     kes_debug_print_flag_handle = print_flag_to_kes_debug;

    /*End:zhaoej add 2013-01-06 >*/

	if(kes_proc_init())

	{

		return -1;

	}


	printk(KERN_INFO "kes module loaded.\n");


	return 0;


}


static void __exit kes_exit(void)

{

	kes_traps_print_handle = NULL;

	kes_mem_dump_handle = NULL;

	print_current_time_handle = NULL;

    /*<Begin:zhaoej add 2013-01-06 */

	kes_dmsg_print_handle = NULL;

      kes_debug_print_handle = NULL;

    remove_proc_entry(KES_DMSG_NAME, NULL);

    /*End:zhaoej add 2013-01-06 >*/

    #if 1

      remove_proc_entry(KES_DEBUG_NAME, NULL);

	remove_proc_entry(KES_TRAPS_NAME, NULL);

	remove_proc_entry(KES_FLAG_NAME, NULL);

      remove_proc_entry(KES_DMSG_SWITCH_NAME, NULL);

       remove_proc_entry(KES_DEBUG_SWITCH_NAME, NULL);

       remove_proc_entry(KES_DEBUG_FLAG_NAME, NULL);

     #endif

	printk(KERN_INFO "kes module unloaded.\n");


}



module_init(kes_init);

module_exit(kes_exit);

