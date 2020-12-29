/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <rt_texture.h>
#include <assert.h>
#include "rt_hittable.h"
#include "rt_hittable_shared.h"
#include "rt_material.h"

typedef struct rt_const_medium_s
{
    rt_hittable_t base;

    rt_hittable_t *boundary;
    rt_material_t *phase_function;

    double inv_neg_density;
} rt_const_medium_t;

static bool rt_const_medium_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                                rt_hit_record_t *record);
static void rt_const_medium_delete(rt_hittable_t *hittable);
static bool rt_const_medium_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);

rt_hittable_t *rt_const_medium_new_with_texture(rt_hittable_t *boundary, double density, rt_texture_t *texture)
{
    rt_const_medium_t *result = calloc(1, sizeof(rt_const_medium_t));
    assert(NULL != result);

    result->boundary = boundary;
    result->inv_neg_density = -1.0 / density;
    result->phase_function = rt_mt_iso_new_with_texture(texture);

    rt_hittable_init(&result->base, RT_HITTABLE_CONSTANT_MEDIUM, rt_const_medium_hit, rt_const_medium_bb,
                     rt_const_medium_delete);

    return (rt_hittable_t *)result;
}

rt_hittable_t *rt_const_medium_new_with_colour(rt_hittable_t *boundary, double density, colour_t colour)
{
    return rt_const_medium_new_with_texture(boundary, density, rt_texture_sc_new(colour));
}

static bool rt_const_medium_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                                rt_hit_record_t *record)
{
    assert(NULL != hittable);
    assert(RT_HITTABLE_CONSTANT_MEDIUM == hittable->type);
    rt_const_medium_t *medium = (rt_const_medium_t *)hittable;

    rt_hit_record_t hit1, hit2;

    if (!rt_hittable_hit(medium->boundary, ray, -INFINITY, INFINITY, &hit1))
    {
        return false;
    }
    if (!rt_hittable_hit(medium->boundary, ray, hit1.t + 0.0001, INFINITY, &hit2))
    {
        return false;
    }

    if (hit1.t < t_min)
    {
        hit1.t = t_min;
    }
    if (hit2.t > t_max)
    {
        hit2.t = t_max;
    }

    if (hit1.t >= hit2.t)
    {
        return false;
    }

    if (hit1.t < 0)
    {
        hit1.t = 0;
    }

    double ray_length = vec3_length(ray->direction);
    double dist_inside_bound = (hit2.t - hit1.t) * ray_length;
    double hit_distance = medium->inv_neg_density * log(rt_random_double(0.0, 1.0));

    if (hit_distance > dist_inside_bound)
    {
        return false;
    }

    record->t = hit1.t + hit_distance / ray_length;
    record->p = ray_at(*ray, record->t);
    record->normal = vec3(1, 0, 0);
    record->front_face = true;
    record->material = medium->phase_function;

    return true;
}

static bool rt_const_medium_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != hittable);
    assert(RT_HITTABLE_CONSTANT_MEDIUM == hittable->type);
    rt_const_medium_t *medium = (rt_const_medium_t *)hittable;

    return rt_hittable_bb(medium->boundary, time0, time1, out_bb);
}

static void rt_const_medium_delete(rt_hittable_t *hittable)
{
    if (NULL == hittable)
    {
        return;
    }

    assert(RT_HITTABLE_CONSTANT_MEDIUM == hittable->type);
    rt_const_medium_t *medium = (rt_const_medium_t *)hittable;

    rt_hittable_delete(medium->boundary);
    rt_material_delete(medium->phase_function);

    free(medium);
}