#ifndef _TIMERHUB_H_
#define _TIMERHUB_H_

#define MAX_TASK    5

extern uint8_t TimerStart(uint16_t timerTicks, void(*pfunc)());
extern void SchedulerPoll();

#endif