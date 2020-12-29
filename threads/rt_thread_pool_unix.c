/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_thread_pool.h"

#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#include <sys/queue.h>
#include <stdbool.h>

typedef enum rt_tp_work_type_e
{
    RT_TP_WORK_TYPE_REGULAR,
    RT_TP_WORK_TYPE_STOP,
} rt_tp_work_type_t;

typedef struct rt_tp_work_item_s
{
    rt_tp_work_type_t type;

    rt_thread_pool_work_t work;
    void *work_args;
    rt_thread_pool_work_completion_cb_t completion_cb;

    TAILQ_ENTRY(rt_tp_work_item_s) next;
} rt_tp_work_item_t;

typedef TAILQ_HEAD(rt_tp_work_item_queue_s, rt_tp_work_item_s) rt_tp_work_item_queue_t;

struct rt_thread_pool_s
{
    pthread_t *threads;
    size_t number_of_threads;

    rt_tp_work_item_queue_t work_queue;
    pthread_cond_t work_signal;
    pthread_mutex_t work_mutex;
};

static void *thread_func(void *params);
static int enqueue_work(rt_thread_pool_t *thread_pool, rt_tp_work_type_t type, rt_thread_pool_work_t work,
                        void *work_args, rt_thread_pool_work_completion_cb_t completion_cb);

rt_thread_pool_t *rt_tp_init(size_t number_of_threads)
{
    rt_thread_pool_t *result = calloc(1, sizeof(rt_thread_pool_t));
    assert(NULL != result);

    result->threads = calloc(number_of_threads, sizeof(pthread_t));
    assert(NULL != result->threads);
    result->number_of_threads = number_of_threads;

    int rc = pthread_cond_init(&result->work_signal, NULL);
    assert(0 == rc);
    rc = pthread_mutex_init(&result->work_mutex, NULL);
    assert(0 == rc);

    TAILQ_INIT(&result->work_queue);

    for (int i = 0; i < number_of_threads; ++i)
    {
        rc = pthread_create(result->threads + i, NULL, thread_func, result);
        assert(0 == rc);
    }

    return result;
}

void rt_tp_deinit(rt_thread_pool_t *thread_pool)
{
    for (int i = 0; i < thread_pool->number_of_threads; ++i)
    {
        enqueue_work(thread_pool, RT_TP_WORK_TYPE_STOP, NULL, NULL, NULL);
    }
    for (int i = 0; i < thread_pool->number_of_threads; ++i)
    {
        pthread_join(thread_pool->threads[i], NULL);
    }

    while (!TAILQ_EMPTY(&thread_pool->work_queue))
    {
        rt_tp_work_item_t *item = TAILQ_FIRST(&thread_pool->work_queue);
        if (NULL != item->completion_cb)
        {
            item->completion_cb(1, item->work_args);
        }
        TAILQ_REMOVE(&thread_pool->work_queue, item, next);
        free(item);
    }

    free(thread_pool);
}

int rt_tp_schedule_work(rt_thread_pool_t *thread_pool, rt_thread_pool_work_t work, void *params,
                        rt_thread_pool_work_completion_cb_t completion_cb)
{
    return enqueue_work(thread_pool, RT_TP_WORK_TYPE_REGULAR, work, params, completion_cb);
}

static void *thread_func(void *params)
{
    rt_thread_pool_t *pool = params;
    assert(NULL != pool);

    while (true)
    {
        pthread_mutex_lock(&pool->work_mutex);

        while (TAILQ_EMPTY(&pool->work_queue))
        {
            int rc = pthread_cond_wait(&pool->work_signal, &pool->work_mutex);
            assert(0 == rc);
        }

        rt_tp_work_item_t *new_work_item = TAILQ_FIRST(&pool->work_queue);
        assert(NULL != new_work_item);

        TAILQ_REMOVE(&pool->work_queue, TAILQ_FIRST(&pool->work_queue), next);

        pthread_mutex_unlock(&pool->work_mutex);

        rt_tp_work_item_t copy = *new_work_item;
        free(new_work_item);

        if (RT_TP_WORK_TYPE_STOP == copy.type)
        {
            return NULL;
        }

        copy.work(copy.work_args);
        copy.completion_cb(0, copy.work_args);
    }

    return NULL;
}

static int enqueue_work(rt_thread_pool_t *thread_pool, rt_tp_work_type_t type, rt_thread_pool_work_t work,
                        void *work_args, rt_thread_pool_work_completion_cb_t completion_cb)
{
    pthread_mutex_lock(&thread_pool->work_mutex);

    rt_tp_work_item_t *new_item = calloc(1, sizeof(rt_tp_work_item_t));
    assert(NULL != new_item);

    new_item->type = type;
    new_item->completion_cb = completion_cb;
    new_item->work = work;
    new_item->work_args = work_args;

    TAILQ_INSERT_TAIL(&thread_pool->work_queue, new_item, next);

    pthread_cond_signal(&thread_pool->work_signal);

    pthread_mutex_unlock(&thread_pool->work_mutex);

    return 0;
}
