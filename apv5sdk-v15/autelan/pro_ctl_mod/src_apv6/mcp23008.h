/*
 * AUTELAN wbs led & reset & fiber optic configuration functions
 *
 * Copyright 2012 AUTELAN Software Engineering, chenxf <chenxf@autelan.com>
 *
 * LED and interrupt handlers for Power button 
 *
 */

#ifndef _MCP23008_H_
#define _MCP23008_H_


#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/err.h>

/*
 * Driver data (common to all clients)
 */

/* Addresses to scan */
static unsigned short normal_i2c[] = { 0x20,I2C_CLIENT_END };

/* The mcp23008 registers */
#define MCP23008_IO_DIR_REG	          0x00
#define MCP23008_INPUT_POLARITY_REG	  0x01
#define MCP23008_GPINTEN_REG	      0x02
#define MCP23008_DEFVAL_REG	          0x03
#define MCP23008_INTCON_REG	          0x04
#define MCP23008_IOCON_REG	          0x05
#define MCP23008_GPPU_REG	          0x06
#define MCP23008_INTF_REG	          0x07
#define MCP23008_INTCAP_REG	          0x08
#define MCP23008_GPIO_REG	          0x09
#define MCP23008_OLAT_REG	          0x0a

/* For each function bit */
#define POWER_LED_BIT	     (0x01 << 0)
#define BEEP_BIT	         (0x01 << 1)
#define WIFI_2G_BIT	         (0x01 << 2)
#define WIFI_5G_BIT	         (0x01 << 3)
#define SFP0_TXDIS_BIT	     (0x01 << 4)
#define SFP0_ABSENT_BIT	     (0x01 << 5)
#define ALERT_L_BIT	         (0x01 << 6)
#define FACT_RESET_BIT	     (0x01 << 7)


static const struct i2c_device_id mcp23008_id[] = {
	{ "mcp23008", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, mcp23008_id);


struct mcp23008_data {
	struct i2c_adapter *i2c_adp;
	struct i2c_client  *mcp_client;

//for reset button
	struct timer_list bt_detect_timer;
	u8 bt_flag_array[3];
	u8 bt_flag_i;
	struct work_struct bt_detect_wq;
//for led op
	struct work_struct led_wq;
	u8 led_info;
};

struct mcp23008_data *mcp_data = NULL;

/* 8 bit registers, 8 bit values */
int mcp23008_writereg(struct mcp23008_data *mcp23008, u8 reg, u8 data)
{
	int ret;
	u8 buf[] = { reg, data };

	struct i2c_msg msg = { .addr = mcp23008->mcp_client->addr,
			       .flags = 0, .buf = buf, .len = 2 };

	ret = i2c_transfer(mcp23008->i2c_adp, &msg, 1);

	if (ret != 1)
		printk("%s: writereg error (reg == 0x%02x, val == 0x%04x, "
		       "ret == %i)\n", __func__, reg, data, ret);

	return (ret != 1) ? -1 : 0;
}

u8 mcp23008_readreg(struct mcp23008_data *mcp23008, u8 reg)
{
	int ret;
	u8 b0[] = { reg };
	u8 b1[] = { 0 };

	struct i2c_msg msg[] = {
		{ .addr = mcp23008->mcp_client->addr, .flags = 0,
		  .buf = b0, .len = 1 },
		{ .addr = mcp23008->mcp_client->addr, .flags = I2C_M_RD,
		  .buf = b1, .len = 1 } };

	ret = i2c_transfer(mcp23008->i2c_adp, msg, 2);

	if (ret != 2)
		printk(KERN_ERR "%s: readreg error (ret == %i)\n",
		       __func__, ret);
	return b1[0];
}


/* led lighting fn
 * note: because of the following linghting led fn, pointer mcp_data has to change to global one, but we keep the ori coding style
 *
 * and also we should consider this situation, led_on & led_off may be called in
 * a timer or an interrupt fn which can not sleep; in this case ,we should use
 * workqueue to avoid it.
 *
 *led_info : 0 turn on 2g; 1 turn on 5g; 2 turn off 2g; 3 turn off 5g
 */

#if 0   //we have to put these fn into interrupt_led.h, to avoid fn redefinition
void open_2G_led(void){ mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)&~(WIFI_2G_BIT));}
void close_2G_led(void){ mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)|(WIFI_2G_BIT));}
void open_5G_led(void){ mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)&~(WIFI_5G_BIT));}
void close_5G_led(void){ mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)|(WIFI_5G_BIT));}
#endif

void led_op_work(struct work_struct *led_work)
{	
	struct mcp23008_data *mcp_data = container_of(led_work, struct mcp23008_data, led_wq);
	
	switch (mcp_data->led_info)
	{
		case 0 :
			//keep the corresponding bit low, then led is on
			mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)&~(WIFI_2G_BIT));
			break;
		case 1 :
			mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)&~(WIFI_5G_BIT));
			break;
		case 2 :
			mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)|(WIFI_2G_BIT));
			break;
		case 3 :
			mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)|(WIFI_5G_BIT));
			break;
		default :
			break;
	}

}
/*Begin: Modified by wangjia, instead of using macro APM82181_WBS */
extern int is_wbs;
/***********yuanding modified for POE power led **********/
void open_power_led(void) { 
    if(is_wbs) {
        mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, 
            mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)|(POWER_LED_BIT));
    } else {
        ;
    }
}

void close_power_led(void) { 
    if(is_wbs) {
        mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, 
            mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)&~(POWER_LED_BIT));
    } else {
        ;
    }
}
/********** yuanding modified end ************************/

