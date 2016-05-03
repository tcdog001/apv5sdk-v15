/*************************
 *rbk @autelan
 *
 *2008.1.10
 * **********************/
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/delay.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/smp_lock.h>
#include <linux/wait.h>
#include <linux/irq.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/netlink.h>
#include <linux/spinlock.h>
#include <net/sock.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include "pro_ctl_mod_ap83.h"
#if defined( PC018 )||defined( XH9344 )|| defined( XJ9344 )|| defined (AQ2000_X)
#include "ar9350.h"
#else
#if defined( AE5000ENT )
#include "ar9330.h"
#else
#include "ar7100.h"
#endif
#endif
/*
 * Macros to convert between "watchdog ticks" and milliseconds.
 * The watchdog counts down at the system frequency (1/4 CPU freq).
 */
//#define wdticks_to_ms(wdticks) (((wdticks) * 1000) / ar531x_sys_frequency())
//#define ms_to_wdticks(ms) (((ms) / 1000) * ar531x_sys_frequency())



#define WRCPU_MINOR 131
#define BUTTON_WAIT 3 //rbk add for button irq wait
#ifdef XJAP93
#define AQ2000_LED 12  //green led
#else
#ifdef AQ2000N
#define AQ2000_LED 14
#else
#ifdef WNAP550
#define AQ2000_LED  13
#else
#define AQ2000_LED  0
#endif
#endif
#endif

#ifdef AP2400
#define AP83_LED 4
#else
#ifdef WNAP350
#define AP83_LED 2
#else
#if defined ( PC018 )&& !defined (XH9344)&& !defined (XJ9344)&& !defined (AQ2000_X)
#define AP83_LED 15
#else
#ifdef XH9344
#define AP83_LED 11
#else
#ifdef XJ9344
#define AP83_LED 12
#define POWER_LED 11
#else
#if defined (AQ2000_X)   // AQ2000_X added by dengpanhong
#define AP83_LED 14
#else 
#ifdef AE5000ENT
#define AP83_LED 7
#else
#define AP83_LED 1

#endif
#endif
#endif
#endif
#endif
#endif
#endif

#if 0
//defined (LTEFI)
#define ACC_GPIO  21
#define SHUT_GPIO 22
#define ACC_TIMER 267 // cat /proc/cpuinfo
struct timer_list acc_timer;
struct timer_list sig_timer;
struct tasklet_struct acc_tasklet;
int acc_flag=0;
int acc_time=10;
int total_time=0;
int total_time1=1800;
int sem_ret=0;
struct semaphore acc_sem;
struct fasync_struct *acc_async;
#endif
#if 0
#ifdef WNAP210
#define AP83_BUTTON 21
#else 
#ifdef XJAP93
#define AP83_BUTTON 11
#else
#define AP83_BUTTON 8
#endif
#endif
#endif
#define BLIND_TIMER 900
extern struct timer_list dog_timer;//wangyu change the dog_timer

wrcpu_op_args wrcpu_ioc_op_args;
static int wrcpu_is_open;
unsigned int reg_val;
static int reset_irq_num = 0;
#ifdef AQ2000N
extern int product_test_flag;
#endif
/**/
unsigned char first_flag = 1;
static unsigned int ar7100_watchdog_timer_value = 0xa4824200;

unsigned char sn_buff[SN_BUF];
unsigned char ap83Mac_buff[MAC_BUF];
unsigned char ProductId_buff[PRO_BUF];
#ifndef AP2400
unsigned char hwverion_buff[HW_BUF];
#endif
unsigned char oem_buff[OEM_BUF];

struct timer_list ap83_led_blink;
struct timer_list xjpb44_led_blink;
#define AR7100_WATCHDOG_MIN_MS 1000
/*AQ2000CN temperature soft i2c*/
#if defined(AQ2000N)||defined( PC018 )
#define SLAVE_ADDR 0x49
#define INTERNAL_ADDR 0x00
#endif
//wangyu add temperature POA 
#ifdef PC018
#define GPIO_SCL 11
#define GPIO_SDA 12
#else
#define GPIO_SCL 0
#define GPIO_SDA 1
#endif

#define AR9350_WATCHDOG_MIN_MS 1000
#define AR9330_WATCHDOG_MIN_MS 1000
//wangyu add for gpio_led
#if defined ( PC018 )&& !defined (XH9344)&& !defined (XJ9344) && !defined (AQ2000_X)
uint8_t GPIO_POWER_IN = 16;//at first is 22
uint8_t GPIO_POWER_OUT = 20;//at first is 21
uint8_t GPIO_2GLED_ONE = 15;
uint8_t GPIO_2GLED_TWO = 19;
uint8_t GPIO_5GLED = 21;
uint8_t GPIO_LINK = 22;
uint8_t hardware_flag = 0;
#endif
//wangyu add end

/*wangyu add 2011-09-29*/
#ifdef XH9344 
uint8_t GPIO_2GLED_ONE = 14;
uint8_t GPIO_2GLED_TWO = 14;
uint8_t GPIO_5GLED = 14;
uint8_t GPIO_LINK = 13;
#endif
/*wangyu add end*/

/*wangyu add 2012-06-04*/
#ifdef  XJ9344 
uint8_t GPIO_2GLED_ONE = 14;
uint8_t GPIO_2GLED_TWO = 14;
uint8_t GPIO_5GLED = 14;
uint8_t GPIO_LINK = 13;
#endif
/*wangyu add end*/
#ifdef AQ2000_X      // dengpanhong add 
uint8_t GPIO_2GLED_ONE = 13;
uint8_t GPIO_2GLED_TWO = 13;
uint8_t GPIO_5GLED = 13;
uint8_t GPIO_LINK = 4;
#endif
/*wangyu add 2012-09-06*/
#ifdef  AE5000ENT 
uint8_t GPIO_5GLED = 17;
uint8_t GPIO_2GLED_ONE = 17;
uint8_t GPIO_2GLED_TWO = 17;
uint8_t GPIO_LINK = 17;
#endif
/*wangyu add end*/
//wangyu add for HWversion flag 2012-03-05
#if defined(PC018) && ! defined( XH9344 )&& !defined (XJ9344)
#define PROD_INFO_OFFSET 0x9ffe0000
#define GPIO_TEMP_CONTROL 20//used to be 18
#endif
//wangyu add end


uint32_t cpu_freq = 0, ahb_freq, ddr_freq;
extern int test_flag; //ghy test
void up_led_blink(void);
void down_led_blink(void);
void led_on(void);
void led_down(void);
void up_ap2400_led_blink(int val);
void down_ap2400_led_blink(int val);
void up_aq2000_green_led(void);
void down_aq2000_green_led(void);
void up_xj_pb44_led_blink(void);
void down_xj_pb44_led_blink(void);
int ax_soft_i2c_set_addr8( uint8_t slave_dev_addr, uint8_t internal_addr);
static void ax_soft_i2c_receive_byte(uint8_t* data, int is_last_byte);
int ax_soft_i2c_read_bytes(uint8_t slave_dev_addr_7bit, uint8_t* data, int data_len);
//wangyu add for gpio_led
extern int (*turn_up_link_led)(void);
extern int (*turn_down_link_led)(void);
extern int (*turn_up_2G_led)(void);
extern int (*turn_down_2G_led)(void);
extern int (*turn_up_5G_led)(void);
extern int (*turn_down_5G_led)(void);
//END:wangyu add for gpio_led
/******************************************************************************/
void up_led_blink(void)
{
	led_on();
	ap83_led_blink.function = down_led_blink;
	ap83_led_blink.expires = jiffies+BLIND_TIMER;
	add_timer(&ap83_led_blink);
}

void down_led_blink(void)
{
	led_down();
	ap83_led_blink.function = up_led_blink;
	ap83_led_blink.expires = jiffies+BLIND_TIMER;
	add_timer(&ap83_led_blink);
}

void led_on(void)
{
	ar7100_gpio_config_output(AP83_LED);
#ifndef AP2400 	
	ar7100_gpio_out_val(AP83_LED, 0);
#else 
	ar7100_gpio_out_val(AP83_LED, 1);
#endif
}

