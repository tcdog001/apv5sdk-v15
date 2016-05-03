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
//#include <linux/types.h>
//#include <linux/errno.h>
//#include <linux/proc_fs.h>
//#include <linux/poll.h>
//#include <linux/fs.h>
//#include <linux/gpio.h>
//#include <linux/of_gpio.h>
//#include <linux/delay.h>
//#include <linux/miscdevice.h>
//#include <linux/uaccess.h>
//#include <linux/interrupt.h>
//#include <linux/of_platform.h>
//#include <asm/io.h>

//#include <linux/reboot.h>
//#include <linux/rtc.h>
//#include <linux/workqueue.h>
#include "led_ctrl.h"
#include "watchdog.h"
#include "interrupt_led.h"


#if 0
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
static struct led_action hlds_led_action[HLDS_LED_IDX_MAX];
static int device_init = DEV_INIT_TRUE;
void hlds_led_cmd_hdlr(struct hlds_ctrl_t *cmd)
{
        struct led_action *action;
        int gpio_output = 0;
        action = &hlds_led_action[cmd->device];

        switch(cmd->ctrl) {
        case 0:
			//close_led(cmd->device);
			break;
        case 1:
			//open_led(cmd->device);
			break;
		case 2: 
			//cmd->stats = gpio_read_bit(cmd->device);
			//printk("the GPIO %d stats is %d\n",cmd->device,cmd->stats);
			break;
		case 3:
			//cmd->reset=reset_irq_num;
			break;
		case 4: stop_watchdog();
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
#endif

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
//	.unlocked_ioctl = hlds_ctrl_ioctl,
	.open  = hlds_ctrl_open,
	.read  = hlds_ctrl_read,
};

static struct miscdevice hlds_ctrl_dev = {
	.minor = 131,
	.name  = "wrcpu",
	.fops  = &hlds_ctrl_fops,
};

unsigned int reset_irq_num = 0;

static int __init hlds_ctrl_init(void)
{       
    int ret = 0;
    /* Init our data structures first */

    /* Register driver with kernel */
    ret = misc_register(&hlds_ctrl_dev);
    if (ret) {
        printk(KERN_ERR "Unable to register pro_ctl_mod driver\n");
        return ret;
    }
    start_watchdog();

    apv7_gpio_set_value = mv_gpio_set_value;
    apv7_stop_watchdog = stop_watchdog;

    printk("pro_ctl_mod driver initialization complete \n");

    return 0;	
}

static void __exit hlds_ctrl_exit(void)
{
    stop_watchdog();
    misc_deregister(&hlds_ctrl_dev);
}

module_init(hlds_ctrl_init);
module_exit(hlds_ctrl_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhengkun<zhengkun@autelan.com>");
MODULE_DESCRIPTION("HLDS NAS LED and buttons control driver");

