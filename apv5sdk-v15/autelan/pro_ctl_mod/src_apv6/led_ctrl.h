/*
 * AMCC HLDS ctrl. function header file
 *
 * Copyright 2010 AMCC Software Engineering, Phong Vo <pvo@amcc.com>
 *
 * LED and interrupt handlers for Power/Reset button and over-temperature
 * event.
 *
 */

#ifndef LED_CTRL_H
#define LED_CTRL_H

#include "linux/ioctl.h"
struct hlds_ctrl_t {
	int ctrl;	/* HLDS_LED_CTRL above */
	int device; 	/* Device index, like from HLDS_LED_IDX */
	int reset;
} wrcpu_op_args;

#define WRCPU_IOC_MAGIC 10
#define WRCPU_IOC_V6_  _IOWR(WRCPU_IOC_MAGIC,1,struct hlds_ctrl_t) // read values

enum HLDS_CTRL_CMD {
	HLDS_LED_CMD = 0,
	/* Add your additional commands above here */
	HLDS_CTRL_CMD_MAX
};

/* HLDS LED controls */
enum HLDS_LED_CTRL  { 
	LED_OFF = 0,
	LED_ON,
	LED_BLINK_ONCE,
	LED_BLINK_SLOW,
	LED_BLINK_FAST,
	LED_BLINK_RAPID,
	LED_BLINK_IRQ_SLOW,
	LED_BLINK_IRQ_FAST,
	LED_BLINK_IRQ_RAPID,
	HLDS_LED_CTRL_MAX
};

/* HLDS LED status */
enum HLDS_LED_STATUS  { 
	LED_BLINK_ENABLE = 0,
	LED_BLINK_DISABLE,
	LED_BLINK_CHANGE,
	HLDS_LED_SATUS_MAX
};

/* Control structure, passed into ioctl */


/* Driver reported event passed to client */
enum HLDS_CTRL_EVT {
	NO_EVENT = 0,
	POWER_DOWN,
    POWER_STANDBY,
	BACKUP_BUTTON,
	/* Add any other event here */
	HLDS_CTRL_EVT_MAX	
};

enum HLDS_EVENT_TYPE {
	HLDS_INPUT_EVENT=1,
	HLDS_ALERT_EVENT,
	HLDS_KEYDOWN_EVENT,
	HLDS_KEYUP_EVENT,
	/* Add any other event type here */
	HLDS_EVENT_TYPE_MAX
};

/* Event structure, passed back to client on a read */
struct hlds_ctrl_evt {
	int event;
};

#ifdef CONFIG_HLDS_NAS
#define HLDS_LED_IDX_MAX 4
typedef enum {
	/* LED devices */
	HLDS_SATA0_LED = 0,
	HLDS_SATA1_LED,
	HLDS_USB_LED,
	/* other devices */
	HLDS_IDX_MAX
} hlds_led;

#define PERIPHERAL_MAX	HLDS_IDX_MAX

#define PERIPHERAL_USB_LED_NAME         "usb_led"
#define PERIPHERAL_USB_LED_ACT          1

#define PERIPHERAL_SATA0_LED_NAME       "sata0_led"
#define PERIPHERAL_SATA0_LED_ACT        1

#define PERIPHERAL_SATA1_LED_NAME       "sata1_led"
#define PERIPHERAL_SATA1_LED_ACT        1

#define PERIPHERAL_ENABLE		1

#else // CONFIG_HLDS_NAS

#define PERIPHERAL_MAX 4

#endif // CONFIG_HLDS_NAS

struct peripheral_def
{
	const char	*name;
	int		enable; /* 1: on state, 0: off state */
	int		active_level; /*  0: Active Low  1: Active High */
};

#ifdef CONFIG_HLDS_NAS

extern struct peripheral_def peripherals_list[PERIPHERAL_MAX];
// extern int peripherals_addr[PERIPHERAL_MAX];
#ifdef CONFIG_PM
extern int peripheral_power_management(void);
#endif

#else // CONFIG_HLDS_NAS
//int peripherals_addr[PERIPHERAL_MAX] = {

//};

struct peripheral_def peripherals_list[PERIPHERAL_MAX] = {

};
#ifdef CONFIG_PM
int peripheral_power_management(void){
	return 0;
};
#endif

#endif //CONFIG_HLDS_NAS

void hlds_led_cmd_hdlr(struct hlds_ctrl_t*);

#define LED_CTRL_EVENTS _IOWR(HLDS_CTRL_IOCTL_BASE, 20, struct event_ctrl)

#define HLDS_EVENT    "APM NAS Platform Events"
#define HLDS_DEV_NAME "wrcpu"
#define HLDS_DEV      "/dev/" HLDS_DEV_NAME

#endif