void led_down(void)
{
	ar7100_gpio_config_output(AP83_LED);
#ifndef AP2400
	ar7100_gpio_out_val(AP83_LED, 1);
#else 
	ar7100_gpio_out_val(AP83_LED, 0);
#endif
}

void led_on_ap2400(int val)
{
	//printk("val=%d\n",val);
	ar7100_gpio_config_output(val);
	ar7100_gpio_out_val(val, 0);
}
void led_down_ap2400(int val)
{
	//printk("val=%d\n",val);
	ar7100_gpio_config_output(val);
	ar7100_gpio_out_val(val, 1);
}
/*begin:wangyu add for the gpio operation*/
void set_gpio_on(uint8_t gpio_N)
{
	led_on_ap2400(gpio_N);

}
void set_gpio_down(uint8_t gpio_N)
{
	led_down_ap2400(gpio_N);

}

int up_link_led(void)
{
	set_gpio_on(GPIO_LINK);
	return 1;
	
}
int down_link_led(void)
{
	set_gpio_down(GPIO_LINK);
	return 1;
}
int up_2G_led(void)
{
	set_gpio_on(GPIO_2GLED_ONE);
	set_gpio_on(GPIO_2GLED_TWO);
	return 1;
}
int down_2G_led(void)
{
	set_gpio_down(GPIO_2GLED_ONE);
	set_gpio_down(GPIO_2GLED_TWO);
	return 1;
}
int up_5G_led(void)
{
	set_gpio_on(GPIO_5GLED);
	return 1;
}
int down_5G_led(void)
{
	set_gpio_down(GPIO_5GLED);
	return 1;
}

/*end:wangyu add for the gpio operation*/

/*Begin:Add by zhangsiyu for AE5000ENT turn off all lights 2012-12-12*/
#ifdef AE5000ENT
int turn_on_all_lights_AE5000(void)
{
    int checklink_turn_off_all_lights_flag;
	checklink_turn_off_all_lights_flag = (ar7100_reg_rd(AR9330_GPIO_OUT) & (1 << 16));
	ar7100_reg_wr(AR9330_GPIO_OE, (ar7100_reg_rd(AR9330_GPIO_OE) | (1 << 15)));
	ar7100_reg_wr(AR9330_GPIO_OUT, (ar7100_reg_rd(AR9330_GPIO_OUT) | (1 << 15)));
	//led_down_ap2400(1);
	led_on_ap2400(14);
	ar7100_reg_wr(AR9330_GPIO_FUNCTIONS2,(ar7100_reg_rd(AR9330_GPIO_FUNCTIONS2) | 0x11));
	if(checklink_turn_off_all_lights_flag)
	{
        ar7100_reg_wr(GPIO_IN_ETH_SWITCH_LED,(ar7100_reg_rd(GPIO_IN_ETH_SWITCH_LED) | (1 << 3)));
	}
	else
	{
	led_on_ap2400(13);
	}
}
int turn_off_all_lights_AE5000(void)
{
    ar7100_reg_wr(AR9330_GPIO_OE, (ar7100_reg_rd(AR9330_GPIO_OE) | (1 << 15)));
	ar7100_reg_wr(AR9330_GPIO_OUT, (ar7100_reg_rd(AR9330_GPIO_OUT) & (~(1 << 15))));
	led_down_ap2400(14);
	led_on_ap2400(0);
	ar7100_reg_wr(GPIO_IN_ETH_SWITCH_LED,(ar7100_reg_rd(GPIO_IN_ETH_SWITCH_LED) & (~(1 << 3))));
	led_on_ap2400(13);
	led_on_ap2400(1);
}
#endif
/*End:Add by zhangsiyu for AE5000ENT turn off all lights 2012-12-12*/
void down_ap2400_led_blink(int val)
{
#if defined(XJAP93) || defined(AQ2000N) || defined(WNAP550)
	led_on_ap2400(val); //red led down***
	down_aq2000_green_led();
#else
	led_down_ap2400(val);
#endif
	ap83_led_blink.data= val;
	ap83_led_blink.function = up_ap2400_led_blink;
	ap83_led_blink.expires = jiffies+BLIND_TIMER;
	add_timer(&ap83_led_blink);
}
void down_aq2000_green_led(void)
{
	ar7100_gpio_config_output(AQ2000_LED);
	ar7100_gpio_out_val(AQ2000_LED, 1);
}
void up_aq2000_green_led(void)
{
	ar7100_gpio_config_output(AQ2000_LED);
	ar7100_gpio_out_val(AQ2000_LED, 0);
}
void up_ap2400_led_blink(int val)
{
#if defined(XJAP93) || defined(AQ2000N) || defined(WNAP550)
	led_down_ap2400(val); //red led up
	up_aq2000_green_led();//green led down
#else
	led_on_ap2400(val); //led on
#endif
	ap83_led_blink.data= val;
	ap83_led_blink.function = down_ap2400_led_blink;
	ap83_led_blink.expires = jiffies+BLIND_TIMER;
	add_timer(&ap83_led_blink);
}
/********************************************************************/
/*ghy add for xjpb44 in 20100304*/
void led_down_xjpb44(void)
{
	ar7100_gpio_config_output(5);
	ar7100_gpio_out_val(5, 1);
}
void led_on_xjpb44(void)
{
	ar7100_gpio_config_output(5);
	ar7100_gpio_out_val(5, 0);
}
void down_xj_pb44_led_blink(void)
{
	led_on_xjpb44();
	xjpb44_led_blink.function = up_xj_pb44_led_blink;
	xjpb44_led_blink.expires = jiffies+BLIND_TIMER;
	add_timer(&xjpb44_led_blink);
}
void up_xj_pb44_led_blink(void)
{
	led_down_xjpb44();
	xjpb44_led_blink.function = down_xj_pb44_led_blink;
	xjpb44_led_blink.expires = jiffies+BLIND_TIMER;
	add_timer(&xjpb44_led_blink);
}


/*********************************************************************/
static irqreturn_t ap2400_button_reset(int cpl, void *dev_id, struct pt_regs *regs)
{
	unsigned int reg_val;
	int i;
	char a[BUTTON_WAIT];
	char button_flag = 1;
	int flag;

	reg_val = ar7100_gpio_in_val(AP83_BUTTON);
	flag = reg_val;
	if(!flag){
		mdelay(999);
		reg_val = ar7100_gpio_in_val(AP83_BUTTON);
		if(!reg_val){
			for(i=0;i<2;i++){
				mdelay(999);
				reg_val = ar7100_gpio_in_val(AP83_BUTTON);
				a[i] = reg_val;
				if(a[i]!=0)
					button_flag = 0;
			}
			if(button_flag){
				printk("\npush the button 3S and state = %d\n", reg_val);
				reset_irq_num=3;
				//printk("reset_irq_num = %d\n",reset_irq_num);
			}
		}
		else{
			if(reset_irq_num!=3){
				reset_irq_num=1;
				printk("\npush the button and state = %d\n", flag);
				//kernel_restart(NULL);
				//printk("reset_irq_num = %d\n",reset_irq_num);
			}
		}
	}
	return IRQ_HANDLED;
}

/*Begin:Add by zhangisyu for AE5000 transplantation 2012-11-22*/
#ifdef AE5000ENT
static irqreturn_t ap83_button_reset(int cpl, void *dev_id, struct pt_regs *regs)
{
    unsigned int reg_val;
	int i;
	char a[BUTTON_WAIT];
	char button_flag = 1;
	int flag;

	reg_val = ar7100_gpio_in_val(AP83_BUTTON);
	flag = reg_val;
	if(!flag){
		mdelay(999);
		reg_val = ar7100_gpio_in_val(AP83_BUTTON);
		if(!reg_val){
			for(i=0;i<2;i++){
				//
				mdelay(999);
				reg_val = ar7100_gpio_in_val(AP83_BUTTON);
				a[i] = reg_val;
				if(a[i]!=0)
					button_flag = 0;
			}
			if(button_flag){
				printk("\npush the button 3S and state = %d\n", reg_val);
				reset_irq_num=3;
				//printk("reset_irq_num = %d\n",reset_irq_num);
			}
		}
		else{
			if(reset_irq_num!=3){
				reset_irq_num=1;
				printk("\npush the button and state = %d\n", flag);
				//kernel_restart(NULL);
				//printk("reset_irq_num = %d\n",reset_irq_num);
			}
		}
	}
	return IRQ_HANDLED;
}
#else
/*Begin:Add by zhangisyu for AE5000 transplantation 2012-11-22*/

