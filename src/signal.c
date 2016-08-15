/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "uv.h"


struct uv_watch_s {
    /* public */                                                                
    void*    data;
    /* read-only */
    uv_loop_t* loop; 
    uint8_t  flags;
    
    uv_signal_t watch_cb;
    
    int trg_signal;
    
    struct list_head list;
}


void uv_signal_init(uv_loop_t* loop, uv_signal_t* handle);
{
//    memset(handle, 0, sizeof(uv_watch_t));
    handle->loop = loop;
}

int uv_signal_start(uv_signal_t* handle, uv_signal_cb cb, int set_signal)
{
	if (cb == NULL)
		return -EINVAL;
	
	handle->cb = cb;
	handle->target = target;
		
	handle->event = target();
	handle->prev_event = event;
	handle->total_count += 1;
	
	if(handle->loop->watch == NULL)
	{
		handle->loop->watch = handle;
		INIT_LIST_HEAD(&(handle->list));
	}
	else
	{
		list_add(&(handle->list), &(handle->loop->watch->list));
	}
	
	return 0;
}

int uv_watch_stop(uv_watch_t* handle)
{
//	if(&(handle->list) == handle->list.next)
	if(--(handle->watch_counter) == 0)
	{
		handle->loop->watch = NULL;
		return 0;
	}

	list_del(&(handle->list));
}

void uv__run_signal(uv_loop_t* loop)
{
    uv_watch_t* watch;
    struct list_head* plist;
    
    if(loop->watch == NULL)
    {
    	return -EINVAL;
    }
    else
    {
    	watch = loop->watch;
    	plist = watch->list;
    }
    
    for(i=0; i<(loop->watch.total_count); i++)
    {
        watch = container_of(plist, uv_watch_t, list);
        
        loop->watch.event = loop->watch.target();
        
        if((loop->watch.event) != (loop->watch.prev_event))
        {
            loop->watch.cb(loop->watch);
        }
        
        loop->watch.prev_event = loop->watch.event;
        plist = plist->next;
        
        if(loop->time_base > watch->timeout)
        {
        	uv_watch_stop(watch);
        }
    }
}