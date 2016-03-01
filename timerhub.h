#ifndef _TIMERHUB_H_
#define _TIMERHUB_H_

#define MAX_TASK    5

extern int8_t TimerCreate(uint16_t timerTicks, void(*pfunc)());
extern void TimerSuspend(uint8_t handle);
extern void TimerResume(uint8_t handle);
extern void TimerTickPoll();

#endif