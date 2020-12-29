/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_THREAD_POOL_H
#define RAY_TRACING_ONE_WEEK_RT_THREAD_POOL_H

#include <stddef.h>

typedef void (*rt_thread_pool_work_t)(void *params);

typedef void (*rt_thread_pool_work_completion_cb_t)(int status, void *params);

typedef struct rt_thread_pool_s rt_thread_pool_t;

rt_thread_pool_t *rt_tp_init(size_t number_of_threads);

void rt_tp_deinit(rt_thread_pool_t *thread_pool);

int rt_tp_schedule_work(rt_thread_pool_t *thread_pool, rt_thread_pool_work_t work, void *params,
                        rt_thread_pool_work_completion_cb_t completion_cb);

#endif // RAY_TRACING_ONE_WEEK_RT_THREAD_POOL_H
