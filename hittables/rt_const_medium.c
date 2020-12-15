/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <rt_texture.h>
#include <assert.h>
#include "rt_const_medium.h"
#include "rt_hittable_shared.h"
#include "rt_material.h"

struct rt_const_medium_s
{
    rt_hittable_t base;

    rt_hittable_t *boundary;
    rt_material_t *phase_function;

    double inv_neg_density;
};

rt_const_medium_t *rt_const_medium_new_with_texture(rt_hittable_t *hittable, double density, rt_texture_t *texture)
{
    rt_const_medium_t *result = calloc(1, sizeof(rt_const_medium_t));
    assert(NULL != result);

    result->boundary = hittable;
    result->inv_neg_density = -1.0 / density;
    result->phase_function = rt_mt_iso_new_with_texture(texture);

    rt_hittable_init(&result->base, RT_HITTABLE_CONSTANT_MEDIUM);

    return result;
}

rt_const_medium_t *rt_const_medium_new_with_colour(rt_hittable_t *hittable, double density, colour_t colour)
{
    return rt_const_medium_new_with_texture(hittable, density, rt_texture_sc_new(colour));
}

void rt_const_medium_delete(rt_const_medium_t *medium)
{
    if (NULL == medium)
    {
        return;
    }

    rt_hittable_delete(medium->boundary);
    rt_material_delete(medium->phase_function);

    free(medium);
}

bool rt_const_medium_hit(const rt_const_medium_t *medium, const ray_t *ray, double t_min, double t_max,
                         rt_hit_record_t *record)
{
    assert(NULL != medium);

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

bool rt_const_medium_bb(const rt_const_medium_t *medium, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != medium);

    return rt_hittable_bb(medium->boundary, time0, time1, out_bb);
}
