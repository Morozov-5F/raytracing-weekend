/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "rt_thread.h"

#include <Windows.h>
#include <process.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

struct rt_thread_s
{
    HANDLE thread_handle;
};

rt_thread_t *rt_thread_create(rt_thread_fn_t thread_fn, void *arg)
{
    rt_thread_t *new_thread = malloc(sizeof(rt_thread_t));
    assert(NULL != new_thread);

    new_thread->thread_handle = (HANDLE)_beginthread(thread_fn, 0, arg);
    assert(NULL != new_thread->thread_handle);

    return new_thread;
}

void rt_thread_join(rt_thread_t *thread)
{
    if (NULL == thread)
    {
        return;
    }

    DWORD res = WaitForSingleObject(thread->thread_handle, INFINITE);
    assert(0 == res);

    CloseHandle(thread->thread_handle);
    MemoryBarrier();

    free(thread);
}
