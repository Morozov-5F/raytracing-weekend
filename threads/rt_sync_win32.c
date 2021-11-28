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
#include <malloc.h>

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

struct rt_cond_s
{
    CONDITION_VARIABLE cond;
};

rt_cond_t *rt_cond_init(void)
{
    rt_cond_t *new_cond = malloc(sizeof(rt_cond_t));
    assert(NULL != new_cond);

    InitializeConditionVariable(&new_cond->cond);

    return new_cond;
}

int rt_cond_wait(rt_cond_t *cond, rt_mutex_t *mutex)
{
    assert(NULL != cond);
    assert(NULL != mutex);

    SleepConditionVariableCS(&cond->cond, &mutex->cs, INFINITE);

    return 0;
}

int rt_cond_signal(rt_cond_t *cond)
{
    assert(NULL != cond);

    WakeConditionVariable(&cond->cond);

    return 0;
}

void rt_cond_deinit(rt_cond_t *cond)
{
    free(cond);
}
