#ifndef _TIMERHUB_H_
#define _TIMERHUB_H_

/*---------------tmpe type----------------*/
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long
#define int8_t char
#define bool uint8_t
/*----------------------------------------*/

#define MAX_TASK    8

typedef struct _STASK
{
	bool lock;
	uint16_t period;
	void (*pfuncTask)(void);
}sTask;


extern int8_t TimerCreate(void(*pfunc)(void), uint16_t timerTicks);
extern void TimerSuspend(uint8_t handle);
extern void TimerResume(uint8_t handle);
extern void TimerTickPoll();

#endif






//test3