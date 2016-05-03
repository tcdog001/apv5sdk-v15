/*
 * Copyright (c) 2008-2010, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "opt_ah.h"

#ifdef AH_SUPPORT_AR9300

#include "ah.h"
#include "ah_internal.h"
#include "ah_devid.h"
#ifdef AH_DEBUG
#include "ah_desc.h"                    /* NB: for HAL_PHYERR* */
#endif

#include "ar9300/ar9300.h"
#include "ar9300/ar9300reg.h"
#include "ar9300/ar9300phy.h"

#define AR_GPIO_BIT(_gpio)                      (1 << (_gpio))

/*
 * Configure GPIO Output Mux control
 */
#ifndef AR9340_EMULATION
static void
ar9300GpioCfgOutputMux(struct ath_hal *ah, u_int32_t gpio, u_int32_t type)
{
    int          addr;
    u_int32_t    gpio_shift;

    // each MUX controls 6 GPIO pins
    if (gpio > 11) {
        addr = AR_HOSTIF_REG(ah, AR_GPIO_OUTPUT_MUX3);
    } else if (gpio > 5) {
        addr = AR_HOSTIF_REG(ah, AR_GPIO_OUTPUT_MUX2);
    } else {
        addr = AR_HOSTIF_REG(ah, AR_GPIO_OUTPUT_MUX1);
    }

    /*
     * 5 bits per GPIO pin.
     * Bits 0..4 for 1st pin in that mux,
     * bits 5..9 for 2nd pin, etc.
     */
    gpio_shift = (gpio % 6) * 5;

    OS_REG_RMW(ah, addr, (type << gpio_shift), (0x1f << gpio_shift));
}
#endif

/*
 * Configure GPIO Output lines
 */
HAL_BOOL
ar9300GpioCfgOutput(
    struct ath_hal *ah,
    u_int32_t gpio,
    HAL_GPIO_OUTPUT_MUX_TYPE halSignalType)
{
#define N(a)    (sizeof(a) / sizeof(a[0]))
#ifndef AR9340_EMULATION
    u_int32_t    ah_signal_type;
    u_int32_t    gpio_shift;

    static const u_int32_t    MuxSignalConversionTable[] = {
        /* HAL_GPIO_OUTPUT_MUX_AS_OUTPUT             */
        AR_GPIO_OUTPUT_MUX_AS_OUTPUT,
        /* HAL_GPIO_OUTPUT_MUX_AS_PCIE_ATTENTION_LED */
        AR_GPIO_OUTPUT_MUX_AS_PCIE_ATTENTION_LED,
        /* HAL_GPIO_OUTPUT_MUX_AS_PCIE_POWER_LED     */
        AR_GPIO_OUTPUT_MUX_AS_PCIE_POWER_LED,
        /* HAL_GPIO_OUTPUT_MUX_AS_MAC_NETWORK_LED    */
        AR_GPIO_OUTPUT_MUX_AS_MAC_NETWORK_LED,
        /* HAL_GPIO_OUTPUT_MUX_AS_MAC_POWER_LED      */
        AR_GPIO_OUTPUT_MUX_AS_MAC_POWER_LED,
        /* HAL_GPIO_OUTPUT_MUX_AS_WLAN_ACTIVE        */
        AR_GPIO_OUTPUT_MUX_AS_RX_CLEAR_EXTERNAL,
        /* HAL_GPIO_OUTPUT_MUX_AS_TX_FRAME           */
        AR_GPIO_OUTPUT_MUX_AS_TX_FRAME,
    };

    HALASSERT(gpio < AH_PRIVATE(ah)->ah_caps.halNumGpioPins);

    // Convert HAL signal type definitions to hardware-specific values.
    if ((halSignalType >= 0) && (halSignalType < N(MuxSignalConversionTable)))
    {
        ah_signal_type = MuxSignalConversionTable[halSignalType];
    }
    else
    {
        return AH_FALSE;
    }

    // Configure the MUX
    ar9300GpioCfgOutputMux(ah, gpio, ah_signal_type);

    // 2 bits per output mode
    gpio_shift = 2*gpio;

    OS_REG_RMW(ah,
               AR_HOSTIF_REG(ah, AR_GPIO_OE_OUT),
               (AR_GPIO_OE_OUT_DRV_ALL << gpio_shift),
               (AR_GPIO_OE_OUT_DRV << gpio_shift));

#endif
    return AH_TRUE;
#undef N
}

/*
 * Configure GPIO Input lines
 */
HAL_BOOL
ar9300GpioCfgInput(struct ath_hal *ah, u_int32_t gpio)
{
#ifndef AR9340_EMULATION
    u_int32_t    gpio_shift;

    HALASSERT(gpio < AH_PRIVATE(ah)->ah_caps.halNumGpioPins);

    /* TODO: configure input mux for AR9300 */
    /* If configured as input, set output to tristate */
    gpio_shift = 2*gpio;

    OS_REG_RMW(ah,
               AR_HOSTIF_REG(ah, AR_GPIO_OE_OUT),
               (AR_GPIO_OE_OUT_DRV_NO << gpio_shift),
               (AR_GPIO_OE_OUT_DRV << gpio_shift));

#endif
    return AH_TRUE;
}

/*
 * Once configured for I/O - set output lines
 */
