#include "uv.h"

//uv_loop_t loop;

static uv_loop_t default_loop_struct;
static uv_loop_t* default_loop_ptr;


uv_loop_t* uv_default_loop(void) {
  if (default_loop_ptr != NULL)
    return default_loop_ptr;

  if (uv_loop_init(&default_loop_struct))
    return NULL;

  default_loop_ptr = &default_loop_struct;
  return default_loop_ptr;
}

int uv_loop_init(uv_loop_t* loop) 
{
  // int err;
  
  memset(loop, 0, sizeof(*loop));
  
//	loop->timer = NULL;
//	loop->timer_counter = 0;
//	loop->time_base = 0;

  return 0;
}

void uv__update_time(uv_loop_t* loop)
{
//  static uint16_t count;
  
//  if(++count >= 100)
  {
//    count = 0;
	  loop->time_base++;
	  usleep(1000);
  }

}

int uv_run(uv_loop_t* loop) {
	
  while (1) {
    uv__update_time(loop);
    uv__run_timers(loop);
    uv__run_watch(loop);
//    uv__run_idle(loop);
//    uv__run_prepare(loop);
  }
}
