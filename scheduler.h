#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

//typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;
//typedef unsigned long uint32_t;
#define uint8_t 	unsigned char
#define uint16_t 	unsigned short
#define uint32_t 	unsigned long

#define SCH_MAX_TASKS 			5

typedef struct
{
	void (*pTask)();	/*指向任务的指针必须是一个*void(void)*函数；*/
	uint16_t Delay;   /*延时（时标）知道下一个函数的运行*/
	uint16_t Period;  /*连续运行之间的间隔*/
	uint8_t RunMe;    /*当任务需要运行的时候由调度器加 1*/
} sTask;

#ifdef __cplusplus  
extern "C" {  
#endif  
extern uint8_t SCH_Add_Task(void (*pFuntion)(void), uint16_t DELAY, uint16_t PERIOD);							
extern void SCH_Delete_Task(uint8_t index);
extern void SCH_Update(void);
extern void SCH_Dispatch_Tasks(void);

extern void HAL_Delay(uint16_t tms);
#ifdef __cplusplus  
} 
#endif  


#define TimerStart					SCH_Add_Task
#define TimerDelete					SCH_Delete_Task
#define TimerUpdate					SCH_Update
#define TimerTasksExecute		SCH_Dispatch_Tasks


#endif