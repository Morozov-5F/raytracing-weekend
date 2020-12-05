/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_AABB_H
#define RAY_TRACING_ONE_WEEK_RT_AABB_H

#include <stdbool.h>
#include "rt_weekend.h"

typedef struct rt_aabb_s
{
    point3_t min;
    point3_t max;
} rt_aabb_t;

rt_aabb_t rt_aabb(point3_t a, point3_t b);

bool rt_aabb_hit(const rt_aabb_t *aabb, double t_min, double t_max, const ray_t *ray);

rt_aabb_t rt_aabb_surrounding_bb(rt_aabb_t a, rt_aabb_t b);

#endif // RAY_TRACING_ONE_WEEK_RT_AABB_H