/*ghy add for ap83 button*/ /*为0时有中断*/
static irqreturn_t ap83_button_reset(int cpl, void *dev_id, struct pt_regs *regs)
{
	unsigned int reg_val;
	int i;
	char a[BUTTON_WAIT];
	char button_flag = 1;

	for(i=0;i<BUTTON_WAIT;i++){
		mdelay(999);
		reg_val = ar7100_gpio_in_val(AP83_BUTTON);
		a[i] = reg_val;
		//printk("a[i]=%d\n",a[i]);
		if(a[i]!=0)
			button_flag = 0;
	}

	if(button_flag){
		reset_irq_num = 3;
		printk("reset_irq_num = %d\n",reset_irq_num);
#if defined (LTEFI)		
		kernel_restart(NULL);
#endif
	}
	return IRQ_HANDLED;
}
/*ghy add for ap83 */

#endif    //Add by zhangisyu for AE5000 transplantation 2012-11-22

#if 0
//defined (LTEFI)
void sig_send(int val)
{
	int ret;
	
	ret = ar7100_reg_rd(AR9350_GPIO_OE);
	printk("0x%x:0x%08x\n", AR9350_GPIO_OE, ret);
	ret = ar7100_reg_rd(AR9350_GPIO_IN);
	printk("0x%x:0x%08x\n", AR9350_GPIO_IN, ret);
	ret = ar7100_reg_rd(AR9350_GPIO_OUT);
	printk("0x%x:0x%08x\n", AR9350_GPIO_OUT, ret);

	printk("SHUT_GPIO by pro_ctl_mod.ko!\n");
	ar7100_gpio_config_output(SHUT_GPIO); //SDA output
	ar7100_gpio_out_val(SHUT_GPIO, 0x1); // SHUT_GPIO=1
}
void shut_gpio_set(int val)
{
	unsigned int reg_val;
	int i;
	char a[BUTTON_WAIT];
	char button_flag = 1;

	for(i=0;i<BUTTON_WAIT;i++){
		mdelay(99);
		reg_val = ar7100_gpio_in_val(ACC_GPIO);
		a[i] = reg_val;
//		printk("acc_gpio_1 a[i]=%d\n",a[i]);
		if(a[i] != 1)
			button_flag = 0;
	}
	if(0 == button_flag){
		printk("ACC_TIMER stop!\n");
//		printk("%s %d, unlock=%d\n\n", __func__, __LINE__, acc_sem);
		up(&acc_sem);
//		printk("%s %d, unlock=%d\n\n", __func__, __LINE__, acc_sem);
		acc_flag = 0;
	}
	else if(total_time >= total_time1){
		printk("pro_ctl_mod.ko send a SIGIO!\n");
		kill_fasync(&acc_async, SIGIO, POLL_IN);

		sig_timer.function = sig_send;
		sig_timer.expires = jiffies + acc_time*ACC_TIMER;
		add_timer(&sig_timer);
//		printk("%s %d, unlock=%d\n\n", __func__, __LINE__, acc_sem);
		up(&acc_sem);
//		printk("%s %d, unlock=%d\n\n", __func__, __LINE__, acc_sem);
		acc_flag = 0;
	}
	else
	{
		acc_timer.expires = jiffies+acc_time*ACC_TIMER;
		total_time += acc_time;
		add_timer(&acc_timer);
		printk("ACC_TIMER restart! ----%d\n", total_time/acc_time);
	}
}
void tasklet_action(unsigned long arg)
{
	acc_flag = 1;
	del_timer(&acc_timer);
	acc_timer.function = shut_gpio_set;
	acc_timer.expires = jiffies+acc_time*ACC_TIMER;
	total_time += acc_time;
	printk("ACC_TIMER start!\n");
	add_timer(&acc_timer);
}
static irqreturn_t acc_gpio_set(int cpl, void *dev_id, struct pt_regs *regs)
{
	unsigned int reg_val;
	int i;
	char a[BUTTON_WAIT];
	char button_flag = 1;

	for(i=0;i<BUTTON_WAIT;i++){
		mdelay(99);
		reg_val = ar7100_gpio_in_val(ACC_GPIO);
		a[i] = reg_val;
//		printk("acc_gpio a[i]=%d\n",a[i]);
		if(a[i] != 1)
			button_flag = 0;
	}
	if(button_flag){
//		printk("%s %d, lock=%d\n\n", __func__, __LINE__, acc_sem);
		sem_ret = down_trylock(&acc_sem);
//		printk("%s %d, lock=%d, sem_ret=%d\n\n", __func__, __LINE__, acc_sem, sem_ret);
		if(sem_ret == 0){
			total_time = 0;
			tasklet_schedule(&acc_tasklet);
		}
	}
	return IRQ_HANDLED;
}
#endif

int  set_ap83_gpio_irq(void)
{
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 )||defined(AQ2000_X)
	unsigned int ret = ar7100_reg_rd(AR9350_GPIO_INT_ENABLE);
#else
#if defined( AE5000ENT )
	unsigned int ret = ar7100_reg_rd(AR9330_GPIO_INT_ENABLE);
#else
	unsigned int ret = ar7100_reg_rd(AR7100_GPIO_INT_ENABLE);
#endif
#endif
	ret |= 1<<AP83_BUTTON;
#if 0
//defined (LTEFI)
	ret |= 1<<ACC_GPIO;
#endif
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 )||defined(AQ2000_X)
	ar7100_reg_wr(AR9350_GPIO_INT_ENABLE, ret);
#else
#if defined( AE5000ENT )
	ar7100_reg_wr(AR9330_GPIO_INT_ENABLE, ret);
#else
	ar7100_reg_wr(AR7100_GPIO_INT_ENABLE, ret);
#endif
#endif
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 )
	ret = ar7100_reg_rd(AR9350_GPIO_INT_ENABLE);
#else
#if defined( AE5000ENT )
	ret = ar7100_reg_rd(AR9330_GPIO_INT_ENABLE);
#else
	ret = ar7100_reg_rd(AR7100_GPIO_INT_ENABLE);
#endif
#endif

	ar7100_gpio_config_input(AP83_BUTTON);

#if defined(PC018)||defined( XH9344 )||defined( XJ9344 )||defined(AQ2000_X) || defined(AE5000ENT)
	int req = request_irq(AR7100_GPIO_IRQn(AP83_BUTTON), ap83_button_reset, IRQF_SAMPLE_RANDOM, "ap83_button_reset", NULL);
#else
#ifdef AP2400
	int req = request_irq(AR7100_GPIO_IRQn(AP83_BUTTON), ap2400_button_reset, SA_SAMPLE_RANDOM, "ap2400_button_reset", NULL);
#else
	int req = request_irq(AR7100_GPIO_IRQn(AP83_BUTTON), ap83_button_reset, SA_SAMPLE_RANDOM, "ap83_button_reset", NULL);
#endif
#endif

	if(req !=0){
		printk("irq set is wrong and errno is %d\n", req);
		return -EINVAL;
	}
#if 0
//defined (LTEFI)
		ar7100_gpio_config_input(ACC_GPIO);
		tasklet_init(&acc_tasklet, tasklet_action, 789);
		int req1 = request_irq(AR7100_GPIO_IRQn(ACC_GPIO), acc_gpio_set, IRQF_SAMPLE_RANDOM, "acc_gpio_set", NULL);
		if(req1 !=0){
			printk("irq1 set is wrong and errno is %d\n", req1);
			return -EINVAL;
		}
