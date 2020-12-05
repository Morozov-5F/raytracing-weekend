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

static rt_bvh_node_t *bvh_make_node(rt_hittable_t **hittable_array, size_t start, size_t end, double time0,
                                    double time1);

rt_bvh_node_t *rt_bvh_node_new(const rt_hittable_list_t *hittable_list, double time0, double time1)
{
    return bvh_make_node(rt_hittable_list_get_underlying_container(hittable_list), 0,
                         rt_hittable_list_get_size(hittable_list), time0, time1);
}

void rt_bvh_node_delete(rt_bvh_node_t *bvh_node)
{
    if (NULL == bvh_node)
    {
        return;
    }

    if (bvh_node->left != bvh_node->right)
    {
        rt_hittable_delete(bvh_node->right);
    }
    rt_hittable_delete(bvh_node->left);

    free(bvh_node);
}

bool rt_bvh_node_hit(const rt_bvh_node_t *bvh_node, const ray_t *ray, double t_min, double t_max,
                     rt_hit_record_t *record)
{
    assert(NULL != bvh_node);
    assert(NULL != ray);

    if (!rt_aabb_hit(&bvh_node->box, t_min, t_max, ray))
    {
        return false;
    }

    bool hit_left = rt_hittable_hit(bvh_node->left, ray, t_min, t_max, record);
    bool hit_right = rt_hittable_hit(bvh_node->right, ray, t_min, hit_left ? record->t : t_max, record);

    return hit_left || hit_right;
}

bool rt_bvh_node_bb(const rt_bvh_node_t *bvh_node, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != bvh_node);
    assert(NULL != out_bb);

    *out_bb = bvh_node->box;

    return true;
}

static rt_bvh_node_t *bvh_make_node(rt_hittable_t **hittable_array, size_t start, size_t end, double time0,
                                    double time1)
{
    assert(NULL != hittable_array);

    rt_bvh_node_t *result = calloc(1, sizeof(rt_bvh_node_t));
    assert(NULL != result);

    size_t number_of_objects = end - start;
    int axis = rand() % 3;
    rt_hittable_compare_fn cmp = rt_hittable_box_cmp_x;
    if (axis == 1)
    {
        cmp = rt_hittable_box_cmp_y;
    }
    else if (axis == 2)
    {
        cmp = rt_hittable_box_cmp_z;
    }

    if (number_of_objects == 1)
    {
        result->left = result->right = hittable_array[start];
    }
    else if (number_of_objects == 2)
    {
        if (cmp(hittable_array + start, hittable_array + start + 1) < 0)
        {
            result->left = hittable_array[start];
            result->right = hittable_array[start + 1];
        }
        else
        {
            result->left = hittable_array[start + 1];
            result->right = hittable_array[start];
        }
    }
    else
    {
        qsort(hittable_array + start, number_of_objects, sizeof(rt_hittable_t *), cmp);
        size_t middle = start + number_of_objects / 2;

        result->left = (rt_hittable_t *)bvh_make_node(hittable_array, start, middle, time0, time1);
        result->right = (rt_hittable_t *)bvh_make_node(hittable_array, middle, end, time0, time1);
    }

    rt_aabb_t box_left = {0}, box_right = {0};
    if (!rt_hittable_bb(result->left, time0, time1, &box_left) ||
        !rt_hittable_bb(result->right, time0, time1, &box_right))
    {
        assert(0);
    }

    result->base.type = RT_HITTABLE_TYPE_BVH_NODE;
    result->time_start = time0;
    result->time_end = time1;
    result->box = rt_aabb_surrounding_bb(box_left, box_right);

    return result;
}
