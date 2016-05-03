/*
 *
 * Copyright 2010 Autelan Software Engineering, zhengkun  <zhengkun@autelan.com>
 *
 * LED , tempreture,and interrupt handlers for Power button 
 *
 */

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/i2c.h>
#include <linux/of_i2c.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/of_platform.h>
#include <asm/io.h>
#include <asm/dcr-native.h>
#include <asm/dcr-regs.h>
#include <asm/machdep.h>

//#include <linux/hlds_ctrl.h>
#include <linux/reboot.h>
#include <linux/rtc.h>
#include <linux/workqueue.h>
#include "led_ctrl.h"
#include "watchdog.h"
#include "interrupt_led.h"
#include "mcp23008.h"

/* uncomment if need to debug */
#define HLDS_CTRL_DEBUG
#define HLDS_NAME "Pro_ctl_mod driver: "
#if defined(HLDS_CTRL_DEBUG)
#define hlds_debug(fmt, args...)      printk(HLDS_NAME fmt, ##args)
#else
#define hlds_debug(fmt, args...)      do {} while (0)
#endif

#define DEV_INIT_TRUE  1
#define DEV_INIT_FALSE 0 

#define SA_INTERRUPT 0

static DEFINE_SPINLOCK(hlds_ctrl_lock);

struct led_action
{
        struct timer_list led_timer;
        int               led_state;
        int               led_device;
        int               led_rate;
        int               led_ctrl;
        int               led_status;
        int               count; /* -1: forever, 0: toggle, otherwise number of times*/
};
//chenxf add for upgrading boot through linux
//chenxf add for pin function control0 configuration
#define SDR0_PFC0		0x4100
#define mfdcr(rn) \
	({	\
		unsigned long rval; \
		asm volatile("mfdcr %0,%1" : "=r"(rval) : "i"(rn)); \
		rval; \
	})
#define mtdcr(rn, val) \
	asm volatile("mtdcr %0,%1" : : "i"(rn), "r"(val))

/* SDRs (440GX, 440SP, 440SPe 460EX and 460GT) */
#define DCRN_SDR0_CONFIG_ADDR 	0xe
#define DCRN_SDR0_CONFIG_DATA	0xf

/*Begin: Added by wangjia, instead of using macro APM82181_WBS */
int is_wbs = 0;

/*
 * Used for getting product type, eg. PC-AF4000 or PC-APV6.
 *
 * Referenced FUNCTION get_boardconfig_app in module pro_ctl_util.
 * NOTICE:In kernel mode, file operations are different from in user mode.
 */
#include <linux/fs.h>
#include <asm/uaccess.h>

#define AP_V6_AF4000            1
#define AP_V6_APV6              0

unsigned char get_product_type()
{
    int  retval = 0, i = 0;
    struct file *filp = NULL;
    
#define PRO_OFFSET 0
#define PRO_BUF    19
#define BUF_LEN (PRO_OFFSET + PRO_BUF + 1)
    char buff[BUF_LEN];
    
    char *apv6_pro_type   = "PC-APV6";
    int  apv6_pro_type_len = 7;
    char *af4000_pro_type = "PC-AF4000";
    int  af4000_pro_type_len = 9;

    memset(buff, 0, sizeof(buff));

    filp = filp_open("/dev/mtdblock3", O_RDONLY, 0);
    
    if(IS_ERR(filp)) 
    {
        printk("pro_ctl_mod, failed to get product type.\r\n");  
        /**
         * If error occured when we determine the product type, 
         * we just think it's a WBS.
         */
        return AP_V6_AF4000;  
    }
    
    mm_segment_t orgfs; 

    if (filp->f_op && filp->f_op->read)  
    {  
        orgfs = get_fs();  
        set_fs(KERNEL_DS);
        retval = filp->f_op->read(filp, buff, BUF_LEN, &filp->f_pos); 
        set_fs(orgfs); 
        if (retval < 0)  
        {   
            filp_close(filp, NULL);
            printk("pro_ctl_mod, failed to get product type.\r\n"); 
            /* The same as the situation when openning file error. */
            return AP_V6_AF4000;  
        }  
    }      

    filp_close(filp, NULL);
    
    if(memcmp(buff + PRO_OFFSET, af4000_pro_type, af4000_pro_type_len) == 0)
    {
        printk("IS.... AP_V6_AF4000\r\n");
        return AP_V6_AF4000;
    }
    else if(memcmp(buff + PRO_OFFSET, apv6_pro_type, apv6_pro_type_len) == 0)
    {
        printk("IS.... AP_V6_APV6\r\n");
        return AP_V6_APV6;
    }
    
    return AP_V6_AF4000;
}
/*End: Added by wangjia, instead of using macro APM82181_WBS. */

#define SDR0_READ(offset) ({\
	mtdcr(DCRN_SDR0_CONFIG_ADDR, offset); \
	mfdcr(DCRN_SDR0_CONFIG_DATA); })
#define SDR0_WRITE(offset, data) ({\
	mtdcr(DCRN_SDR0_CONFIG_ADDR, offset); \
	mtdcr(DCRN_SDR0_CONFIG_DATA, data); })
