#ifndef _TIMERHUB_H_
#define _TIMERHUB_H_

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
	uint16_t delay;   /*延时（时标）知道下一个函数的运行*/
	uint16_t period;  /*连续运行之间的间隔*/
	uint8_t runMe;    /*当任务需要运行的时候由调度器加 1*/
} sTask;

#ifdef __cplusplus  
extern "C" {  
#endif  

extern uint8_t  TimerCreate       (void (*pFuntion)(void), uint16_t delay,  uint16_t period);			
extern void     TimerDelete       (uint8_t index);
extern void     TimerUpdate       (void);
extern void     TimerTasksExecute (void);
extern void     TimerDelay        (uint16_t tms);

#ifdef __cplusplus  
} 
#endif  


#endif
