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
#include <linux/rtc.h>
#include <linux/workqueue.h>
#include "mcp23008.h"



/******************************************************/

//zhengkun add end 
//zhengkun add for gpio control
static int backup_btn_intr = NO_IRQ;
extern void  (*turnon_eth_led)(void);
extern void  (*turnoff_eth_led)(void);
static int power_btn_gpio;
extern unsigned int reset_irq_num;
void gpio_write_bit(int gpio, int val) 
{
	if (gpio_request(gpio, NULL) >= 0) {
		gpio_set_value(gpio, val);
		gpio_free(gpio);
	}
	else 
	printk(KERN_INFO "can not request GPIO %d\n", gpio);
}

int gpio_read_bit(int gpio)
{
	int ret = -1;
        if (gpio_request(gpio, NULL) >= 0) {
		ret = gpio_get_value(gpio);
		gpio_free(gpio);
	}
	return ret;
}

extern int gpio_direction_output(unsigned gpio, int value);
void gpio_set_direction_output(unsigned gpio, int val)
	{
	if (gpio_request(gpio, NULL) >= 0) {
		gpio_direction_output(gpio, val);
		gpio_free(gpio);
		}
	else 
		printk(KERN_INFO "can not request GPIO %d\n", gpio);
}
extern int gpio_direction_input(unsigned gpio);
void gpio_set_direction_input(unsigned gpio)
		{
		if (gpio_request(gpio, NULL) >= 0) {
			gpio_direction_input(gpio);
			gpio_free(gpio);
			}
		else 
			printk(KERN_INFO "can not request GPIO %d\n", gpio);
	}
extern int gpio_to_irq1(unsigned gpio );
 void gpio_set_irq(unsigned gpio) 
{
       if (gpio_request(gpio, NULL) >= 0) 
	   {
		      gpio_to_irq1(gpio);
	          gpio_free(gpio);
       }
	  else
	 printk(KERN_INFO "can not request GPIO %d\n", gpio);
}
//zhengkun add for gpio controlend 
//watchdog code 

/*Begin: Modified by wangjia, instead of using macro APM82181_WBS */
void open_2G_led(void) { 
    mcp_data->led_info = 0; 
    schedule_work(&mcp_data->led_wq);
}

void close_2G_led(void) { 
    mcp_data->led_info = 2; 
    schedule_work(&mcp_data->led_wq);
}

void open_5G_led(void) { 
    mcp_data->led_info = 1; 
    schedule_work(&mcp_data->led_wq);
}

void close_5G_led(void) { 
    mcp_data->led_info = 3; 
    schedule_work(&mcp_data->led_wq);
}

void open_eth_led(void) {
    if(is_wbs) {
        ;
    } else {
        mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, 
             mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)&~(POWER_LED_BIT));
    }
}

void close_eth_led(void) {
    if(is_wbs) {
        ;
    } else {
        mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, 
            mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)|(POWER_LED_BIT));
    }
}
/*End: Modified by wangjia, instead of using macro APM82181_WBS */

EXPORT_SYMBOL_GPL(open_2G_led);
EXPORT_SYMBOL_GPL(close_2G_led);
EXPORT_SYMBOL_GPL(open_5G_led);
EXPORT_SYMBOL_GPL(close_5G_led);
EXPORT_SYMBOL_GPL(open_eth_led);
EXPORT_SYMBOL_GPL(close_eth_led);

static irqreturn_t ppc4xx_button_reset(int irq, void *dev_id)
{
	unsigned int reg_val;
	int i;
	unsigned int a[3];
	char button_flag = 1;
	for(i=0;i<3;i++){
		mdelay(999);
		reg_val = gpio_read_bit(power_btn_gpio);
		a[i] = reg_val;
		if(a[i]!=0)
			button_flag = 0;
	}
	if(button_flag){
		reset_irq_num = 3;
		printk("reset_irq_num = %d\n",reset_irq_num);
	}
	return IRQ_HANDLED;
}


