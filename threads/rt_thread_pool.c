/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_thread_pool.h"
#include "rt_sync.h"
#include <rt_thread.h>

#include <stdlib.h>
#include <assert.h>

#include <utils/rt_queue.h>

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

    RT_QUEUE_ENTRY(rt_tp_work_item_s) next;
} rt_tp_work_item_t;

typedef RT_QUEUE_HEAD(rt_tp_work_item_queue_s, rt_tp_work_item_s) rt_tp_work_item_queue_t;

struct rt_thread_pool_s
{
    rt_thread_t **threads;
    size_t number_of_threads;

    rt_tp_work_item_queue_t work_queue;
    rt_cond_t *work_signal;
    rt_mutex_t *work_mutex;
};

static void thread_func(void *params);
static int enqueue_work(rt_thread_pool_t *thread_pool, rt_tp_work_type_t type, rt_thread_pool_work_t work,
                        void *work_args, rt_thread_pool_work_completion_cb_t completion_cb);

rt_thread_pool_t *rt_tp_init(size_t number_of_threads)
{
    rt_thread_pool_t *result = calloc(1, sizeof(rt_thread_pool_t));
    assert(NULL != result);

    result->threads = calloc(number_of_threads, sizeof(rt_thread_t *));
    assert(NULL != result->threads);
    result->number_of_threads = number_of_threads;

    result->work_signal = rt_cond_init();
    assert(NULL != result->work_signal);

    result->work_mutex = rt_mutex_init();
    assert(NULL != result->work_mutex);

    RT_QUEUE_INIT(&result->work_queue);

    for (int i = 0; i < number_of_threads; ++i)
    {
        result->threads[i] = rt_thread_create(thread_func, result);
        assert(NULL != result->threads[i]);
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
        rt_thread_join(thread_pool->threads[i]);
    }

    while (!RT_QUEUE_EMPTY(&thread_pool->work_queue))
    {
        rt_tp_work_item_t *item = RT_QUEUE_FIRST(&thread_pool->work_queue);
        if (NULL != item->completion_cb)
        {
            item->completion_cb(1, item->work_args);
        }
        RT_QUEUE_DEQUEUE(&thread_pool->work_queue, next);
        free(item);
    }

    rt_mutex_deinit(thread_pool->work_mutex);
    rt_cond_deinit(thread_pool->work_signal);

    free(thread_pool);
}

int rt_tp_schedule_work(rt_thread_pool_t *thread_pool, rt_thread_pool_work_t work, void *params,
                        rt_thread_pool_work_completion_cb_t completion_cb)
{
    return enqueue_work(thread_pool, RT_TP_WORK_TYPE_REGULAR, work, params, completion_cb);
}

static void thread_func(void *params)
{
    rt_thread_pool_t *pool = params;
    assert(NULL != pool);

    while (true)
    {
        rt_mutex_lock(pool->work_mutex);

        while (RT_QUEUE_EMPTY(&pool->work_queue))
        {
            int rc = rt_cond_wait(pool->work_signal, pool->work_mutex);
            assert(0 == rc);
        }

        rt_tp_work_item_t *new_work_item = RT_QUEUE_FIRST(&pool->work_queue);
        assert(NULL != new_work_item);

        RT_QUEUE_DEQUEUE(&pool->work_queue, next);

        rt_mutex_unlock(pool->work_mutex);

        rt_tp_work_item_t copy = *new_work_item;
        free(new_work_item);

        if (RT_TP_WORK_TYPE_STOP == copy.type)
        {
            return;
        }

        copy.work(copy.work_args);
        copy.completion_cb(0, copy.work_args);
    }
}

static int enqueue_work(rt_thread_pool_t *thread_pool, rt_tp_work_type_t type, rt_thread_pool_work_t work,
                        void *work_args, rt_thread_pool_work_completion_cb_t completion_cb)
{
    rt_mutex_lock(thread_pool->work_mutex);

    rt_tp_work_item_t *new_item = calloc(1, sizeof(rt_tp_work_item_t));
    assert(NULL != new_item);

    new_item->type = type;
    new_item->completion_cb = completion_cb;
    new_item->work = work;
    new_item->work_args = work_args;

    RT_QUEUE_ENQUEUE(&thread_pool->work_queue, new_item, next);

    rt_cond_signal(thread_pool->work_signal);

    rt_mutex_unlock(thread_pool->work_mutex);

    return 0;
}
