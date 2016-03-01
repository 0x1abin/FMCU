#ifndef _TIMERHUB_H_
#define _TIMERHUB_H_

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long
#define int8_t char


#define MAX_TASK    8

typedef struct
{
	uint8_t lock;
	uint8_t timerTicks;
	void (*pfuncTask)(void);
}sTask;


extern int8_t TimerCreate( void(*pfunc)(void), uint16_t timerTicks);
extern void TimerSuspend(uint8_t handle);
extern void TimerResume(uint8_t handle);
extern void TimerTickPoll();



#endif