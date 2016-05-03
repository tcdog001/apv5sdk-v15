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
#include <linux/rtc.h>
#include <linux/workqueue.h>


extern void  (*apv7_gpio_set_value)(unsigned,int);
extern void  (*apv7_stop_watchdog)(void);

typedef unsigned int MV_U32;

#define GPIO_INPUT_OK		(1 << 0)
#define GPIO_OUTPUT_OK		(1 << 1)
#define	MV_GPP_IN	0xFFFFFFFF	/* GPP input */
#define MV_GPP_OUT	0		/* GPP output */

#define MV_GPP_MAX_GROUP    		3 /* group == configuration register? */
#define INTER_REGS_BASE			0xFBB00000
#define MV_GPP_REGS_OFFSET(unit)		(0x18100 + ((unit) * 0x40))

#define MV_32BIT_LE(X)  (X)
#define MV_32BIT_LE_FAST(val)            MV_32BIT_LE(val)
#define MV_MEMIO32_WRITE(addr, data)    \
    ((*((volatile unsigned int*)(addr))) = ((unsigned int)(data)))
    
/* 32bit write in little endian mode */
#define MV_MEMIO_LE32_WRITE(addr, data) \
        MV_MEMIO32_WRITE(addr, MV_32BIT_LE_FAST(data))

#define LED_REG_30 (*((volatile uint32_t *)(INTER_REGS_BASE|0x18100))) 

/*
** Base address for GPP registers.
*/
#define MV_GPP_REGS_BASE(unit)		(MV_GPP_REGS_OFFSET(unit))
#define GPP_DATA_OUT_REG(grp)			(MV_GPP_REGS_BASE(grp) + 0x00)
#define GPP_BLINK_EN_REG(grp)			(MV_GPP_REGS_BASE(grp) + 0x08)
#define GPP_DATA_OUT_EN_REG(grp)		(MV_GPP_REGS_BASE(grp) + 0x04)
#define GPP_DATA_IN_REG(grp)			(MV_GPP_REGS_BASE(grp) + 0x10)
#define GPP_DATA_IN_POL_REG(grp)		(MV_GPP_REGS_BASE(grp) + 0x0C)

#define MV_MEMIO32_READ(addr)           \
    ((*((volatile unsigned int*)(addr))))

#define MV_MEMIO8_READ(addr)            \
    ((*((volatile unsigned char*)(addr))))

#define MV_MEMIO8_WRITE(addr, data)     \
    ((*((volatile unsigned char*)(addr))) = ((unsigned char)(data)))

/* Marvell controller register read/write macros */
#define MV_REG_READ(offset)             \
        (MV_MEMIO_LE32_READ(INTER_REGS_BASE | (offset)))
        
#define MV_REG_WRITE(offset, val)    \
        MV_MEMIO_LE32_WRITE((INTER_REGS_BASE | (offset)), (val))

#define MV_REG_BYTE_READ(offset)        \
        (MV_MEMIO8_READ((INTER_REGS_BASE | (offset))))

#define MV_REG_BYTE_WRITE(offset, val)  \
        MV_MEMIO8_WRITE((INTER_REGS_BASE | (offset)), (val))
        
/* 32bit read in little endian mode */
static unsigned int MV_MEMIO_LE32_READ(unsigned int addr)
{
    unsigned int data;

    data= (unsigned int)MV_MEMIO32_READ(addr);

    return (unsigned int)MV_32BIT_LE_FAST(data);
}

static void gppRegSet(MV_U32 group, MV_U32 regOffs, MV_U32 mask, MV_U32 value)
{
	MV_U32 gppData;

	gppData = MV_REG_READ(regOffs);

	gppData &= ~mask;

	gppData |= (value & mask);

	MV_REG_WRITE(regOffs, gppData);
}

int mvGppValueSet(MV_U32 group, MV_U32 mask, MV_U32 value)
{
	//MV_U32 outEnable, tmp;
	//MV_U32 i;

	if (group >= MV_GPP_MAX_GROUP) {
		printk("mvGppValueSet: Error invalid group number \n");
		return 0;
	}
#if 0
	/* verify that the gpp pin is configured as output              */
	/* Note that in the register out enabled -> bit = '0'.  */
	outEnable = ~MV_REG_READ(GPP_DATA_OUT_EN_REG(group));

	/* Workaround for Erratum FE-MISC-70 */
	if (gppHalData.ctrlRev == MV_88F6XXX_A0_REV && (group == 1)) {
		tmp = ~MV_REG_READ(GPP_DATA_OUT_EN_REG(0));
		outEnable &= 0xfffffffd;
		outEnable |= (tmp & 0x2);
	}
	/*End of WA */
	for (i = 0; i < 32; i++) {
		if (((mask & (1 << i)) & (outEnable & (1 << i))) != (mask & (1 << i))) {
			mvOsPrintf("mvGppValueSet: Err. An attempt to set output "
				   "value to GPP %d in input mode.\n", i);
			return MV_ERROR;
		}
	}
#endif
	gppRegSet(group, GPP_DATA_OUT_REG(group), mask, value);

	return 1;

}

