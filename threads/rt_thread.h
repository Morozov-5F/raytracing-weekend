/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_THREAD_H
#define RAY_TRACING_ONE_WEEK_RT_THREAD_H

#if defined(__GNUC__) || defined(__clang__)
#define RT_THREAD_LOCAL __thread
#elif defined(__MSVC__)
#define RT_THREAD_LOCAL __declspec(thread)
#else
#warning "Thread local storage is not supported for this compiler, there might be artifacts in the rendered image in case of multi-threaded rendering"
#endif

typedef struct rt_thread_s rt_thread_t;

typedef void (*rt_thread_fn_t)(void *params);

rt_thread_t *rt_thread_create(rt_thread_fn_t thread_fn, void *arg);

void rt_thread_join(rt_thread_t *thread);

#endif // RAY_TRACING_ONE_WEEK_RT_THREAD_H
