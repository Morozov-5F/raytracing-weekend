/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <rt_thread.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

struct rt_thread_s
{
    pthread_t thread_handle;
};

rt_thread_t *rt_thread_create(rt_thread_fn_t thread_fn, void *arg)
{
    rt_thread_t *new_thread = malloc(sizeof(rt_thread_t));
    assert(NULL != new_thread);

    union {
        void (*in)(void *);
        void *(*out)(void *);
    } fn = {
        .in = thread_fn,
    };

    int rc = pthread_create(&new_thread->thread_handle, NULL, fn.out, arg);
    assert(0 == rc);

    return new_thread;
}

void rt_thread_join(rt_thread_t *thread)
{
    if (NULL != thread)
    {
        pthread_join(thread->thread_handle, NULL);

        free(thread);
    }
}