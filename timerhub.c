/*************************************************************
 *
 * Copyright 2016 (c) 丰唐物联技术（深圳）有限公司
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


/*******************************************************************************
* Function Name: TimerCreate
********************************************************************************
*
* Summary:
*  Register a function that is called when the specified time has elapsed.
*
* Parameters:
*  pfuncTask:  Period call function poinit.
*  timerTicks: Set the period time.
*              (the unit time decide by call TimerTickPoll() interval time)
*
* Return:
*  Timer handle.
*
* Global Variables:
*  taskList[] - used to store the timer set.
*  taskIndex  - count and index the task handle.
*
* Side Effects: 
*  None.
*
*******************************************************************************/
int8_t TimerCreate(void(*pfuncTask)(void), uint16_t timerTicks)
{
	taskList[taskIndex].lock = 0;
	taskList[taskIndex].period = timerTicks;   //interval time
	taskList[taskIndex].pfuncTask = pfuncTask; //new task func point
	
	if(++taskIndex <= MAX_TASK)
		return (taskIndex-1);     //return the task handle
	else
		return -1;
}

/*******************************************************************************
* Function Name: TimerSuspend
********************************************************************************
*
* Summary:
*  Suspend the Timer by handle.
*
* Parameters:
*  handle: Timer handle.
*
* Global Variables:
*  taskList[].lock - The timer task suspend flag.
*
*******************************************************************************/
void TimerSuspend(uint8_t handle)
{
	taskList[handle].lock = 1;
}

/*******************************************************************************
* Function Name: TimerResume
********************************************************************************
*
* Summary:
*  Resume the Timer by handle.
*
* Parameters:
*  handle: Timer handle.
*
* Global Variables:
*  taskList[].lock - The timer task suspend flag.
*
*******************************************************************************/
void TimerResume(uint8_t handle)
{
	taskList[handle].lock = 0;
}

/*******************************************************************************
* Function Name: TimerTickPoll
********************************************************************************
*
* Summary:
*  looping execution this function in the hardwave timer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  taskList[] - check the ticks count and execution the task.
*  taskIndex  - index the task.
*
* Side Effects: 
*  None.
*
*******************************************************************************/
void TimerTickPoll()
{
	static uint32_t ticks = 0;
	uint8_t index;
	
	ticks++;
	for(index=0; index<taskIndex; index++)
	{
		if(ticks % taskList[index].period == 0)	//check timeout
		{
			if(taskList[index].lock)		          //check the suspend flag
				taskList[index].pfuncTask();        //run the task
		}
	}
}



//