#endif
	return 0;
}
/*ghy add end*/

static inline void get_sys_frequency()
{
	uint32_t pll, pll_div, cpu_div, ahb_div, ddr_div, freq,usec;
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 ) || defined (AQ2000_X)
	pll = ar7100_reg_rd(AR9350_PLL_CONFIG);
#else
#if defined( AE5000ENT )
	pll = ar7100_reg_rd(AR9330_PLL_CONFIG);
#else
	pll = ar7100_reg_rd(AR7100_PLL_CONFIG);
#endif
#endif

	pll_div  = ((pll >> PLL_DIV_SHIFT) & PLL_DIV_MASK) + 1;
	freq     = pll_div * 40000000;
	cpu_div  = ((pll >> CPU_DIV_SHIFT) & CPU_DIV_MASK) + 1;
	ddr_div  = ((pll >> DDR_DIV_SHIFT) & DDR_DIV_MASK) + 1;
	ahb_div  = (((pll >> AHB_DIV_SHIFT) & AHB_DIV_MASK) + 1)*2;

	cpu_freq = freq/cpu_div;
	ddr_freq = freq/ddr_div;
	ahb_freq = cpu_freq/ahb_div;

}

void watchdog_notify_alive(void)
{

	uint32_t usec;
	/* lisongbai modify the wd timeout to the max, because different platform has different ahb_freq value:
	 *  AQ2000N2/NH = 120M, AQ2000SNH = 100M AP2400-IFM=170M
	 */
	//	usec = 35ul * USEC_PER_SEC;
	//	usec = usec * (ahb_freq / USEC_PER_SEC);
	/* watchdog timer timeout for reboot:35sec ahb_freq = 120 000 000  */
	//	printk("usec = %lu\n", usec);
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 ) || defined (AQ2000_X)
	usec = 900000000ul;
#else
#if defined(XJPB44_02) || defined(TDPB44_02)
	usec = 3400000000ul;
#else
	usec = 4200000000ul;
	//	printk("ahb_freq = %lu\n", ahb_freq);//pc018 ahb_freq  is 113333333
#endif
#endif
	/* lisongbai modify end */
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 ) || defined(AQ2000_X)
	int reg = ar7100_reg_rd(AR9350_WATCHDOG_TMR);
	//printk("**reg=%x\n", reg);
	ar7100_reg_wr(AR9350_WATCHDOG_TMR, usec);
#else
#if defined( AE5000ENT )
	int reg = ar7100_reg_rd(AR9330_WATCHDOG_TMR);
	//printk("**reg=%x\n", reg);
	ar7100_reg_wr(AR9330_WATCHDOG_TMR, usec);
#else
	int reg = ar7100_reg_rd(AR7100_WATCHDOG_TMR);
	//printk("**reg=%x\n", reg);
	ar7100_reg_wr(AR7100_WATCHDOG_TMR, usec);
#endif
#endif
	//sysWbFlush();

}

watchdog_start(unsigned int milliseconds)
{
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 ) || defined(AQ2000_X)
	if (milliseconds < AR9350_WATCHDOG_MIN_MS)
#else
#if defined( AE5000ENT )
	if (milliseconds < AR9330_WATCHDOG_MIN_MS)
#else
	if (milliseconds < AR7100_WATCHDOG_MIN_MS)
#endif
#endif
			return -1;

	watchdog_notify_alive(); /* Initialize timer */
	/*
	 * Cause AHB error interrupt on watchdog expiration.
	 * See bug 14407 for details.
	 */
	// int val = ar7100_reg_rd(AR7100_WATCHDOG_TMR_CONTROL);
	//printk("1,val=%x\n", val);
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 ) || defined(AQ2000_X)
	ar7100_reg_wr(AR9350_WATCHDOG_TMR_CONTROL, 3);
#else
#if defined( AE5000ENT )
	ar7100_reg_wr(AR9330_WATCHDOG_TMR_CONTROL, 3);
#else
	ar7100_reg_wr(AR7100_WATCHDOG_TMR_CONTROL, 3);
#endif
#endif
	//val = ar7100_reg_rd(AR7100_WATCHDOG_TMR_CONTROL);
	//printk("2,val=%x\n", val);
	//sysRegWrite(AR531XPLUS_WDC, WDC_AHB_INTR);
	return 0;
}
#define feed_wd_interval 10 // lisongbai add interval times of seed watchdog
#if defined (AQ2000N) || defined( PC018 )
static void AX_SOFT_I2C_DELAY(int us)
{
	udelay(us);
}
int ax_soft_i2c_start(void)
{
	ar7100_gpio_config_output(GPIO_SDA); //SDA output
	ar7100_gpio_out_val(GPIO_SDA, 1); // SDA=1
	ar7100_gpio_config_output(GPIO_SCL);
	ar7100_gpio_out_val(GPIO_SCL,1);// SCL=1
	//	ar7100_gpio_config_output(GPIO_SDA); 
	ar7100_gpio_out_val(GPIO_SDA,0);// SDA=0
	//	ar7100_gpio_config_output(0);
	ar7100_gpio_out_val(GPIO_SCL,0);// SCL=0
}
static void ax_soft_i2c_stop(void)
{
	ar7100_gpio_config_output(GPIO_SCL);//DIR=0, cpu write
	ar7100_gpio_out_val(GPIO_SCL,0); //SCL=0, SDA could be changed
	ar7100_gpio_config_output(GPIO_SDA);
	ar7100_gpio_out_val(GPIO_SDA,0); //SDA=0
	//	ar7100_gpio_config_output(GPIO_SCL);
	ar7100_gpio_out_val(GPIO_SCL,1);  //SCL=1
	//	ar7100_gpio_config_output(GPIO_SDA);
	ar7100_gpio_out_val(GPIO_SDA,1); //SDA=1, sda jump to high, i2c stop
}

/**
 * Soft I2C send byte as master-sender.
 * @return: AX_SOFT_I2C_OK(0) for success. AX_SOFT_I2C_FAIL(-1) for failed.
 */
static int ax_soft_i2c_send_byte(uint8_t data)
{
	int i;
	//send byte data
	ar7100_gpio_config_output(GPIO_SCL);
	ar7100_gpio_out_val(GPIO_SCL,0);//SCL=0, SDA chould be changed
	ar7100_gpio_config_output(GPIO_SDA);
	//ar7100_gpio_config_output(0);
	//DIR=0, cpu write   //ghy ???
	//	printk("..be about to sent 0x%x\n", data);
	for (i=7; i>=0; i--)
	{
		ar7100_gpio_config_output(GPIO_SCL);
		ar7100_gpio_out_val(GPIO_SCL,0);//SCL=0, SDA chould be changed
		if (data & (0x1<<i))
		{
			ar7100_gpio_config_output(GPIO_SDA); 
			ar7100_gpio_out_val(GPIO_SDA,1);//SDA=1, //bit is high
		}
		else
		{
			ar7100_gpio_config_output(GPIO_SDA);
			ar7100_gpio_out_val(GPIO_SDA,0); //SDA=0 //bit is low
		}
		ar7100_gpio_config_output(GPIO_SCL); 
		ar7100_gpio_out_val(GPIO_SCL,1); //SCL=1, SDA data valid
	}
	//receive ACK

	ar7100_gpio_config_output(GPIO_SCL);
	ar7100_gpio_out_val(GPIO_SCL,0); //SCL=0, SDA chould be changed, no delay
	ar7100_gpio_config_input(GPIO_SDA); //DIR=1, cpu read     // add add ??? after
	ar7100_gpio_config_output(GPIO_SCL);
	ar7100_gpio_out_val(GPIO_SCL,1);//SCL=1, SDA data valid

	if(ar7100_gpio_in_val(GPIO_SDA)!=0){
		ar7100_gpio_config_output(GPIO_SCL);
		ar7100_gpio_out_val(GPIO_SCL,0);  //SCL=0
		return AX_SOFT_I2C_FAIL; //ACK=0, failed
	}else{
		ar7100_gpio_config_output(GPIO_SCL);
		ar7100_gpio_out_val(GPIO_SCL,0);  //SCL=0
		return AX_SOFT_I2C_OK; //ACK=0, success
	}

}

