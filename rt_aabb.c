/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_aabb.h"

#define INTERSECTION_FOR_COMPONENT(component, aabb, ray, t_min, t_max)                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        double inv_direction = 1.0 / (ray)->direction.component;                                                       \
        double __t0__ = ((aabb)->min.component - (ray)->origin.component) * inv_direction,                             \
               __t1__ = ((aabb)->max.component - (ray)->origin.component) * inv_direction;                             \
        if (inv_direction < 0)                                                                                         \
        {                                                                                                              \
            double __buf__ = __t0__;                                                                                   \
            __t0__ = __t1__;                                                                                           \
            __t1__ = __buf__;                                                                                          \
        }                                                                                                              \
        t_min = __t0__ > t_min ? __t0__ : t_min;                                                                       \
        t_max = __t1__ < t_max ? __t1__ : t_max;                                                                       \
        if ((t_max) <= (t_min))                                                                                        \
        {                                                                                                              \
            return false;                                                                                              \
        }                                                                                                              \
    } while (0)

rt_aabb_t rt_aabb(point3_t a, point3_t b)
{
    rt_aabb_t result = {
        .min = a,
        .max = b,
    };
    return result;
}

bool rt_aabb_hit(const rt_aabb_t *aabb, double t_min, double t_max, const ray_t *ray)
{
    assert(NULL != aabb);

    INTERSECTION_FOR_COMPONENT(x, aabb, ray, t_min, t_max);
    INTERSECTION_FOR_COMPONENT(y, aabb, ray, t_min, t_max);
    INTERSECTION_FOR_COMPONENT(z, aabb, ray, t_min, t_max);

    return true;
}
