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


void uv_timer_init(uv_loop_t* loop, uv_timer_t* handle)
{
//	memset(handle, 0, sizeof(uv_timer_t));
	handle->loop = loop;
}


int uv_timer_start(uv_timer_t* handle,
                   uv_timer_cb cb,
                   uint32_t timeout,
                   uint32_t repeat)
{

	if (cb == NULL)
		return -EINVAL;

	handle->timer_cb = cb;
	handle->timeout = handle->loop->time_base + timeout;
	handle->repeat = repeat;

	if(handle->loop->timer == NULL)
	{
		INIT_LIST_HEAD(&(handle->list));
		handle->loop->timer = handle;
	}
	else
	{
		list_add(&(handle->list), &(handle->loop->timer->list));
	}

	return 0;
}


int uv_timer_stop(uv_timer_t* handle)
{
	if(&(handle->list) == handle->list.next)
	{
		handle->loop->timer = NULL;
		return 0;
	}

	list_del(&(handle->list));
	
	return 0;
}


int uv_timer_again(uv_timer_t* handle)
{
	if (handle->timer_cb == NULL)
		return -EINVAL;

	if (handle->repeat)
	{
		//handle->loop->timer->timeout = handle->loop->time_base + loop->timer->repeat;
		uv_timer_stop(handle);
    	uv_timer_start(handle, handle->timer_cb, handle->repeat, handle->repeat);
	}

	return 0;
}


void uv_timer_set_repeat(uv_timer_t* handle, uint32_t repeat)
{
	handle->repeat = repeat;
}


uint32_t uv_timer_get_repeat(const uv_timer_t* handle)
{
	return handle->repeat;
}


void uv__run_timers(uv_loop_t* loop)
{
	if(loop->timer != NULL)
	{
		if((loop->time_base) > (loop->timer->timeout))
		{
			loop->timer->timer_cb(loop->timer);
			
			if (loop->timer->repeat)
			{
				loop->timer->timeout = loop->time_base + loop->timer->repeat;
			}
			else
			{
				uv_timer_stop(loop->timer);
			}
		}
		
		loop->timer = container_of((loop->timer->list.next), uv_timer_t, list);
//		loop->timer = loop->timer->prev;
	}
}