int ax_soft_i2c_write_bytes(uint8_t slave_dev_addr_7bit, uint8_t* data, int data_len)
{
	int i;
	//uint64_t cpld_base_addr;
	uint8_t first_byte = 0; //high 7 bit is addr, LSB=0 indicates write operation
	first_byte |= (slave_dev_addr_7bit << 1);
	first_byte &= ~((uint8_t)0x1); //LSB=0, write operation
	//printk("data=%x\n", *data);
	if (data == NULL || data_len <= 0)
	{
		printk("ax_soft_i2c_write_bytes: param invalid!\n");
		return AX_SOFT_I2C_FAIL;
	}

	//cpld_base_addr = AX_SOFT_I2C_CONVERT_SLOT_NUM_TO_CPLD_ADDR(slot_num);
	//ASI_DEBUG("ax_soft_i2c_write_bytes: now begin to write data to i2c, slot num is %d, dev addr is %#x, data length is %d...\n", slot_num, slave_dev_addr_7bit, data_len);
	ax_soft_i2c_start(); //start i2c
	if (ax_soft_i2c_send_byte(first_byte) != AX_SOFT_I2C_OK) //write dev_addr and op code
	{
		printk("ax_soft_i2c_write_bytes: write first byte failed, no ACK...\n");
		return AX_SOFT_I2C_FAIL;
	}
	for (i=0; i<data_len; i++)
	{
		if (ax_soft_i2c_send_byte(*(data+i)) != AX_SOFT_I2C_OK)
		{
			printk("ax_soft_i2c_write_bytes: write data byte%d(%#x) failed, no ACK...\n", i, *(data+i));
			return AX_SOFT_I2C_FAIL;
		}
		AX_SOFT_I2C_DELAY(1000);
		//		printk("ax_soft_i2c_write_bytes: write data byte%d(%#x) ok...\n", i, *(data+i));
	}
	ax_soft_i2c_stop(); //stop i2c
	AX_SOFT_I2C_DELAY(9000);
	return AX_SOFT_I2C_OK;
}

int ax_soft_i2c_set_addr8( uint8_t slave_dev_addr, uint8_t internal_addr)
{
	uint8_t  tmp[2];
	tmp[0] = (internal_addr >> 8) & 0xff; //internal addr's high 8 bit
	tmp[1] = (internal_addr) & 0xff; //internal addr's low 8 bit
	return ax_soft_i2c_write_bytes( slave_dev_addr, tmp, 1);
}

/**
 * Soft I2C receive byte as master-receiver.
 * @param  is_last_byte:  if is last byte, then should send NO_ACK.
 * @return: AX_SOFT_I2C_OK(0) for success. AX_SOFT_I2C_FAIL(-1) for failed.
 */
static void ax_soft_i2c_receive_byte(uint8_t* data, int is_last_byte)
{
	int i;
	uint8_t tmp = 0;
	//receive data byte
	ar7100_gpio_config_output(GPIO_SCL); 
	ar7100_gpio_out_val(GPIO_SCL,0);// SCL=0	 //SCL=0, SDA chould be changed, no delay
	ar7100_gpio_config_input(GPIO_SDA);  //DIR=1, cpu read   // ghy ???
	ar7100_gpio_config_output(GPIO_SCL);
	for (i=7; i>=0; i--)
	{
		ar7100_gpio_config_output(GPIO_SCL);
		ar7100_gpio_out_val(GPIO_SCL,1);//SCL=1, SDA data valid, receive data from slave
		tmp |= (ar7100_gpio_in_val(GPIO_SDA) << i); //read from SDA
		ar7100_gpio_config_output(GPIO_SCL);
		ar7100_gpio_out_val(GPIO_SCL,0); //SCL=0, SDA chould be changed
	}
	*data = tmp;
	//send ACK or NO_ACK
	//SCL already=0, SDA chould be changed
	// ghy ????  	// AX_SOFT_I2C_DIR(0, cpld_base_addr, 1); //DIR=0, cpu write
	ar7100_gpio_config_output(GPIO_SDA);
	ar7100_gpio_config_output(GPIO_SCL);
	if (is_last_byte){
		ar7100_gpio_config_output(GPIO_SDA);
		ar7100_gpio_out_val(GPIO_SDA, 1);//is last byte, SDA=1, send NO_ACK
	}else{
		ar7100_gpio_config_output(GPIO_SDA);
		ar7100_gpio_out_val(GPIO_SDA,0); //is not last byte, SDA=0, send ACK
	}
	ar7100_gpio_config_output(GPIO_SCL);
	ar7100_gpio_out_val(GPIO_SCL,1); //SCL=1, SDA data valid, send ACK or NO_ACK
	ar7100_gpio_config_output(GPIO_SCL);
	ar7100_gpio_out_val(GPIO_SCL,0); //SCL=0

	return;
}

/**
 * Function for read bytes from i2c.
 * @param    slave_dev_addr:  slave device's 7 bit addr. ONLY  low 7 bit valid!
 * @param    data: buffer to be put data which is read from i2c.
 * @param    data_len: the data length to be read.
 * @return: AX_SOFT_I2C_OK(0) for success. AX_SOFT_I2C_FAIL(-1) for failed.
 */
int ax_soft_i2c_read_bytes(uint8_t slave_dev_addr_7bit, uint8_t* data, int data_len)
{
	int i;
	//uint64_t cpld_base_addr;
	uint8_t first_byte = 0; //high 7 bit is addr, LSB=1 indicates read operation
	first_byte |= (slave_dev_addr_7bit << 1);
	first_byte |= ((uint8_t)0x1); //LSB=1, read operation
	if (data == NULL || data_len <= 0)
	{
		printk("ax_soft_i2c_read_bytes: param invalid!\n");
		return AX_SOFT_I2C_FAIL;
	}
	//cpld_base_addr = AX_SOFT_I2C_CONVERT_SLOT_NUM_TO_CPLD_ADDR(slot_num);
	//	printk("ax_soft_i2c_read_bytes: now begin to read data from i2c, dev addr is %#x, data length is %d...\n", slave_dev_addr_7bit, data_len);
	ax_soft_i2c_start(); //start i2c
	if (ax_soft_i2c_send_byte(first_byte) != AX_SOFT_I2C_OK) //write dev_addr and op code
	{
		printk("ax_soft_i2c_read_bytes: write first byte failed, no ACK...\n");
		return AX_SOFT_I2C_FAIL;
	}
	for (i=0; i<data_len-1; i++) //receive (data_len-1)  bytes, the last byte is sent later
	{
		ax_soft_i2c_receive_byte(data+i, 0);
		printk("ax_soft_i2c_read_bytes: get byte %d, %#x\n", i, *(data+i));
	}
	AX_SOFT_I2C_DELAY(1000);
	ax_soft_i2c_receive_byte(data+data_len-1, 1); //receive the last byte, send no ACK
	AX_SOFT_I2C_DELAY(5000);
	ax_soft_i2c_stop(); //stop i2c
	return AX_SOFT_I2C_OK;
}
#endif
static void wrcpu_dog_timer_fn(void)
{
	watchdog_notify_alive();
	mod_timer(&dog_timer, jiffies + HZ * feed_wd_interval);
}

void wrcpu_dog_timer_init()
{
	dog_timer.function = wrcpu_dog_timer_fn;
	dog_timer.expires = jiffies + HZ * feed_wd_interval;                    //
	add_timer(&dog_timer);
}

void wrcpu_stop_watchdog()
{
	//ar7100_reg_wr(AR7100_WATCHDOG_TMR_CONTROL, 0);
	del_timer(&dog_timer);
}
EXPORT_SYMBOL(wrcpu_stop_watchdog);

/*****************************************************************/
	static int
wrcpu_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int i;
	int reg_val;
	int flag;
	unsigned long reg;
#if defined( PC018 )|| defined(AQ2000N)
	//	uint8_t data[1024];
	uint8_t data1;
	uint8_t* data=&data1;
	uint16_t m_data;
