#include "scheduler.h"


void setup()
{
    
}

void loop()
{
    
}


//-----------------------------------------------

static void  _init()
{
    TimerInit();
}

int main()
{
    _init();
    
    setup();
    
    while(1) {
        loop();
        eventloop();
        TimerTasksDispatch();
    }
    
    return 0;
}