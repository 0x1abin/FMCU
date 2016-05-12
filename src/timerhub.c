/* USER CODE BEGIN Includes */
#include "timerhub.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
volatile static sTask    tasksList[SCH_MAX_TASKS]; /*建立的任务数*/
volatile static uint32_t timerTicks = 0;

/* Public  variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/*******************************************************************************************
* 函 数 名: TimerCreate
* 功能说明: 添加任务。
* 形 参：   void (*pFuntion)(void) uint16_t DELAY uint16_t PERIOD
* 返 回 值: 返回任务的 ID 号
*******************************************************************************************/
uint8_t TimerCreate(void (*pFuntion)(void),
                    uint16_t delay,
                    uint16_t period)
{
	uint8_t index = 0; /*首先在队列中找到一个空隙，（如果有的话） */
	
	while((tasksList[index].pTask != 0) && (index <SCH_MAX_TASKS))
	{
		index++;
	}
	
	if(index == SCH_MAX_TASKS)/*超过最大的任务数目 则返错误信息*/
	{
		return SCH_MAX_TASKS;
	}
	
	tasksList[index].pTask  = pFuntion; /*运行到这里说明申请的任务块成功*/
	tasksList[index].delay  = delay;
	tasksList[index].period = period;
	tasksList[index].runMe  = 0;
	return index; /*返回任务的位置，以便于以后删除*/
}

/*******************************************************************************************
* 函 数 名: TimerDelete
* 功能说明: 删除任务。
* 形 参：   任务handle。
* 返 回 值: 返回任务的 ID 号
*******************************************************************************************/
void TimerDelete(uint8_t index)
{
	tasksList[index].pTask  = 0;
	tasksList[index].delay  = 0;
	tasksList[index].period = 0;
	tasksList[index].runMe  = 0;
}

/*******************************************************************************************
* 函 数 名: TimerUpdate(void)
* 功能说明: 调度器的刷新函数，每个时标中断执行一次。在嘀嗒定时器中断里面执行。
*           当刷新函数确定某个任务要执行的时候，将 runMe 加 1，要注意的是刷新任务
*           不执行任何函数，需要运行的任务有调度函数激活。
* 形 参：   无
* 返 回 值: 无
*******************************************************************************************/
void TimerUpdate(void)
{
	uint8_t index;
	
	/*滴答定时器，提供延时函数基准*/
	timerTicks++;
	
	/*注意计数单位是时标，不是一定毫秒*/
	for(index = 0; index < SCH_MAX_TASKS; index++)
	{
		/*检测这里是否有任务*/
		if(tasksList[index].pTask)
		{
			if(tasksList[index].delay == 0)
			{
				/*任务需要运行 将 runMe 置 1*/
				tasksList[index].runMe++;
				if(tasksList[index].period)
				{
					/*调度周期性的任务再次执行*/
					tasksList[index].delay = tasksList[index].period;
				}
			}
			else
			{
				/*还有准备好运行*/
				tasksList[index].delay--;
			}
		}
	}
}

/*******************************************************************************************
* 函 数 名: TimerTasksExecute
* 功能说明: 在主任务里面执行的调度函数。
* 形 参：   无
* 返 回 值: 无
*******************************************************************************************/
void TimerTasksExecute(void)
{
	uint8_t index;
	/*运行下一个任务，如果下一个任务准备就绪的话*/
	for(index = 0; index < SCH_MAX_TASKS; index++)
	{
		if(tasksList[index].runMe >0)
		{
			/*执行任务 */
			(*tasksList[index].pTask)();
			/* 执行任务完成后，将 runMe 减一 */
			tasksList[index].runMe--;
			/*如果是单次任务的话，则将任务删除 */
			if(tasksList[index].period == 0)
			{
				TimerDelete(index);
			}
		}
	}
}

/*******************************************************************************************
* 函 数 名: TimerDelay
* 功能说明: 定时器延时函数。
* 形 参：   ms
* 返 回 值: 无
*******************************************************************************************/
void TimerDelay(uint16_t tms)
{
	uint32_t setTime = timerTicks + tms;
	while(setTime-timerTicks > 0){}
}

