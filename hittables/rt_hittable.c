/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_hittable.h"
#include "rt_hittable_shared.h"

static bool hit_base(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                     rt_hit_record_t *record);
static bool bb_base(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);
static void delete_base(rt_hittable_t *hittable);

void rt_hittable_init(rt_hittable_t *hittable, rt_hittable_type_t type, rt_hittable_hit_fn hit_fn,
                      rt_hittable_bb_fn bb_fn, rt_hittable_delete_fn delete_fn)
{
    assert(NULL != hittable);

    hittable->refcount = 1;
    hittable->type = type;

    hittable->hit = hit_fn ? hit_fn : hit_base;
    hittable->bb = bb_fn ? bb_fn : bb_base;
    hittable->delete = delete_fn ? delete_fn : delete_base;
}

bool rt_hittable_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                     rt_hit_record_t *record)
{
    assert(NULL != hittable);

    return hittable->hit(hittable, ray, t_min, t_max, record);
}

rt_hittable_t *rt_hittable_claim(rt_hittable_t *hittable)
{
    hittable->refcount++;

    return hittable;
}

void rt_hittable_delete(rt_hittable_t *hittable)
{
    if (NULL == hittable || --hittable->refcount > 0)
    {
        return;
    }

    hittable->delete (hittable);
}

bool rt_hittable_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != hittable);

    return hittable->bb(hittable, time0, time1, out_bb);
}

int rt_hittable_box_cmp_x(const void *a, const void *b)
{
    assert(NULL != a && NULL != b);

    rt_aabb_t box_a, box_b;

    if (!rt_hittable_bb(*(const rt_hittable_t *const *)a, 0, 0, &box_a) ||
        !rt_hittable_bb(*(const rt_hittable_t *const *)b, 0, 0, &box_b))
    {
        assert(0);
    }

    if (box_a.min.x > box_b.min.x)
    {
        return 1;
    }
    else if (box_a.min.x < box_b.min.x)
    {
        return -1;
    }
    return 0;
}

int rt_hittable_box_cmp_y(const void *a, const void *b)
{
    assert(NULL != a && NULL != b);

    rt_aabb_t box_a, box_b;

    if (!rt_hittable_bb(*(const rt_hittable_t *const *)a, 0, 0, &box_a) ||
        !rt_hittable_bb(*(const rt_hittable_t *const *)b, 0, 0, &box_b))
    {
        assert(0);
    }

    if (box_a.min.y > box_b.min.y)
    {
        return 1;
    }
    else if (box_a.min.y < box_b.min.y)
    {
        return -1;
    }
    return 0;
}

int rt_hittable_box_cmp_z(const void *a, const void *b)
{
    assert(NULL != a && NULL != b);

    rt_aabb_t box_a, box_b;

    if (!rt_hittable_bb(*(const rt_hittable_t *const *)a, 0, 0, &box_a) ||
        !rt_hittable_bb(*(const rt_hittable_t *const *)b, 0, 0, &box_b))
    {
        assert(0);
    }

    if (box_a.min.z > box_b.min.z)
    {
        return 1;
    }
    else if (box_a.min.z < box_b.min.z)
    {
        return -1;
    }
    return 0;
}

static bool hit_base(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                     rt_hit_record_t *record)
{
    return false;
}

static bool bb_base(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    return false;
}

static void delete_base(rt_hittable_t *hittable)
{
    free(hittable);
}