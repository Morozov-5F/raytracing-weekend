/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H
#define RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H

#include <stdbool.h>

typedef enum rt_hittable_type_e
{
    RT_HITTABLE_TYPE_UNKNOWN = -1,
    RT_HITTABLE_TYPE_SPHERE = 0,
} rt_hittable_type_t;

struct rt_hittable_s
{
    rt_hittable_type_t type;
};

#endif // RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H
