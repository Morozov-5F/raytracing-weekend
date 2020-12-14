/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <rt_texture_solid_colour.h>
#include <assert.h>
#include "rt_const_medium.h"
#include "rt_hittable_shared.h"

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

    rt_hittable_init(hittable, RT_HITTABLE_CONSTANT_MEDIUM);

    return result;
}

rt_const_medium_t *rt_const_medium_new_with_colour(rt_hittable_t *hittable, double density, colour_t colour)
{
    return rt_const_medium_new_with_texture(hittable, density, (rt_texture_t *)rt_texture_sc_new(colour));
}

void rt_const_medium_delete(rt_const_medium_t *medium)
{
    if (NULL != medium)
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

    return false;
}

bool rt_const_medium_bb(const rt_const_medium_t *medium, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != medium);

    return rt_hittable_bb(medium->boundary, time0, time1, out_bb);
}
