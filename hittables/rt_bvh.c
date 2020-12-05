/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_bvh.h"
#include "rt_hittable_shared.h"

struct rt_bvh_node_s
{
    rt_hittable_t base;

    rt_aabb_t box;

    rt_hittable_t *left;
    rt_hittable_t *right;

    double time_start;
    double time_end;
};

rt_bvh_node_t *rt_bvh_node_new(const rt_hittable_list_t *hittable_list, double time0, double time1)
{
    rt_bvh_node_t *result = calloc(1, sizeof(rt_bvh_node_t));
    assert(NULL != result);

    result->time_start = time0;
    result->time_end = time1;

    return result;
}

void rt_bvh_node_delete(rt_bvh_node_t *bvh_node)
{
    if (NULL == bvh_node)
    {
        return;
    }
    free(bvh_node);
}

bool rt_bvh_node_hit(const rt_bvh_node_t *bvh_node, const ray_t *ray, double t_min, double t_max,
                     rt_hit_record_t *record)
{
    assert(NULL != bvh_hode);
    assert(NULL != ray);

    if (!rt_aabb_hit(&bvh_node->box, t_min, t_max, ray))
    {
        return false;
    }

    bool hit_left = rt_hittable_hit(bvh_node->left, ray, t_min, t_max, record);
    bool hit_right = rt_hittable_hit(bvh_node->right, ray, t_min, t_max, record);

    return hit_left || hit_right;
}

bool rt_bvh_node_bb(const rt_bvh_node_t *bvh_node, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != bvh_hode);
    assert(NULL != out_bb);

    *out_bb = bvh_node->box;

    return true;
}
