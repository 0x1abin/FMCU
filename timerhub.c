/*************************************************************
 *
 * Copyright 2014 (c) 丰唐物联技术（深圳）有限公司
 * All right reserved.
 *
 * FileName:        timerhub.c
 * Description:     Softwave timer module
 * Author:          Zibin Zheng
 * CreateDate:      2016/3/1
 * CurrentVersion:  V1.00
 *
 *=========================history============================
 *
 * Version:         V1.00
 * Modify:          Zibin Zheng
 * Date:            2016/3/1
 * ChangeItems:     Create
 *
*************************************************************/

#include "timerhub.h"

static sTask taskList[MAX_TASK];
static uint8_t taskIndex = 0;

/*===============================   TimerCreate   ============================
**    Register a function that is called when the specified time has elapsed.
**    RET Timer handle
**    IN  Timeout value (for Schedule tick)
**    IN  Timeout function address  
**--------------------------------------------------------------------------*/
int8_t TimerCreate( void(*pfuncTask)(void), uint16_t timerTicks)
{
	taskList[taskIndex].lock = 0;
	taskList[taskIndex].timerTicks = timerTicks;  //interval time
	taskList[taskIndex].pfuncTask = pfuncTask;    //new task func point
	
	if(++taskIndex <= MAX_TASK)        //task handle
		return (taskIndex-1);
	else
		return -1;
}

/*==========================   TimerSuspend   ============================
**  Suspend the Timer by handle
**-----------------------------------------------------------------------*/
void TimerSuspend(uint8_t handle)
{
	taskList[handle].lock = 1;
}

/*==========================   TimerResume   ============================
**  Resume the Timer by handle
**-----------------------------------------------------------------------*/
void TimerResume(uint8_t handle)
{
	taskList[handle].lock = 0;
}

/*==========================   TimerTickPoll   ============================
**  looping execution this function in the hardwave timer
**-----------------------------------------------------------------------*/
void TimerTickPoll()
{
	static uint32_t ticks = 0;
	uint8_t index;
	
	ticks++;
	for(index=0; index<taskIndex; index++)
	{
		if(ticks % taskList[index].timerTicks == 0)	//check timeout
		{
			if(taskList[index].lock == 0)
				taskList[index].pfuncTask();   //run the task
		}
	}
}
/*---------------------------------------------------------------------------*/
