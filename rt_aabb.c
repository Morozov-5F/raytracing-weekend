/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_aabb.h"

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

    for (vec3_axis_t axis = VEC3_AXIS_X; axis <= VEC3_AXIS_Z; axis++)
    {
        double inv_direction = 1.0 / ray->direction.components[axis];
        double t0 = (aabb->min.components[axis] - ray->origin.components[axis]) * inv_direction,
               t1 = (aabb->max.components[axis] - ray->origin.components[axis]) * inv_direction;
        if (inv_direction < 0)
        {
            double tmp = t0;
            t0 = t1;
            t1 = tmp;
        }
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
        {
            return false;
        }
    }

    return true;
}

rt_aabb_t rt_aabb_surrounding_bb(rt_aabb_t a, rt_aabb_t b)
{
    rt_aabb_t result = {.min =
                            {
                                fmin(a.min.x, b.min.x),
                                fmin(a.min.y, b.min.y),
                                fmin(a.min.z, b.min.z),
                            },
                        .max = {
                            fmax(a.max.x, b.max.x),
                            fmax(a.max.y, b.max.y),
                            fmax(a.max.z, b.max.z),
                        }};
    return result;
}
