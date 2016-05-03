//zhengkun add for watchdog 
#include <linux/smp.h>
#include <linux/notifier.h>
#include <linux/watchdog.h>
#include <linux/uaccess.h>

#include <asm/reg_booke.h>
#include <asm/system.h>
#include <asm/time.h>
#include <asm/div64.h>
#define WDT_PERIOD_DEFAULT 3
u32 booke_wdt_enabled;
u32 booke_wdt_period = WDT_PERIOD_DEFAULT;
#define WDTP(x)		(TCR_WP(x))
#define WDTP_MASK	(TCR_WP_MASK)
//static DEFINE_SPINLOCK(booke_wdt_lock);
extern int (*stop_ap2400_watchdog)(void);
extern struct timer_list dog_timer;
static void __booke_wdt_ping(void *data)
{  
	mtspr(SPRN_TSR, TSR_ENW|TSR_WIS);
}
static void booke_wdt_ping(void)
{
	on_each_cpu(__booke_wdt_ping, NULL, 0);
}
//timer related code add by zhengun 
#define feed_wd_interval 3

static void wrcpu_dog_timer_fn()
{   // printk("##feed dog");
     booke_wdt_ping();
	 mod_timer(&dog_timer, jiffies + HZ);
}

void wrcpu_dog_timer_init()
{  
	dog_timer.function = wrcpu_dog_timer_fn;
	dog_timer.expires = jiffies + HZ;                    //
	add_timer(&dog_timer);
}
static void __booke_wdt_enable()
{
	u32 val;

	/* clear status before enabling watchdog */
	__booke_wdt_ping(NULL);
	val = mfspr(SPRN_TCR);
	val &= ~WDTP_MASK;
	val |= (TCR_WIE|TCR_WRC(WRC_SYSTEM)|WDTP(booke_wdt_period));

	mtspr(SPRN_TCR, val);
}
void start_watchdog()
{   printk("start watchdog");
  	init_timer(&dog_timer);
    wrcpu_dog_timer_init();
 __booke_wdt_enable();
}
EXPORT_SYMBOL_GPL(start_watchdog);
int stop_watchdog(void)
{   del_timer(&dog_timer);
	printk("stop watchdog");
}
EXPORT_SYMBOL_GPL(stop_watchdog);

