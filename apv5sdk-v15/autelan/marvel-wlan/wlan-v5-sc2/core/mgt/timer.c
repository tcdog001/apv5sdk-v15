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
*   Description:  This file implemented a Timer list object 
*                 functionality
*                                          
*/


/*!
* \file    timer.c
* \brief   software timer module
*/
#include "wltypes.h"
#include "osif.h"
#include "timer.h"

#include "wldebug.h"

void TimerRemove(Timer *me);

/*!
* Initialize a timer
* @param me Pointer to the timer
*/
void TimerInit(Timer *me)
{
	if(timer_pending(me))
		return;
	init_timer(me);
	me->function = NULL;
}

/*************************************************************************
* Function:
*
* Description:
*
* Input:
*
* Output:
*
**************************************************************************/
static void TimerAdd(Timer *me, int act, void *callback, unsigned char *data_p, unsigned int ticks)
{
	me->function = callback;
	me->data = (UINT32) data_p;
	me->expires=jiffies + ticks * TIMER_100MS;    
	if(timer_pending(me))
		return;
	add_timer(me);
}

/*!
* Start a one shot timer 
* @param me Pointer to the timer
* @param act none-zero to active the timer
* @param callback Pointer to the callback function, which will be called if the timer expired
* @param data_p Pointer to user defined data, which is useful by user when implementing callback function
* @param ticks Timer alarm in ticks
*/
void TimerFireIn(Timer *me, int act, void *callback, unsigned char *data_p, unsigned int ticks)
{
	if (act)
	{
		/* Remove in case Timer is already added */
		TimerRemove(me);
		TimerAdd(me, act, callback, data_p, ticks);
	}
}

/*!
* Start a periodic timer 
* @param me Pointer to the timer
* @param act none-zero to active the timer
* @param callback Pointer to the callback function, which will be called if the timer expired
* @param data_p Pointer to user defined data, which is useful by user when implementing callback function
* @param ticks Timer alarm in ticks
*/
void TimerFireEvery(Timer *me, int act, void *callback, unsigned char *data_p, unsigned int ticks)
{
	/* This is not used in current code.  In order to trigger a callback periodically 
	the user will need to rearm the timer within the callback.  */
	if (act)
	{
		TimerRemove(me);
		TimerAdd(me, act, callback, data_p, ticks);
	}
}

/*!
* Stop a timer 
* @param me Pointer to the timer
*/
void TimerDisarm(Timer *me)
{
	if(me->function)
	{
		if(timer_pending(me))
		{
			del_timer(me);
			me->function = NULL;
		}
	}
}

/*!
* Remove a timer 
* @param me Pointer to the timer
*/
void TimerRemove(Timer *me)
{
	if(me->function)
	{
		if(timer_pending(me))
		{
			del_timer(me);
			me->function = NULL;
		}
	}
}

/*!
* Restart a timer 
* @param me Pointer to the timer
* @param ticks Timer alarm in ticks
*/
void TimerRearm(Timer *me, unsigned int ticks)
{
	if (me->function)
	{
		me->expires=jiffies + ticks * TIMER_100MS;    
		if(timer_pending(me))
			return;
		add_timer(me);
	}
}

