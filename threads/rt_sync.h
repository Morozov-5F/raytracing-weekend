/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_SYNC_H
#define RAY_TRACING_ONE_WEEK_RT_SYNC_H

int rt_sync_get_number_of_cores(void);



typedef struct rt_mutex_s rt_mutex_t;

rt_mutex_t *rt_mutex_init(void);

int rt_mutex_lock(rt_mutex_t *mutex);

int rt_mutex_unlock(rt_mutex_t *mutex);

void rt_mutex_deinit(rt_mutex_t *mutex);



typedef struct rt_cond_s rt_cond_t;

rt_cond_t *rt_cond_init(void);

int rt_cond_wait(rt_cond_t *cond, rt_mutex_t *mutex);

int rt_cond_signal(rt_cond_t *cond);

void rt_cond_deinit(rt_cond_t *cond);

#endif // RAY_TRACING_ONE_WEEK_RT_SYNC_H