#endif
	if (_IOC_TYPE(cmd) != WRCPU_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > WRCPU_IOC_MAXNR) return -ENOTTY;
	memset(&wrcpu_ioc_op_args,0,sizeof(wrcpu_op_args));

	switch (cmd) {
		case WRCPU_IOC_G_:
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			wrcpu_ioc_op_args.op_value=ar7100_reg_rd(wrcpu_ioc_op_args.op_addr);
			copy_to_user((wrcpu_op_args *)arg,&wrcpu_ioc_op_args,sizeof(wrcpu_op_args));

			//		ar7100_reg_rd(0xb1000090,1);

			break;
		case WRCPU_IOC_X_:
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			printk("Write addr[0x%08x] 32 bits with [0x%08x].\n",wrcpu_ioc_op_args.op_addr,wrcpu_ioc_op_args.op_value);
			ar7100_reg_wr(wrcpu_ioc_op_args.op_addr,(unsigned int)wrcpu_ioc_op_args.op_value);
			mdelay(10);
			wrcpu_ioc_op_args.op_value =  ar7100_reg_rd(wrcpu_ioc_op_args.op_addr);
			printk("Read  addr[0x%08x] 32 bits got  [0x%08x].\n",wrcpu_ioc_op_args.op_addr,wrcpu_ioc_op_args.op_value);
			copy_to_user((wrcpu_op_args *)arg,&wrcpu_ioc_op_args,sizeof(wrcpu_op_args));
			break;
		case NETGEAR_DOWN_LED:
			del_timer(&ap83_led_blink);
			del_timer(&xjpb44_led_blink);
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			reg_val =  wrcpu_ioc_op_args.num;
#if defined( AP2400 )|| defined ( AE5000ENT )
			led_on_ap2400(reg_val); //***down the led
#else
			//printk("reg_val=%d\n",reg_val);
			led_down_ap2400(reg_val);
#endif
			break;
		case WRCPU_IOC_B_ :/*blue led on*/
			del_timer(&ap83_led_blink);
			del_timer(&xjpb44_led_blink);
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			reg_val =  wrcpu_ioc_op_args.num;

#if defined( AP2400 )|| defined ( AE5000ENT )
			led_down_ap2400(reg_val); //****up the  led
#else
			//printk("reg_val=%d\n",reg_val);
			led_on_ap2400(reg_val);
#endif
			break;
			/***********************************************************************************************/
#if 0
		case WRCPU_IOC_I_:/*get sysinf*/
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			if(first_flag){
				//ae531x_get_board_config();
				/*				printk("----------\n");
								for(i=0;i<184;i++){
								if(!(i%10)){
								printk("\n");
								}
								printk("  %2x",*ap83_ProductId_addr);
								ap83_ProductId_addr++;
								}
								*/
				//memset(ap83Mac_buff,0,12);
				unsigned char *ptr_mac = ap83_mac_addr;
				for(i=0; i<MAC_BUF; i++){
					//unsigned char *ptr = ap83_mac_addr;
					ap83Mac_buff[i] =*ptr_mac;
					//printk("mac : %c", ap83Mac_buff[i]);
					ptr_mac++;
				}
#ifndef WNAP210
				unsigned char *ptr_sn = ap83_sn_addr;
				for(i=0; i<SN_BUF; i++){
					sn_buff[i] = *ptr_sn;
					ptr_sn++;
				}

				unsigned char *ptr_pro = ap83_ProductId_addr;
				for(i=0;i<PRO_BUF;i++){
					ProductId_buff[i] = *ptr_pro;
					//	printk("***%c",ProductId_buff[i]);
					ptr_pro++;
				}
#endif
#ifndef AP2400
				unsigned char *ptr_hw = hwverion;
				for(i=0; i<HW_BUF; i++){
					hwverion_buff[i] = *ptr_hw;
					ptr_hw++;
				}
#endif
#if defined(XHAP93) || defined(XJAP93_02) || defined(XJAP93) || defined(AQ2000N)
				unsigned char *ptr_oem = oeminfo;
				for(i=0;i<OEM_BUF;i++){
					oem_buff[i] = *ptr_oem;
					ptr_oem++;
				}
				first_flag = 1;
#else
#ifdef AP2400
				first_flag = 1;
#else 
				first_flag = 0;
#endif
#endif
			}
			memcpy(wrcpu_ioc_op_args.ap83Mac,ap83Mac_buff,MAC_BUF);		
			memcpy(wrcpu_ioc_op_args.ProductId, ProductId_buff,PRO_BUF);
			//printk("***%c", wrcpu_ioc_op_args.ProductId[1]);
			memcpy(wrcpu_ioc_op_args.wnapSN ,sn_buff,SN_BUF);

#ifndef AP2400
			memcpy(wrcpu_ioc_op_args.hwver, hwverion_buff,HW_BUF);
#endif
#if defined(XJAP93) || defined(XJAP93_02) || defined(XHAP93) || defined(AQ2000N)
			memcpy(wrcpu_ioc_op_args.oem_data, oem_buff,OEM_BUF);
#endif
			//wrcpu_ioc_op_args.factory_reset_get = 0;
			copy_to_user((wrcpu_op_args *)arg, &wrcpu_ioc_op_args, sizeof(wrcpu_op_args));	
			break;
#endif
			/***********************************************************************************************/

		case WRCPU_IOC_L_:
			wrcpu_ioc_op_args.factory_reset_get = reset_irq_num;
			//wrcpu_ioc_op_args.button_factory_reset_get = autelan_button_num;
			copy_to_user((wrcpu_op_args *)arg,&wrcpu_ioc_op_args,sizeof(wrcpu_op_args));
			/*ghy add this reset_irq_num=0 for XJ_PB44*/
			reset_irq_num = 0;
			break;
		case NETGEAR_LED_BLINK: /* up ap83 leb blink*/
			del_timer(&ap83_led_blink);
			del_timer(&xjpb44_led_blink);
			init_timer(&ap83_led_blink);
			init_timer(&xjpb44_led_blink);

			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			reg_val =  wrcpu_ioc_op_args.num;
			up_ap2400_led_blink(reg_val);
			break;
		case WRCPU_IOC_S_:/*gpio n's state*/
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			flag = wrcpu_ioc_op_args.num;
			if(flag == AP83_BUTTON){
				wrcpu_ioc_op_args.status = ar7100_gpio_in_val(wrcpu_ioc_op_args.num);
			}else{
				int state = ar7100_gpio_in_val(wrcpu_ioc_op_args.num);
				printk("state=%d\n", state);
			}
			copy_to_user((wrcpu_op_args *)arg,&wrcpu_ioc_op_args,sizeof(wrcpu_op_args));
			break;
		case WRCPU_IOC_U_:
			init_timer(&dog_timer);
			get_sys_frequency();
			wrcpu_dog_timer_init(); 
			watchdog_start(60000);
			break;
		case WRCPU_IOC_D_:
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 )|| defined(AQ2000_X)
			ar7100_reg_wr(AR9350_WATCHDOG_TMR_CONTROL, 0);
#else
#if defined( AE5000ENT )
			ar7100_reg_wr(AR9330_WATCHDOG_TMR_CONTROL, 0);
#else
			ar7100_reg_wr(AR7100_WATCHDOG_TMR_CONTROL, 0);
#endif
#endif
			del_timer(&dog_timer);
			break;
		case WRCPU_IOC_O_:
			del_timer(&xjpb44_led_blink);
			init_timer(&xjpb44_led_blink);
			up_xj_pb44_led_blink();
			break;
		case FREE_IRQ: /*XJ free irq when stop_monitor */
			free_irq(AR7100_GPIO_IRQn(AP83_BUTTON), NULL);
			printk("now free the button irq\n");
			break;
		case SET_IRQ:/*XJ set irq*/
#ifdef AQ2000N
			//printk("%d\n",product_test_flag);
			wrcpu_ioc_op_args.product_test_flag_app = product_test_flag;
			copy_to_user((wrcpu_op_args *)arg,&wrcpu_ioc_op_args,sizeof(wrcpu_op_args));
