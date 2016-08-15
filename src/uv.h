#ifndef UV_H
#define UV_H

#include "typedef.h"
#include <string.h>
//#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "list.h"


#define EINVAL      0x86

# define UV__HANDLE_INTERNAL  0x80
# define UV__HANDLE_ACTIVE    0x40
# define UV__HANDLE_REF       0x20
# define UV__HANDLE_CLOSING   0x01


/* Handle types. */
typedef struct uv_loop_s        uv_loop_t;
typedef struct uv_handle_s      uv_handle_t;
typedef struct uv_tty_s         uv_tty_t;
typedef struct uv_poll_s        uv_poll_t;
typedef struct uv_timer_s       uv_timer_t;
typedef struct uv_check_s       uv_check_t;
typedef struct uv_idle_s        uv_idle_t;
typedef struct uv_process_s     uv_process_t;
typedef struct uv_fs_event_s    uv_fs_event_t;
typedef struct uv_fs_poll_s     uv_fs_poll_t;
typedef struct uv_signal_s      uv_signal_t;
typedef struct uv_watch_s       uv_watch_t;



typedef int  (*uv_targer_cb)(void);
typedef void (*uv_watch_cb)(uv_watch_t* handle);
typedef void (*uv_timer_cb)(uv_timer_t* handle);

//extern uv_loop_t loop;

struct uv_loop_s {
    uint8_t     flags;        
    
    uv_timer_t* timer;
    uint8_t     timer_counter;
    
    uv_watch_t* watch;
    uint8_t     watch_counter;
    
//    uv_idle_t*  idle;
    
    uint32_t    time_base;
};


struct uv_timer_s {
    /* public */                                                                
//    void*    data;
    /* read-only */
    uv_loop_t* loop; 
//    uint8_t  flags;
    
    uv_timer_cb timer_cb;
    uint32_t timeout;
    uint32_t repeat;
    
    struct list_head list;
};


struct uv_watch_s {
    /* public */                                                                
//    void*    data;
    /* read-only */
    uv_loop_t* loop; 
//    uint8_t  flags;
    
    uv_watch_cb  cb;
    uv_targer_cb target;
    int event;
    int prev_event;
    uint32_t timeout;
    
    struct list_head list;
};


uv_loop_t* uv_default_loop(void);
int uv_loop_init(uv_loop_t* loop);
int uv_run(uv_loop_t* loop);

void uv__update_time(uv_loop_t* loop);
void uv_register_ticks();


void uv_timer_init(uv_loop_t* loop, uv_timer_t* handle);
int uv_timer_start(uv_timer_t* handle, uv_timer_cb cb, uint32_t timeout, uint32_t repeat);
int uv_timer_stop(uv_timer_t* handle);
int uv_timer_again(uv_timer_t* handle);
void uv_timer_set_repeat(uv_timer_t* handle, uint32_t repeat);
uint32_t uv_timer_get_repeat(const uv_timer_t* handle);
void uv__run_timers(uv_loop_t* loop);


void uv_watch_init(uv_loop_t* loop, uv_watch_t* handle);
int uv_watch_start(uv_watch_t* handle, uv_watch_cb cb, uv_targer_cb target, uint32_t timeout);
int uv_watch_stop(uv_watch_t* handle);
void uv__run_watch(uv_loop_t* loop);

#endif /* UV_H */

