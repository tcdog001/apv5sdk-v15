/*
*                Copyright 2002-2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

/*
*
*   Description:  This file defined a Timer list object 
*
*/
#ifndef timer_h
#define timer_h

#define CALL_BACK 0
#define NO_CALL_BACK 1

#define TIMER_100MS     HZ/10
#define TIMER_10MS      HZ/100
#define TIMER_1MS       HZ/1000

typedef struct timer_list Timer;
void TimerFireIn(Timer *me, int act, void *callback, unsigned char *data_p, unsigned int ticks);
void TimerFireEvery(Timer *me, int act, void *callback, unsigned char *data_p, unsigned int ticks);
void TimerDisarm(Timer *me);
void TimerRearm(Timer *me, unsigned int ticks);
void TimerInit(Timer *me);
void TimerRemove(Timer *me);


#endif
