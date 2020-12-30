/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "rt_sync.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

int rt_sync_get_number_of_cores(void)
{
    return (int)sysconf(_SC_NPROCESSORS_ONLN);
}

struct rt_mutex_s
{
    pthread_mutex_t mutex;
};

rt_mutex_t *rt_mutex_init(void)
{
    rt_mutex_t *res = calloc(1, sizeof(rt_mutex_t));
    assert(NULL != res);

    int rc = pthread_mutex_init(&res->mutex, NULL);
    assert(0 == rc);

    return res;
}

int rt_mutex_lock(rt_mutex_t *mutex)
{
    assert(NULL != mutex);

    return pthread_mutex_lock(&mutex->mutex);
}

int rt_mutex_unlock(rt_mutex_t *mutex)
{
    assert(NULL != mutex);

    return pthread_mutex_unlock(&mutex->mutex);
}

void rt_mutex_deinit(rt_mutex_t *mutex)
{
    if (NULL != mutex)
    {
        pthread_mutex_destroy(&mutex->mutex);
        free(mutex);
    }
}

struct rt_cond_s
{
    pthread_cond_t cond;
};

rt_cond_t *rt_cond_init(void)
{
    rt_cond_t *new_cond = malloc(sizeof(rt_cond_t));
    assert(NULL != new_cond);

    int rc = pthread_cond_init(&new_cond->cond, NULL);
    assert(0 == rc);

    return new_cond;
}

int rt_cond_wait(rt_cond_t *cond, rt_mutex_t *mutex)
{
    assert(NULL != cond);
    assert(NULL != mutex);

    return pthread_cond_wait(&cond->cond, &mutex->mutex);
}

int rt_cond_signal(rt_cond_t *cond)
{
    assert(NULL != cond);

    return pthread_cond_signal(&cond->cond);
}

void rt_cond_deinit(rt_cond_t *cond)
{
    if (NULL != cond)
    {
        pthread_cond_destroy(&cond->cond);
        free(cond);
    }
}