
void subscribe(char *event, callback)
void publish(char *event)
int watchState(int (*targefunc)(void parm), void *parm, int event, callback)

void ev_timer_init (ev_timer *, callback, ev_tstamp after, ev_tstamp repeat)
void ev_timer_set (ev_timer *, ev_tstamp after, ev_tstamp repeat)

void ev_stat_init (ev_stat *, callback, const char *path, ev_tstamp interval)
void ev_stat_set (ev_stat *, const char *path, ev_tstamp interval)





# define UV__HANDLE_INTERNAL  0x80
# define UV__HANDLE_ACTIVE    0x40
# define UV__HANDLE_REF       0x20
# define UV__HANDLE_CLOSING   0x01


typedef struct uv_timer_s uv_timer_t;

struct uv_timer_s {
//UV_HANDLE_FIELDS
    /* public */                                                                \
    void* data;                                                                 \
    /* read-only */                                                             \
    uv_loop_t* loop;                                                            \
    uv_handle_type type;                                                        \
    /* private */                                                               \
    uv_close_cb close_cb;                                                       \
    void* handle_queue[2];                                                      \
    union {                                                                     \
    int fd;                                                                   \
    void* reserved[4];                                                        \
    } u;
    //UV_HANDLE_PRIVATE_FIELDS                                                    
      uv_handle_t* next_closing;                                                  
      unsigned int flags;    
    
//UV_TIMER_PRIVATE_FIELDS
    uv_timer_cb timer_cb;                                                       
    void* heap_node[3];                                                         
    uint64_t timeout;                                                           
    uint64_t repeat;                                                            
    uint64_t start_id;
};




int uv_timer_init(uv_loop_t* loop, uv_timer_t* handle)
//Initialize the handle.

int uv_timer_start(uv_timer_t* handle, uv_timer_cb cb, uint64_t timeout, uint64_t repeat)
//Start the timer. timeout and repeat are in milliseconds.

//If timeout is zero, the callback fires on the next event loop iteration. If 
//repeat is non-zero, the callback fires first after timeout milliseconds and 
//then repeatedly after repeat milliseconds.

int uv_timer_stop(uv_timer_t* handle)
//Stop the timer, the callback will not be called anymore.

int uv_timer_again(uv_timer_t* handle)
//Stop the timer, and if it is repeating restart it using the repeat value as 
//the timeout. If the timer has never been started before it returns UV_EINVAL.

void uv_timer_set_repeat(uv_timer_t* handle, uint64_t repeat)
//Set the repeat interval value in milliseconds. The timer will be scheduled to 
//run on the given interval, regardless of the callback execution duration, and 
//will follow normal timer semantics in the case of a time-slice overrun.


int uv_timer_start(uv_timer_t* handle,
                   uv_timer_cb cb,
                   uint64_t timeout,
                   uint64_t repeat) {
  uint64_t clamped_timeout;

  if (cb == NULL)
    return -EINVAL;

  if (uv__is_active(handle))
    uv_timer_stop(handle);

  clamped_timeout = handle->loop->time + timeout;
  if (clamped_timeout < timeout)
    clamped_timeout = (uint64_t) -1;

  handle->timer_cb = cb;
  handle->timeout = clamped_timeout;
  handle->repeat = repeat;
  /* start_id is the second index to be compared in uv__timer_cmp() */
  handle->start_id = handle->loop->timer_counter++;

  heap_insert((struct heap*) &handle->loop->timer_heap,
              (struct heap_node*) &handle->heap_node,
              timer_less_than);
  uv__handle_start(handle);

  return 0;
}