//chenxf add end
static struct led_action hlds_led_action[HLDS_LED_IDX_MAX];
static int device_init = DEV_INIT_TRUE;
void hlds_led_cmd_hdlr(struct hlds_ctrl_t *cmd)
{
        struct led_action *action;
        int gpio_output = 0;
        action = &hlds_led_action[cmd->device];
//chenxf add for upgrading boot through linux
	unsigned int sdr0_pfc0 = 0x125;
//chenxf add end 
        switch(cmd->ctrl) {
        case 0:
             if(cmd->device == 1) 
			   close_2G_led();
		     else if(cmd->device == 2)
			   close_5G_led(); 
			 else if(cmd->device == 3)
			   close_eth_led();
			 else if(cmd->device == 4)
			   stop_beeping();
//chenxf add for upgrading boot through linux
	     else if(cmd->device == 125){
		sdr0_pfc0 = SDR0_READ(SDR0_PFC0);
		sdr0_pfc0 |= (1 << (31 - 0));
		sdr0_pfc0 |= (1 << (31 - 3));
		sdr0_pfc0 |= (1 << (31 - 9));
		sdr0_pfc0 |= (1 << (31 - 10));
		sdr0_pfc0 |= (1 << (31 - 11));
		printk("recover sdr0_pfc0 = 0x%x\n",sdr0_pfc0);
		SDR0_WRITE(SDR0_PFC0, sdr0_pfc0); 
	     }
//chenxf add end 
		break;
        case 1:
             if(cmd->device == 1) 
			   open_2G_led();
		     else if(cmd->device == 2)
			   open_5G_led(); 
			 else if(cmd->device == 3)
			   open_eth_led();
			 else if(cmd->device == 4)
			   start_beeping();
//chenxf add for upgrading boot through linux
	     else if(cmd->device == 125){
		sdr0_pfc0 = SDR0_READ(SDR0_PFC0);
		sdr0_pfc0 &= ~(1 << (31 - 0));
		sdr0_pfc0 &= ~(1 << (31 - 3));
		sdr0_pfc0 &= ~(1 << (31 - 9));
		sdr0_pfc0 &= ~(1 << (31 - 10));
		sdr0_pfc0 &= ~(1 << (31 - 11));
		printk("set sdr0_pfc0 = 0x%x\n",sdr0_pfc0);
		SDR0_WRITE(SDR0_PFC0, sdr0_pfc0);
	     }
//chenxf add end 
                break;
		case 2: cmd->reset=reset_irq_num;
			    break;
		case 3: stop_watchdog();
				break;
       default:
                break;
        }
}

static long hlds_ctrl_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
        struct hlds_ctrl_t hlds_ctrl;
        if (copy_from_user(&hlds_ctrl, (void *) arg, sizeof(struct hlds_ctrl_t))) {
            return -EFAULT;
        }
       switch(cmd) {
        case WRCPU_IOC_V6_:
                hlds_led_cmd_hdlr(&hlds_ctrl);
                break;
        default:
                return -EINVAL;
        }
	    copy_to_user((void *) arg, &hlds_ctrl, sizeof(struct hlds_ctrl_t));
        return 0;
}

static int hlds_ctrl_open(struct inode *inode, struct file *file)
{
        return nonseekable_open(inode, file);
}

static ssize_t hlds_ctrl_read(struct file *file, char __user * buffer, 
                                size_t count, loff_t *pos)
{  return 0;
}

static const struct file_operations hlds_ctrl_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.unlocked_ioctl = hlds_ctrl_ioctl,
	.open  = hlds_ctrl_open,
	.read  = hlds_ctrl_read,
};

static struct miscdevice hlds_ctrl_dev = {
	.minor = 131,
	.name  = HLDS_DEV_NAME,
	.fops  = &hlds_ctrl_fops,
};

unsigned int reset_irq_num = 0;

static int __init hlds_ctrl_init(void)
{       struct device_node *np, *pwr_node, *bkup_node;
        int ret = 0;
        int i;
        int err;
		int fd;
	enum of_gpio_flags flags;

        ret = -ENODEV;
        for_each_compatible_node(np, NULL, "ibm,ppc4xx-gpio") {
                ret = 0;
        }

        if (ret != 0) return ret;

        is_wbs = get_product_type();
        printk("IS WBS - %d...\r\n", is_wbs);
        
        device_init = DEV_INIT_TRUE;
        /* Init our data structures first */

        for (i=0; i < HLDS_LED_IDX_MAX; i++) {
                hlds_led_action[i].led_state = 0; /* Will read from gpio later */
                hlds_led_action[i].led_device = i;
                hlds_led_action[i].led_status = LED_BLINK_ENABLE;
        }


        /* Register driver with kernel */
        ret = misc_register(&hlds_ctrl_dev);
        if (ret) {
                printk(KERN_ERR "Unable to register pro_ctl_mod driver\n");
                return ret;
        }

		i2c_add_driver(&mcp23008_driver);

	 	printk("pro_ctl_mod driver initialization complete \n");
	   	start_watchdog();
        turnon_eth_led=open_eth_led;
        turnoff_eth_led=close_eth_led;
		stop_ap2400_watchdog=stop_watchdog;
        return 0;	
}

static void __exit hlds_ctrl_exit(void)
{
		i2c_del_driver(&mcp23008_driver);

		misc_deregister(&hlds_ctrl_dev);
}

module_init(hlds_ctrl_init);
module_exit(hlds_ctrl_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhengkun<zhengkun@autelan.com>");
MODULE_DESCRIPTION("HLDS NAS LED and buttons control driver");
