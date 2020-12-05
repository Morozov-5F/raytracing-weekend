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
            return rt_bvh_node_hit((const rt_bvh_node_t*)hittable, ray, t_min, t_max, record);
        default:
            assert(0);
    }
    return false;
}

void rt_hittable_delete(rt_hittable_t *hittable)
{
    switch (hittable->type)
    {
        case RT_HITTABLE_TYPE_SPHERE:
            rt_sphere_delete((rt_sphere_t *)hittable);
            break;
        case RT_HITTABLE_TYPE_MOVING_SPHERE:
            rt_moving_sphere_delete((rt_moving_sphere_t*)hittable);
        case RT_HITTABLE_TYPE_BVH_NODE:
            rt_bvh_node_delete((rt_bvh_node_t*)hittable);
        default:
            assert(0);
    }
}

bool rt_hittable_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    switch (hittable->type)
    {
        case RT_HITTABLE_TYPE_SPHERE:
            return rt_sphere_bb((const rt_sphere_t*)hittable, time0, time1, out_bb);
        case RT_HITTABLE_TYPE_MOVING_SPHERE:
            return rt_moving_sphere_bb((const rt_moving_sphere_t *)hittable, time0, time1, out_bb);
        case RT_HITTABLE_TYPE_BVH_NODE:
            return rt_bvh_node_bb((const rt_bvh_node_t*)hittable, time0, time1, out_bb);
        default:
            assert(0);
    }
    return false;
}