HAL_BOOL
ar9300GpioSet(struct ath_hal *ah, u_int32_t gpio, u_int32_t val)
{
    HALASSERT(gpio < AH_PRIVATE(ah)->ah_caps.halNumGpioPins);

    OS_REG_RMW(ah, AR_HOSTIF_REG(ah, AR_GPIO_OUT), ((val&1) << gpio), AR_GPIO_BIT(gpio));

    return AH_TRUE;
}

/*
 * Once configured for I/O - get input lines
 */
u_int32_t
ar9300GpioGet(struct ath_hal *ah, u_int32_t gpio)
{
    u_int32_t gpio_in;
    HALASSERT(gpio < AH_PRIVATE(ah)->ah_caps.halNumGpioPins);

    if (gpio >= AH_PRIVATE(ah)->ah_caps.halNumGpioPins) {
        return 0xffffffff;
    }

    gpio_in = OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_GPIO_IN));
    return (MS(gpio_in, AR_GPIO_IN_VAL) & AR_GPIO_BIT(gpio)) != 0;
}

/*
 * Set the GPIO Interrupt
 * Sync and Async interrupts are both set/cleared.
 * Async GPIO interrupts may not be raised when the chip is put to sleep.
 */
void
ar9300GpioSetIntr(struct ath_hal *ah, u_int gpio, u_int32_t ilevel)
{
#ifndef AR9340_EMULATION
    struct ath_hal_9300 *ahp = AH9300(ah);
    u_int32_t val, mask;


    HALASSERT(gpio < AH_PRIVATE(ah)->ah_caps.halNumGpioPins);


    if (ilevel == HAL_GPIO_INTR_DISABLE) {
        val = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_INTR_ASYNC_ENABLE)),
            AR_INTR_ASYNC_ENABLE_GPIO);
        val &= ~AR_GPIO_BIT(gpio);
        OS_REG_RMW_FIELD(
            ah, AR_HOSTIF_REG(ah, AR_INTR_ASYNC_ENABLE), AR_INTR_ASYNC_ENABLE_GPIO, val);

        mask = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_INTR_ASYNC_MASK)), AR_INTR_ASYNC_MASK_GPIO);
        mask &= ~AR_GPIO_BIT(gpio);
        OS_REG_RMW_FIELD(ah, AR_HOSTIF_REG(ah, AR_INTR_ASYNC_MASK), AR_INTR_ASYNC_MASK_GPIO, mask);

        /*
         * Clear synchronous GPIO interrupt registers and pending
         * interrupt flag
         */
        val = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_ENABLE)),
            AR_INTR_SYNC_ENABLE_GPIO);
        val &= ~AR_GPIO_BIT(gpio);
        OS_REG_RMW_FIELD(
            ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_ENABLE), AR_INTR_SYNC_ENABLE_GPIO, val);

        mask = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_MASK)), AR_INTR_SYNC_MASK_GPIO);
        mask &= ~AR_GPIO_BIT(gpio);
        OS_REG_RMW_FIELD(ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_MASK), AR_INTR_SYNC_MASK_GPIO, mask);

        val = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_CAUSE)), AR_INTR_SYNC_ENABLE_GPIO);
        val |= AR_GPIO_BIT(gpio);
        OS_REG_RMW_FIELD(ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_CAUSE), AR_INTR_SYNC_ENABLE_GPIO, val);
    } else {
        val = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_GPIO_INTR_POL)), AR_GPIO_INTR_POL_VAL);
        if (ilevel == HAL_GPIO_INTR_HIGH) {
            /* 0 == interrupt on pin high */
            val &= ~AR_GPIO_BIT(gpio);
        } else if (ilevel == HAL_GPIO_INTR_LOW) {
            /* 1 == interrupt on pin low */
            val |= AR_GPIO_BIT(gpio);
        }
        OS_REG_RMW_FIELD(ah, AR_HOSTIF_REG(ah, AR_GPIO_INTR_POL), AR_GPIO_INTR_POL_VAL, val);

        /* Change the interrupt mask. */
        val = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_INTR_ASYNC_ENABLE)),
            AR_INTR_ASYNC_ENABLE_GPIO);
        val |= AR_GPIO_BIT(gpio);
        OS_REG_RMW_FIELD(
            ah, AR_HOSTIF_REG(ah, AR_INTR_ASYNC_ENABLE), AR_INTR_ASYNC_ENABLE_GPIO, val);

        mask = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_INTR_ASYNC_MASK)), AR_INTR_ASYNC_MASK_GPIO);
        mask |= AR_GPIO_BIT(gpio);
        OS_REG_RMW_FIELD(ah, AR_HOSTIF_REG(ah, AR_INTR_ASYNC_MASK), AR_INTR_ASYNC_MASK_GPIO, mask);

        /* Set synchronous GPIO interrupt registers as well */
        val = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_ENABLE)),
            AR_INTR_SYNC_ENABLE_GPIO);
        val |= AR_GPIO_BIT(gpio);
        OS_REG_RMW_FIELD(
            ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_ENABLE), AR_INTR_SYNC_ENABLE_GPIO, val);

        mask = MS(OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_MASK)), AR_INTR_SYNC_MASK_GPIO);
        mask |= AR_GPIO_BIT(gpio);
        OS_REG_RMW_FIELD(ah, AR_HOSTIF_REG(ah, AR_INTR_SYNC_MASK), AR_INTR_SYNC_MASK_GPIO, mask);
    }

    /* Save GPIO interrupt mask */
    HALASSERT(mask == val);
    ahp->ah_gpioMask = mask;
#endif
}

#endif  /* AH_SUPPORT_AR9300 */
