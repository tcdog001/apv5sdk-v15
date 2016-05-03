//zhengkun add for watchdog 
#include <linux/smp.h>
#include <linux/notifier.h>
#include <linux/watchdog.h>
#include <linux/uaccess.h>

#include <asm/div64.h>
//#include "interrupt_led.h"

#if 1
#define WDT_PERIOD_DEFAULT 3
u32 booke_wdt_enabled;
u32 booke_wdt_period = WDT_PERIOD_DEFAULT;
#define WDTP(x)		(TCR_WP(x))
#define WDTP_MASK	(TCR_WP_MASK)
//static DEFINE_SPINLOCK(booke_wdt_lock);
//extern int (*stop_ap2400_watchdog)(void);
#define feed_wd_interval 3
#endif

static struct timer_list dog_timer;

/*Begin : enable watchdoc for APv7 [liuqiang 2013-3-15]*/
#define INTER_REGS_BASE			0xFBB00000
#define WD_MASK_REG (*((volatile uint32_t *)(INTER_REGS_BASE|0x20704))) //datasheet Table 305
#define WD_TIMER_CTRL_REG (*((volatile uint32_t *)(INTER_REGS_BASE|0x21840))) //datasheet Table 212
#define WD_TIMER_REG (*((volatile uint32_t *)(INTER_REGS_BASE|0x21864))) //datasheet Table 214

/*End : enable watchdoc for APv7 [liuqiang 2013-3-15]*/

static void wrcpu_dog_timer_fn(unsigned long data)
{

	WD_TIMER_REG = 0x2FFFFFFF;
	mod_timer(&dog_timer, jiffies + HZ);
}

void wrcpu_dog_timer_init(void)
{  
	dog_timer.function = wrcpu_dog_timer_fn;
	dog_timer.expires = jiffies + HZ;
	add_timer(&dog_timer);
}
void start_watchdog(void)
{
    printk("start watchdog\n");
	WD_MASK_REG = 0x1;
	WD_TIMER_REG = 0x2FFFFFFF;
	WD_TIMER_CTRL_REG = 0x30011;
	
  	init_timer(&dog_timer);
    wrcpu_dog_timer_init();
}

void stop_watchdog(void)
{   
	WD_TIMER_CTRL_REG = 0x30001;
	del_timer(&dog_timer);
	printk("stop watchdog\n");
}


