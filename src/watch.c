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


void uv_watch_init(uv_loop_t* loop, uv_watch_t* handle)
{
//    memset(handle, 0, sizeof(uv_watch_t));
    handle->loop = loop;
}

int uv_watch_start(uv_watch_t* handle, uv_watch_cb cb, uv_targer_cb target, uint32_t timeout)
{
	if (cb == NULL)
		return -EINVAL;
	
	handle->cb = cb;
	handle->target = target;
	
	if(timeout == 0)
		handle->timeout = 0xffffffff;
	else
		handle->timeout = handle->loop->time_base + timeout;
		
	handle->event = (int)(target());
	handle->prev_event = handle->event;
	handle->loop->watch_counter += 1;
	
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
	if(--(handle->loop->watch_counter) == 0)
	{
		handle->loop->watch = NULL;
		return 0;
	}

	list_del(&(handle->list));
	
	return 1;
}

void uv__run_watch(uv_loop_t* loop)
{
    uint8_t i;
    uv_watch_t* watch;
    struct list_head* plist;
    
    if(loop->watch != NULL)
    {
    	watch = loop->watch;
    	plist = &(watch->list);
    
        for(i=0; i<(loop->watch_counter); i++)
        {
            watch = container_of(plist, uv_watch_t, list);
            
            loop->watch->event = loop->watch->target();
            
            if((loop->watch->event) != (loop->watch->prev_event))
            {
                loop->watch->cb(loop->watch);
            }
            
            loop->watch->prev_event = loop->watch->event;
            plist = plist->next;
            
            if(loop->time_base > watch->timeout)
            {
            	//loop->watch->event = "timeout";
            	//loop->watch->cb(loop->watch);
            	uv_watch_stop(watch);
            }
        }
    }
}