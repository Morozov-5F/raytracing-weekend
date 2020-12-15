/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_hittable.h"
#include "rt_hittable_shared.h"

typedef struct rt_moving_sphere_s
{
    rt_hittable_t base;

    point3_t center_start;
    point3_t center_end;

    double time_start;
    double time_end;

    double radius;
    rt_material_t *material;
} rt_moving_sphere_t;

static rt_moving_sphere_t rt_moving_sphere_init(point3_t center_start, point3_t center_end, double time_start,
                                                double time_end, double radius, rt_material_t *material);
static bool rt_moving_sphere_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                                 rt_hit_record_t *record);
static bool rt_moving_sphere_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);
static void rt_moving_sphere_delete(rt_hittable_t *hittable);
static point3_t get_center_at_time(const rt_moving_sphere_t *moving_sphere, double time);

rt_hittable_t *rt_moving_sphere_new(point3_t center_start, point3_t center_end, double time_start, double time_end,
                                    double radius, rt_material_t *material)
{
    rt_moving_sphere_t *sphere = calloc(1, sizeof(rt_moving_sphere_t));
    assert(NULL != sphere);

    *sphere = rt_moving_sphere_init(center_start, center_end, time_start, time_end, radius, material);
    return (rt_hittable_t *)sphere;
}

static rt_moving_sphere_t rt_moving_sphere_init(point3_t center_start, point3_t center_end, double time_start,
                                                double time_end, double radius, rt_material_t *material)
{
    assert(NULL != material);

    rt_moving_sphere_t result = {
        .radius = radius,
        .center_start = center_start,
        .center_end = center_end,
        .time_start = time_start,
        .time_end = time_end,
        .material = material,
    };
    rt_hittable_init(&result.base, RT_HITTABLE_TYPE_MOVING_SPHERE, rt_moving_sphere_hit, rt_moving_sphere_bb,
                     rt_moving_sphere_delete);
    return result;
}

bool rt_moving_sphere_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                          rt_hit_record_t *record)
{
    assert(NULL != hittable);
    assert(NULL != ray);

    assert(RT_HITTABLE_TYPE_MOVING_SPHERE == hittable->type);
    rt_moving_sphere_t *moving_sphere = (rt_moving_sphere_t *)hittable;

    point3_t center = get_center_at_time(moving_sphere, ray->time);
    return rt_sphere_hit_test_generic(center, moving_sphere->radius, moving_sphere->material, ray, t_min, t_max,
                                      record);
}

bool rt_moving_sphere_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != hittable);
    assert(NULL != out_bb);

    assert(RT_HITTABLE_TYPE_MOVING_SPHERE == hittable->type);
    rt_moving_sphere_t *moving_sphere = (rt_moving_sphere_t *)hittable;

    vec3_t offset = vec3(moving_sphere->radius, moving_sphere->radius, moving_sphere->radius);

    rt_aabb_t a = {
        .min = vec3_diff(get_center_at_time(moving_sphere, time0), offset),
        .max = vec3_sum(get_center_at_time(moving_sphere, time0), offset),
    };
    rt_aabb_t b = {
        .min = vec3_diff(get_center_at_time(moving_sphere, time1), offset),
        .max = vec3_sum(get_center_at_time(moving_sphere, time1), offset),
    };

    *out_bb = rt_aabb_surrounding_bb(a, b);

    return true;
}

static void rt_moving_sphere_delete(rt_hittable_t *hittable)
{
    if (NULL == hittable)
    {
        return;
    }

    assert(RT_HITTABLE_TYPE_MOVING_SPHERE == hittable->type);
    rt_moving_sphere_t *moving_sphere = (rt_moving_sphere_t *)hittable;

    rt_material_delete(moving_sphere->material);

    free(moving_sphere);
}

static point3_t get_center_at_time(const rt_moving_sphere_t *moving_sphere, double time)
{
    assert(NULL != moving_sphere);

    double scale = (time - moving_sphere->time_start) / (moving_sphere->time_end - moving_sphere->time_start);
    return vec3_sum(moving_sphere->center_start,
                    vec3_scale(vec3_diff(moving_sphere->center_end, moving_sphere->center_start), scale));
}