static void __set_level(unsigned pin, int high)
{
	MV_U32 grp = pin >> 5;
	MV_U32 mask = (1 << (pin & 0x1F));

	if (high)
		mvGppValueSet (grp, mask, mask);
	else
		mvGppValueSet (grp, mask, 0);
}

static void mv_gpio_set_value(unsigned pin,int value)
{
	//unsigned long flags;

	/* Configure GPIO output value. */
	__set_level(pin, value);

}

static inline int mv_gpio_is_valid(unsigned pin, int mode)
{
	return true;
}

int mvGppBlinkEn(MV_U32 group, MV_U32 mask, MV_U32 value)
{
	if (group >= MV_GPP_MAX_GROUP) {
		printk("mvGppBlinkEn: ERR. invalid group number \n");
		return 0;
	}

	gppRegSet(group, GPP_BLINK_EN_REG(group), mask, value);

	return 1;

}

static inline void __set_blinking(unsigned pin, int blink)
{
	MV_U32 grp = pin >> 5;
	MV_U32 mask = (1 << (pin & 0x1F));

	if (blink)
		mvGppBlinkEn(grp, mask, mask);
	else
		mvGppBlinkEn(grp, mask, 0);
}

int mvGppTypeSet(MV_U32 group, MV_U32 mask, MV_U32 value)
{
	if (group >= MV_GPP_MAX_GROUP) {
		printk("mvGppTypeSet: ERR. invalid group number \n");
		return 0;
	}

	gppRegSet(group, GPP_DATA_OUT_EN_REG(group), mask, value);
#if 0
	/* Workaround for Erratum FE-MISC-70 */
	if (gppHalData.ctrlRev == MV_88F6XXX_A0_REV && (group == 1)) {
		mask &= 0x2;
		gppRegSet(0, GPP_DATA_OUT_EN_REG(0), mask, value);
	}
#endif
	/*End of WA */
	return 1;
}

static inline void __set_direction(unsigned pin, int input)
{
	MV_U32 grp = pin >> 5;
	MV_U32 mask = (1 << (pin & 0x1F));

	if (input)
		mvGppTypeSet(grp, mask, MV_GPP_IN & mask);
	else
		mvGppTypeSet(grp, mask, MV_GPP_OUT & mask);
}


static int mv_gpio_direction_output(unsigned pin,int value)
{
	//unsigned long flags;

	if (!mv_gpio_is_valid(pin, GPIO_OUTPUT_OK))
		return -EINVAL;
    
	/* Disable blinking. */
	__set_blinking(pin, 0);

	/* Configure GPIO output value. */
	__set_level(pin, value);

	/* Configure GPIO direction. */
	__set_direction(pin, 0);

	return 0;
}

MV_U32 mvGppValueGet(MV_U32 group, MV_U32 mask)
{
	MV_U32 gppData;

	gppData = MV_REG_READ(GPP_DATA_IN_REG(group));

	gppData &= mask;

	return gppData;

}

MV_U32 mvGppPolarityGet(MV_U32 group, MV_U32 mask)
{
	MV_U32 regVal;

	if (group >= MV_GPP_MAX_GROUP) {
		printk("mvGppActiveSet: Error invalid group number \n");
		return 0;
	}
	regVal = MV_REG_READ(GPP_DATA_IN_POL_REG(group));

	return (regVal & mask);
}


static int mv_gpio_get_value(unsigned pin)
{
	MV_U32 val;
	MV_U32 grp = pin >> 5;
	MV_U32 mask = (1 << (pin & 0x1F));

	if (MV_REG_READ(GPP_DATA_OUT_EN_REG(grp)) & mask)
		val = mvGppValueGet(grp, mask) ^ mvGppPolarityGet(grp, mask);
	else
		val = MV_REG_READ(GPP_DATA_OUT_REG(grp));

	return (val >> (pin & 31)) & 1;
}

static int mv_gpio_direction_input(struct gpio_chip *chip, unsigned pin)
{
	//unsigned long flags;

	if (!mv_gpio_is_valid(pin, GPIO_INPUT_OK))
		return -EINVAL;

	/* Configure GPIO direction. */
	__set_direction(pin, 1);

	return 0;
}

