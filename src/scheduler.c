/* USER CODE BEGIN Includes */
#include "scheduler.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
volatile static sTask SCH_task_G[SCH_MAX_TASKS]; /*建立的任务数*/
volatile static uint32_t timerTicks = 0;

/* Public  variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/*******************************************************************************************
* 函 数 名: SCH_Add_Task
* 功能说明: 添加任务。
* 形 参：   void (*pFuntion)(void) uint16_t DELAY uint16_t PERIOD
* 返 回 值: 返回任务的 ID 号
*******************************************************************************************/
uint8_t SCH_Add_Task(void (*pFuntion)(void),
                     uint16_t DELAY,
                     uint16_t PERIOD)
{
	uint8_t index = 0; /*首先在队列中找到一个空隙，（如果有的话） */
	while((SCH_task_G[index].pTask != 0) && (index <SCH_MAX_TASKS))
	{
		index ++;
	}
	if(index == SCH_MAX_TASKS)/*超过最大的任务数目 则返错误信息*/
	{
		return SCH_MAX_TASKS;
	}
	SCH_task_G[index].pTask = pFuntion; /*运行到这里说明申请的任务块成功*/
	SCH_task_G[index].Delay = DELAY;
	SCH_task_G[index].Period = PERIOD;
	SCH_task_G[index].RunMe =0;
	return index; /*返回任务的位置，以便于以后删除*/
}

/*******************************************************************************************
* 函 数 名: SCH_Add_Task
* 功能说明: 添加任务。
* 形 参：   void (*pFuntion)(void) uint16_t DELAY uint16_t PERIOD
* 返 回 值: 返回任务的 ID 号
*******************************************************************************************/
void SCH_Delete_Task(uint8_t index)
{
	SCH_task_G[index].pTask = 0;
	SCH_task_G[index].Delay = 0;
	SCH_task_G[index].Period = 0;
	SCH_task_G[index].RunMe = 0;
}

/*******************************************************************************************
* 函 数 名: SCH_Update(void)
* 功能说明: 调度器的刷新函数，每个时标中断执行一次。在嘀嗒定时器中断里面执行。
*           当刷新函数确定某个任务要执行的时候，将 RunMe 加 1，要注意的是刷新任务
*           不执行任何函数，需要运行的任务有调度函数激活。
* 形 参：   无
* 返 回 值: 无
*******************************************************************************************/
void SCH_Update(void)
{
	uint8_t index;
	
	/*滴答定时器，提供延时函数基准*/
	timerTicks++;
	
	/*注意计数单位是时标，不是毫秒*/
	for(index = 0; index < SCH_MAX_TASKS; index++)
	{
		/*检测这里是否有任务*/
		if(SCH_task_G[index].pTask)
		{
			if(SCH_task_G[index].Delay == 0)
			{
				/*任务需要运行 将 RunMe 置 1*/
				SCH_task_G[index].RunMe++;
				if(SCH_task_G[index].Period)
				{
					/*调度周期性的任务再次执行*/
					SCH_task_G[index].Delay = SCH_task_G[index].Period;
				}
			}
			else
			{
				/*还有准备好运行*/
				SCH_task_G[index].Delay--;
			}
		}
	}
}

/*******************************************************************************************
* 函 数 名: SCH_Dispatch_Tasks
* 功能说明: 在主任务里面执行的调度函数。
* 形 参：   无
* 返 回 值: 无
*******************************************************************************************/
void SCH_Dispatch_Tasks(void)
{
	uint8_t index;
	/*运行下一个任务，如果下一个任务准备就绪的话*/
	for(index = 0; index < SCH_MAX_TASKS; index++)
	{
		if(SCH_task_G[index].RunMe >0)
		{
			/*执行任务 */
			(*SCH_task_G[index].pTask)();
			/* 执行任务完成后，将 RunMe 减一 */
			SCH_task_G[index].RunMe -= 1;
			/*如果是单次任务的话，则将任务删除 */
			if(SCH_task_G[index].Period == 0)
			{
				SCH_Delete_Task(index);
			}
		}
	}
}

/*******************************************************************************************
* 函 数 名: HAL_Delay
* 功能说明: 定时器延时函数。
* 形 参：   ms
* 返 回 值: 无
*******************************************************************************************/
void HAL_Delay(uint16_t tms)
{
	uint32 setTime = timerTicks + tms;
	while(setTime-timerTicks > 0){}
}

