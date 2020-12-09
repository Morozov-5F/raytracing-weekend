/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_hittable.h"
#include "rt_hittable_shared.h"
#include "rt_sphere.h"
#include "rt_moving_sphere.h"
#include "rt_bvh.h"
#include "rt_aa_rect.h"

bool rt_hittable_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                     rt_hit_record_t *record)
{
    assert(NULL != hittable);

    switch (hittable->type)
    {
        case RT_HITTABLE_TYPE_SPHERE:
            return rt_sphere_hit((const rt_sphere_t *)hittable, ray, t_min, t_max, record);
        case RT_HITTABLE_TYPE_MOVING_SPHERE:
            return rt_moving_sphere_hit((const rt_moving_sphere_t *)hittable, ray, t_min, t_max, record);
        case RT_HITTABLE_TYPE_BVH_NODE:
            return rt_bvh_node_hit((const rt_bvh_node_t *)hittable, ray, t_min, t_max, record);
        case RT_HITTABLE_TYPE_AA_RECT:
            return rt_aa_rect_hit((const rt_aa_rect_t *)hittable, ray, t_min, t_max, record);
        default:
            assert(0);
    }
    return false;
}

rt_hittable_t *rt_hittable_claim(rt_hittable_t *hittable)
{
    hittable->refcount++;

    return hittable;
}

void rt_hittable_delete(rt_hittable_t *hittable)
{
    if (--hittable->refcount > 0)
    {
        return;
    }

    switch (hittable->type)
    {
        case RT_HITTABLE_TYPE_SPHERE:
            rt_sphere_delete((rt_sphere_t *)hittable);
            break;
        case RT_HITTABLE_TYPE_MOVING_SPHERE:
            rt_moving_sphere_delete((rt_moving_sphere_t *)hittable);
            break;
        case RT_HITTABLE_TYPE_BVH_NODE:
            rt_bvh_node_delete((rt_bvh_node_t *)hittable);
        case RT_HITTABLE_TYPE_AA_RECT:
            rt_aa_rect_delete((rt_aa_rect_t *)hittable);
            break;
        default:
            assert(0);
    }
}

bool rt_hittable_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    switch (hittable->type)
    {
        case RT_HITTABLE_TYPE_SPHERE:
            return rt_sphere_bb((const rt_sphere_t *)hittable, time0, time1, out_bb);
        case RT_HITTABLE_TYPE_MOVING_SPHERE:
            return rt_moving_sphere_bb((const rt_moving_sphere_t *)hittable, time0, time1, out_bb);
        case RT_HITTABLE_TYPE_BVH_NODE:
            return rt_bvh_node_bb((const rt_bvh_node_t *)hittable, time0, time1, out_bb);
        case RT_HITTABLE_TYPE_AA_RECT:
            return rt_aa_rect_bb((const rt_aa_rect_t *)hittable, time0, time1, out_bb);
        default:
            assert(0);
    }
    return false;
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

void rt_hittable_init(rt_hittable_t *hittable, rt_hittable_type_t type)
{
    assert(NULL != hittable);

    hittable->refcount = 1;
    hittable->type = type;
}