#endif
			break;
#if defined(AQ2000N) || defined( PC018 ) && !defined( XH9344 )&& !defined( XJ9344 )
		case WOC_GET_TEMPUTER:
			if (ax_soft_i2c_set_addr8(SLAVE_ADDR, INTERNAL_ADDR) != 0)
				return (-1);
			if (ax_soft_i2c_read_bytes(SLAVE_ADDR, data, 1) != 0){
				return (-1);
			}
			//printk("data=%x\n",*(uint16_t *)data);
			m_data = *(uint16_t *)data;
			m_data = m_data >> 15;
			if(m_data == 1){
				/* the temperature is < 0 */
				*(uint16_t *)data = *(uint16_t *)data >> 6;
				wrcpu_ioc_op_args.temperature = ((*(uint16_t *)data^0x3ff)+1)/4;
				wrcpu_ioc_op_args.temperature = - wrcpu_ioc_op_args.temperature;
				//printk("temperature = -%d\n",wrcpu_ioc_op_args.temperature);  //take 10 MSBs 
			}else{
				*(uint16_t *)data = *(uint16_t *)data >> 6;
				//f_data =*(uint16_t *)data/4;
				wrcpu_ioc_op_args.temperature =*(uint16_t *)data/4;
				//printk("temperature = %d\n",wrcpu_ioc_op_args.temperature);
			}
			copy_to_user((wrcpu_op_args *)arg,&wrcpu_ioc_op_args,sizeof(wrcpu_op_args));
			break;
#endif
			//wangyu add the function for PC018
#if defined(PC018) && ! defined( XH9344 )&& !defined( XJ9344 ) && !defined (AQ2000_X)
			//#if 1
		case WRCPU_POWER_IN:/*WRCPU_POWER_IN state*/
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			wrcpu_ioc_op_args.status = ar7100_gpio_in_val(GPIO_POWER_IN);

			copy_to_user((wrcpu_op_args *)arg,&wrcpu_ioc_op_args,sizeof(wrcpu_op_args));
			break;
		case WRCPU_POWER_OUT:/*WRCPU_POWER_OUT state*/
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			wrcpu_ioc_op_args.status = ar7100_gpio_in_val(GPIO_POWER_OUT);

			copy_to_user((wrcpu_op_args *)arg,&wrcpu_ioc_op_args,sizeof(wrcpu_op_args));
			break;
		case SET_TEMP_UP:/*temperature rise*/
			del_timer(&ap83_led_blink);
			del_timer(&xjpb44_led_blink);
			led_down_ap2400(GPIO_TEMP_CONTROL);
			break;
		case SET_TEMP_DOWN:/*temperature down*/
			del_timer(&ap83_led_blink);
			del_timer(&xjpb44_led_blink);
			led_on_ap2400(GPIO_TEMP_CONTROL);
			break;

#endif
			//wangyu add end
/*Begin:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/
#if defined(AE5000ENT)
        case TURN_OFF_ALL_LED:
			del_timer(&ap83_led_blink);
			del_timer(&xjpb44_led_blink);
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			turn_off_all_lights_AE5000();
			break;
		case TURN_ON_ALL_LED:
			del_timer(&ap83_led_blink);
			del_timer(&xjpb44_led_blink);
			copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
			turn_on_all_lights_AE5000();
			break;
#endif
/*End:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/
#if 0
//defined (LTEFI)			
		case SET_ACC_GPIO_TIMER:
			if(0 == acc_flag)
			{
				copy_from_user(&wrcpu_ioc_op_args,(wrcpu_op_args *)arg,sizeof(wrcpu_op_args));
				total_time1 =  wrcpu_ioc_op_args.num;		
				printk("\nmodify the acc_time=%d(sec) now\n", total_time1);
			}
			else
			{
				printk("\nCannot be modified, Kill check_lte first!\n");
			}
			break;
		case GET_ACC_GPIO_TIMER:
			if(0 == acc_flag)
			{
				printk("\nacc_time=%d(sec)\n", total_time1);
			}
			else
			{
				printk("\nacc_time=%d(sec), Cannot be modified\n", total_time1);
			}
			break;
#endif
		default:
			return -1;
			break;
	}
	return 0;
}
static int  stop_the_watchdog(void)
{
	del_timer(&dog_timer);
	return 1;

}
extern int (*stop_ap2400_watchdog)(void);
static int wrcpu_open(struct inode* inode, struct file* file)
{
	switch (MINOR(inode->i_rdev)) {
		case WRCPU_MINOR:
			if (wrcpu_is_open)
				return -EBUSY;
			wrcpu_is_open = 1;
			return 0;
		default:
			return -ENODEV;
	}
}
static int wrcpu_close(struct inode* inode, struct file* file)
{
	if (MINOR(inode->i_rdev) == WRCPU_MINOR) {
		wrcpu_is_open = 0;
	}
	return 0;
}
#if 0
//defined (LTEFI)
static int wrcpu_fasync(int fd, struct file* filp, int mode)
{
    return fasync_helper(fd, filp, mode, &acc_async);
}
#endif
static struct file_operations wrcpu_fops =
{
owner:		THIS_MODULE,
			ioctl:		wrcpu_ioctl,
			open:		wrcpu_open,
			release:	wrcpu_close,
#if 0
//defined (LTEFI)
			fasync:		wrcpu_fasync,
#endif
};
static struct miscdevice wrcpu_miscdev = {
	WRCPU_MINOR,
	"wrcpu",
	&wrcpu_fops
};

	static int __init
wrcpu_init(void)
{
	int val;
	printk("\nin ap83 pro_ctl_mod init\n");
#ifdef AQ2000N
	ar7100_reg_wr(AR7100_GPIO_FUNCTIONS,0x00048062); // must set bit 2-7 to 0
#endif
	//wangyu add for GPIO function register
#ifdef XJAP93_02
	ar7100_reg_wr(AR7100_GPIO_FUNCTIONS,0x00048083); 
	set_gpio_down(13);
#endif
	//wangyu add end

	misc_register(&wrcpu_miscdev);
#if defined(PC018) && !defined( XH9344 )&& !defined( XJ9344 )&& !defined (AQ2000_X)
	//	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS,0x00048002); // must set bit 2-7 to 0


	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS1,0x0b0a0900); //wangyu add for the GPIO 4 
	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS2,0x00180000); //wangyu add for GPIO 11
	
	//ar7100_reg_wr(AR9350_GPIO_FUNCTIONS4,0x00000000); //wangyu add for GPIO 16
	// Autelan-added: chenxf comment it because of gpio 16 should be configured to UART1 TD mode 2014-09-13 
#endif	
	//	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS3,0x00000000);
	//	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS4,0x0000004d); 
	
#if defined ( XJ9344 )
		ar7100_reg_wr(AR9350_GPIO_FUNCTIONS3,0x00000000); //wangyu add for GPIO 12
		ar7100_reg_wr(AR9350_GPIO_FUNCTIONS2,0x00180000); //wangyu add for GPIO 11
#endif

//add for AQ2000_X by dengpanhong 

#ifdef AQ2000_X
#if 0
    // printk("\n........AQ2000-X2.........\n");
		ar7100_reg_wr(AR9350_GPIO_OE,0x00021319);
		ar7100_reg_wr(AR9350_GPIO_FUNCTIONS2,0x2d180000);
		ar7100_reg_wr(AR9350_GPIO_FUNCTIONS4,0x0000002d);
		set_gpio_on(AP83_LED);
		//ar7100_gpio_config_output(14);
   //	ar7100_gpio_out_val(14, 0);
		ar7100_gpio_config_input(AP83_BUTTON);
#endif
#endif
//dengpanhong add end

#if 0
//defined (LTEFI)
	sema_init(&acc_sem, 1);
	init_timer(&acc_timer);
	init_timer(&sig_timer);
	ar7100_gpio_out_val(12, 1);//for LTEFI Ver.D --- 0 disable LTE 1;
	ar7100_gpio_out_val(15, 1);//for LTEFI Ver.D --- 0 reset LTE 1;
	ar7100_gpio_out_val(16, 1);//for LTEFI Ver.D --- 0 disable LTE 2;
	ar7100_gpio_out_val(20, 1);//for LTEFI Ver.D --- 0 reset LTE 2;
	ar7100_gpio_config_input(ACC_GPIO);
	ar7100_gpio_config_output(SHUT_GPIO);
	ar7100_gpio_out_val(SHUT_GPIO, 0);
#endif
#if defined (LTEFI)
	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS3,0x002f3300);
	val = ar7100_reg_rd(AR9350_GPIO_FUNCTIONS4);
	val = (val & (~0x00ff0000)) | 0x002e0000;
	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS4, val);  //Autelan-added: chenxf modified for gpio16 2014-09-13
#endif
#if defined (AK2000_EN2T)
	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS2,0x00180000);
	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS3,0x002f3300);
	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS4,0x002e0000);
#endif
//begin:wangyu add for HWversion flag 2012-03-05	
#if defined(PC018)&& !defined( XH9344 )&& !defined( XJ9344 ) && !defined (AQ2000_X)
   // #error("+++++++++++++++pc018+++++++++++");
	unsigned char hwversion_buf[HW_BUF];
	unsigned char *ptr_hw =  PROD_INFO_OFFSET + HW_OFFSET;
	uint8_t i;
	for(i=0; i<HW_BUF; i++){
		if(*ptr_hw == 0xff)break;
		hwversion_buf[i] = *ptr_hw;
		ptr_hw++;
	}
//	printk("***********the hard version is %s ****PROD_INFO_OFFSET + HW_OFFSET is 0x%08x***********\n",
//				hwversion_buf,PROD_INFO_OFFSET + HW_OFFSET);
	if(strcmp(hwversion_buf,"5.0") == 0){
			GPIO_2GLED_ONE = 17;
			GPIO_2GLED_TWO = 17;
#if defined(LTEFI_V2)
			GPIO_5GLED = 17;
			GPIO_LINK = 15;
#else
			GPIO_5GLED = 15;
			GPIO_LINK = 16;
#endif
			hardware_flag = 1;
//	printk(".......enter the 5.0...............");
		}
	else if(strcmp(hwversion_buf,"6.0") == 0){
			GPIO_2GLED_ONE = 17;
			GPIO_2GLED_TWO = 17;
			GPIO_5GLED = 21;
			GPIO_LINK = 22;
			hardware_flag = 2;
//			printk("enter the 6.0...............");
		}
#endif
//end:wangyu add for HWversion flag 2012-03-05	

#if defined(PC018)&& !defined( XH9344 )&& !defined( XJ9344 ) && ! defined (AQ2000_X)
	//	printk("the GPIO stats value is 0x%08x......\n",ar7100_reg_rd(0x18040004));

	ar7100_gpio_config_input(AP83_BUTTON);

	if(!hardware_flag)
		ar7100_gpio_config_input(GPIO_POWER_IN);
	else		
		ar7100_gpio_config_output(GPIO_POWER_IN);
	set_gpio_down(GPIO_POWER_OUT);
#endif
	//	printk("the GPIO stats value is 0x%08x......\n",ar7100_reg_rd(0x18040004));

#if defined(PC018)||defined( XH9344 )||defined( XJ9344 ) || defined (AQ2000_X)
	down_link_led();//set GPIO_LINK to 1
	down_2G_led();//set GPIO_2GLED_ONE GPIO_2GLED_TWO to 1
	down_5G_led();//set GPIO_5GLED to 1

	turn_up_link_led = up_link_led;
	turn_down_link_led = down_link_led;
	turn_up_2G_led = up_2G_led;
	turn_down_2G_led = down_2G_led;
	turn_up_5G_led = up_5G_led;
	turn_down_5G_led = down_5G_led;

#endif

#if defined(PC018)&& !defined( XH9344 )&& !defined( XJ9344 ) && !defined (AQ2000_X)
	if( hardware_flag == 2)
		set_gpio_on(AP83_LED);
	/*begin:wangyu add for GPIO 18 19 xlna*/
	if(( hardware_flag == 1)||( hardware_flag == 2)){
	//ar7100_reg_wr(AR9350_GPIO_FUNCTIONS4,0x2f2e0000); 
	val = ar7100_reg_rd(AR9350_GPIO_FUNCTIONS4);
	val = (val & (~0xffff0000)) | 0x2f2e0000;
	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS4, val);  //Autelan-added: chenxf modified for gpio16 2014-09-13
	ar7100_gpio_config_output(18);//0x2e chain 0 xlna control
	ar7100_gpio_config_output(19);//0x2f chain 1 xlna control
		}
	/*end:wangyu add for GPIO 18 19 xlna*/
