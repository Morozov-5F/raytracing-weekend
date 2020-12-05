/**
 * Copyright (c) ${$YEAR}, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_aabb.h"

#define INTERSECTION_FOR_COMPONENT(component, aabb, ray, t_min, t_max)                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        double __t0_pre__ = ((aabb)->min.component - (ray)->origin.component) / (ray)->direction.component,            \
               __t1_pre__ = ((aabb)->max.component - (ray)->origin.component) / (ray)->direction.component;            \
        t_min = fmax(fmin(__t0_pre__, __t1_pre__), (t_min));                                                           \
        t_max = fmin(fmax(__t0_pre__, __t1_pre__), (t_max));                                                           \
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