void start_beeping(void){ 
    if(is_wbs) {
        mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, 
            mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)&~(BEEP_BIT));
    } else {
        ;
    }
}
void stop_beeping(void) { 
    if(is_wbs) {
        mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, 
            mcp23008_readreg(mcp_data, MCP23008_GPIO_REG)|(BEEP_BIT));
    } else {
        ;
    }
}
/*End: Modified by wangjia, instead of using macro APM82181_WBS */


/*==========================================*/
//for reset button part
extern unsigned int reset_irq_num;  //we should have a global varible to check if the button has been kept pushing for 3 secs

void bt_detect_work(struct work_struct *work)
{
	u8 flag;
	unsigned int time_diff;
	struct mcp23008_data *mcp_data = container_of(work, struct mcp23008_data, bt_detect_wq);

	flag = mcp23008_readreg(mcp_data,MCP23008_GPIO_REG);
	flag = flag >> 7;
	
	if(mcp_data->bt_flag_i >= 3)
		mcp_data->bt_flag_i = 0;
	
	if( 0 == flag )
	{
		unsigned int i;
		u8 button_flag = 1;
		mcp_data->bt_flag_array[mcp_data->bt_flag_i] = 0;
		for(i = 0; i < 3; i++)
		{
			if( mcp_data->bt_flag_array[i] != 0 )
				button_flag = 0;
		}
		if(button_flag)
		{
			reset_irq_num = 3;
			printk("reset_irq_num = %d\n",reset_irq_num);
		}
	}
	else
	{
		mcp_data->bt_flag_array[mcp_data->bt_flag_i] = 1;
	}
	mcp_data->bt_flag_i++;
}

static void bt_detect_timer_fn(struct mcp23008_data *mcp_data)
{
	schedule_work(&mcp_data->bt_detect_wq);
	mod_timer(&mcp_data->bt_detect_timer, jiffies + HZ);
}

void bt_detect_timer_init(struct mcp23008_data **mcp_str)
{
	init_timer(&(*mcp_str)->bt_detect_timer);
	(*mcp_str)->bt_detect_timer.function = bt_detect_timer_fn;
	(*mcp_str)->bt_detect_timer.data     = *mcp_str;
	(*mcp_str)->bt_detect_timer.expires  = jiffies + HZ;      //check per sec
	add_timer(&(*mcp_str)->bt_detect_timer);
}
/*==========================================*/

static int mcp23008_detect(struct i2c_client *client,
			 struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	strlcpy(info->type, mcp23008_id[0].name, I2C_NAME_SIZE);
	dev_info(&adapter->dev, "Detected TI %s chip at 0x%02x\n",
		 "mcp23008", client->addr);

	return 0;
}

static int mcp23008_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
//	struct mcp23008_data *mcp_data = NULL;
	
	mcp_data = kzalloc(sizeof(struct mcp23008_data), GFP_KERNEL);
	if (!mcp_data)
		return -ENOMEM;

	i2c_set_clientdata(client, mcp_data);
	mcp_data->mcp_client = client;
	mcp_data->i2c_adp    = client->adapter;
	printk("%s: i2c slave device name = %s, addr = 0x%x\n",\
			__func__,mcp_data->mcp_client->name,mcp_data->mcp_client->addr);

/* set all pins pull up */
	mcp23008_writereg(mcp_data, MCP23008_GPPU_REG, 0xff);

/* set default value for each pin
 * attention: SFP0_TXDIS is an output pin, fiber optic can only work when we set SFP0_TXDIS output 0
**/
	mcp23008_writereg(mcp_data, MCP23008_GPIO_REG, \
			(SFP0_ABSENT_BIT | ALERT_L_BIT | FACT_RESET_BIT | POWER_LED_BIT | BEEP_BIT | WIFI_2G_BIT | WIFI_5G_BIT));

/* set IO according to the func : 1 represent input; 0 represent output*/
	mcp23008_writereg(mcp_data, MCP23008_IO_DIR_REG, (SFP0_ABSENT_BIT | ALERT_L_BIT | FACT_RESET_BIT));

/* then we turn on the power led */
	open_power_led();
/* init led operation work */
	mcp_data->led_info = 0xff;
	INIT_WORK(&mcp_data->led_wq, led_op_work);

	
/* for the reset button init */
	{
		u8 array = 0;
		mcp_data->bt_flag_i = 0;
		for (array = 0; array < 3; array++)
		{
			mcp_data->bt_flag_array[array] = 1;
		}
		bt_detect_timer_init(&mcp_data);
		INIT_WORK(&mcp_data->bt_detect_wq, bt_detect_work);
	}

	return 0;
}

static int mcp23008_remove(struct i2c_client *client)
{
	struct mcp23008_data *data = i2c_get_clientdata(client);
	
	del_timer(&data->bt_detect_timer);
	kfree(data);
	return 0;
}

struct i2c_driver mcp23008_driver = {
	.driver = {
		.name	= "mcp23008",
	},
	.probe    = mcp23008_probe,
	.remove   = mcp23008_remove,
	.id_table = mcp23008_id,
	.detect   = mcp23008_detect,
	.address_list = normal_i2c,
};

#if 0
static int __init mcp23008_init(void)
{
	return i2c_add_driver(&mcp23008_driver);
}

static void __exit mcp23008_exit(void)
{
	i2c_del_driver(&mcp23008_driver);
}

MODULE_AUTHOR("chenxf <chenxf@autelan.com>");
MODULE_DESCRIPTION("Microchip MCP23008 8-bit I/O Expander with serial interface"
		   " driver");
MODULE_LICENSE("GPL");

module_init(mcp23008_init);
module_exit(mcp23008_exit);
#endif
#endif //#ifndef _MCP23008_H_