#endif

#if 1
	init_timer(&dog_timer);
	get_sys_frequency();
	wrcpu_dog_timer_init();
	watchdog_start(60000);
	stop_ap2400_watchdog = stop_the_watchdog;
#endif

#ifdef XJ9344
set_gpio_on(AP83_LED);
set_gpio_on(POWER_LED);
#endif

#ifdef XH9344
	set_gpio_down(13);
	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS2,0x00180000); //wangyu add for GPIO 11
	ar7100_reg_wr(AR9350_GPIO_FUNCTIONS3,0x00000000); //wangyu add for GPIO 13 used to be 0x00001800
	set_gpio_on(AP83_LED);
#endif
	//ar7100_gpio_config_output(AP83_LED);
	//ar7100_gpio_out_val(AP83_LED, 0);


/*Begin:Modify by zhangsiyu for reset button 2012.11.6*/
#ifdef AE5000ENT                           
    ar7100_reg_wr(AR9330_GPIO_OE, 0x040024ff);
	ar7100_reg_wr(AR9330_GPIO_INT_ENABLE, 0x00201800);
	ar7100_reg_wr(AR9330_GPIO_OUT, (ar7100_reg_rd(AR9330_GPIO_OUT) & (~(1 << 13))));
	//printk("....................._%S__%d_0x1804000=%d\n", __func__, __LINE__,ar7100_reg_rd(0x18040000));/////////
#endif
/*End:Modify by zhangsiyu for reset button 2012.11.6*/

	set_ap83_gpio_irq();

	return 0;
}
	static void __exit
wrcpu_exit(void)
{
#ifdef AP2400
#ifndef XHPB44
	del_timer(&dog_timer);
	stop_ap2400_watchdog = NULL;
	/*led 4 and led 5 down*/
	del_timer(&ap83_led_blink);
	del_timer(&xjpb44_led_blink);
	//ar7100_gpio_config_output(4);
	//ar7100_gpio_out_val(4, 0);
	led_down_xjpb44();
#else
	del_timer(&ap83_led_blink);
	led_down();
#endif
#else
	del_timer(&dog_timer);
	stop_ap2400_watchdog = NULL;
	del_timer(&xjpb44_led_blink);
	del_timer(&ap83_led_blink);
#endif

	free_irq(AR7100_GPIO_IRQn(AP83_BUTTON), NULL);

#if 0
//defined (LTEFI)
	del_timer(&acc_timer);
	del_timer(&sig_timer);
	free_irq(AR7100_GPIO_IRQn(ACC_GPIO), NULL);
#endif

	misc_deregister(&wrcpu_miscdev);
	
#if defined(PC018)||defined( XH9344 )||defined( XJ9344 ) || defined (AQ2000_X)
	turn_up_link_led = NULL;
	turn_down_link_led = NULL;
	turn_up_2G_led = NULL;
	turn_down_2G_led = NULL;
	turn_up_5G_led = NULL;
	turn_down_5G_led = NULL;

	#endif
	printk(KERN_INFO "\nwrcpu exit  MAJOR:10 MINOR:131\n");
}


module_init(wrcpu_init);
module_exit(wrcpu_exit);

MODULE_AUTHOR("caosicong@autelan");
MODULE_DESCRIPTION("Support for Atheros WiSoC Register");
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif

