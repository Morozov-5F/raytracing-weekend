/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <rt_sync.h>
#include <stddef.h>

rt_mutex_t *rt_mutex_init(void)
{
    return NULL;
}

int rt_mutex_lock(rt_mutex_t *mutex)
{
    return 0;
}

int rt_mutex_unlock(rt_mutex_t *mutex)
{
    return 0;
}

void rt_mutex_deinit(rt_mutex_t *mutex)
{
}

int rt_sync_get_number_of_cores(void)
{
    return 0;
}
