/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <rt_sync.h>
#include <stddef.h>

#include <Windows.h>
#include <assert.h>

struct rt_mutex_s
{
    CRITICAL_SECTION cs;
};

rt_mutex_t *rt_mutex_init(void)
{
    rt_mutex_t *new_mutex = malloc(sizeof(rt_mutex_t));
    assert(NULL != new_mutex);

    InitializeCriticalSection(&new_mutex->cs);

    return new_mutex;
}

int rt_mutex_lock(rt_mutex_t *mutex)
{
    assert(NULL != mutex);

    EnterCriticalSection(&mutex->cs);

    return 0;
}

int rt_mutex_unlock(rt_mutex_t *mutex)
{
    assert(NULL != mutex);

    LeaveCriticalSection(&mutex->cs);

    return 0;
}

void rt_mutex_deinit(rt_mutex_t *mutex)
{
    if (NULL != mutex)
    {
        DeleteCriticalSection(&mutex->cs);
        free(mutex);
    }
}

int rt_sync_get_number_of_cores(void)
{
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);

    return system_info.dwNumberOfProcessors;
}
