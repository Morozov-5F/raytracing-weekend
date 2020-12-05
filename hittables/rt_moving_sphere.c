/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_moving_sphere.h"
#include "rt_hittable_shared.h"
#include "rt_sphere.h"

struct rt_moving_sphere_s
{
    rt_hittable_t base;

    point3_t center_start;
    point3_t center_end;

    double time_start;
    double time_end;

    double radius;
    rt_material_t *material;
};

static point3_t get_center_at_time(const rt_moving_sphere_t *moving_sphere, double time);

rt_moving_sphere_t rt_moving_sphere_init(point3_t center_start, point3_t center_end, double time_start, double time_end,
                                         double radius, rt_material_t *material)
{
    assert(NULL != material);

    rt_moving_sphere_t result = {
        .base.type = RT_HITTABLE_TYPE_MOVING_SPHERE,
        .radius = radius,
        .center_start = center_start,
        .center_end = center_end,
        .time_start = time_start,
        .time_end = time_end,
        .material = material,
    };
    rt_material_claim(material);
    return result;
}

rt_moving_sphere_t *rt_moving_sphere_new(point3_t center_start, point3_t center_end, double time_start, double time_end,
                                         double radius, rt_material_t *material)
{
    rt_moving_sphere_t *sphere = calloc(1, sizeof(rt_moving_sphere_t));
    assert(NULL != sphere);

    *sphere = rt_moving_sphere_init(center_start, center_end, time_start, time_end, radius, material);
    return sphere;
}

void rt_moving_sphere_delete(rt_moving_sphere_t *moving_sphere)
{
    if (NULL == moving_sphere)
    {
        return;
    }

    rt_material_delete(moving_sphere->material);

    free(moving_sphere);
}

bool rt_moving_sphere_hit(const rt_moving_sphere_t *moving_sphere, const ray_t *ray, double t_min, double t_max,
                          rt_hit_record_t *record)
{
    assert(NULL != moving_sphere);
    assert(NULL != ray);

    point3_t center = get_center_at_time(moving_sphere, ray->time);
    return rt_sphere_hit_test_generic(center, moving_sphere->radius, moving_sphere->material, ray, t_min, t_max,
                                      record);
}

static point3_t get_center_at_time(const rt_moving_sphere_t *moving_sphere, double time)
{
    assert(NULL != moving_sphere);

    double scale = (time - moving_sphere->time_start) / (moving_sphere->time_end - moving_sphere->time_start);
    return vec3_sum(moving_sphere->center_start,
                    vec3_scale(vec3_diff(moving_sphere->center_end, moving_sphere->center_start), scale));
}
