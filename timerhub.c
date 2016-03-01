#include timerhub.h

static uint8_t pTask = 0;
static uint16_t runTimer[MAX_TASK];
static void (*pfunc_task[MAX_TASK])(void);

/*================================   TimerStart   ============================
**    Register a function that is called when the specified time has elapsed.
**    The function is called "repeats" times before the timer is stopped.
**    IN  Timeout function address  
**    IN  Timeout value (for Schedule tick)
**--------------------------------------------------------------------------*/
uint8_t TimerStart(uint16_t timerTicks, void(*pfunc)())
{
    runTimer[pTask] = timerTicks;	//interval time
    pfunc_task[pTask] = pfunc;		//new task func point
    pTask++;						//add task number
    
    return (pTask-1);
}

/*==========================   SchedulePoll   ============================
**  looping execution this function in the hardwave timer
**--------------------------------------------------------------------------*/
void SchedulerPoll()
{
    uint8_t i;
    static uint32_t tick;
    tick++;
	
    for(i=0; i<pTask; i++)
    {
        if(tick % runTimer[i] == 0)	//check timeout
            (*pfunc_task[i])();		//run the task
    }
}
/*---------------------------------------------------------------------------*/
