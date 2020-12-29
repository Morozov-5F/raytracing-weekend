/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_hittable.h"
#include "rt_hittable_list.h"
#include "rt_aa_rect.h"
#include <rt_hittable_shared.h>
#include <assert.h>

typedef struct rt_box_s
{
    rt_hittable_t base;

    rt_hittable_list_t *sides;
    point3_t min;
    point3_t max;
} rt_box_t;

static bool rt_box_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                       rt_hit_record_t *record);
static bool rt_box_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);
static void rt_box_delete(rt_hittable_t *hittable);

rt_hittable_t *rt_box_new(point3_t min, point3_t max, rt_material_t *material)
{
    rt_box_t *result = calloc(1, sizeof(rt_box_t));
    assert(NULL != result);

    result->min = min;
    result->max = max;

    result->sides = rt_hittable_list_init(6);
    assert(NULL != result->sides);

    // Front and back
    rt_hittable_list_add(result->sides, rt_aa_rect_new_xy(min.x, max.x, min.y, max.y, min.z, material));
    // Left
    rt_hittable_list_add(result->sides,
                         rt_aa_rect_new_yz(min.y, max.y, min.z, max.z, min.x, rt_material_claim(material)));
    // Right
    rt_hittable_list_add(result->sides,
                         rt_aa_rect_new_yz(min.y, max.y, min.z, max.z, max.x, rt_material_claim(material)));
    // Top
    rt_hittable_list_add(result->sides,
                         rt_aa_rect_new_xz(min.x, max.x, min.z, max.z, max.y, rt_material_claim(material)));
    // Bottom
    rt_hittable_list_add(result->sides,
                         rt_aa_rect_new_xz(min.x, max.x, min.z, max.z, min.y, rt_material_claim(material)));
    // Back
    rt_hittable_list_add(result->sides,
                         rt_aa_rect_new_xy(min.x, max.x, min.y, max.y, max.z, rt_material_claim(material)));

    rt_hittable_init(&result->base, RT_HITTABLE_TYPE_BOX, rt_box_hit, rt_box_bb, rt_box_delete);

    return (rt_hittable_t *)result;
}

bool rt_box_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record)
{
    assert(NULL != hittable);
    assert(NULL != ray);

    assert(RT_HITTABLE_TYPE_BOX == hittable->type);
    rt_box_t *box = (rt_box_t *)hittable;

    return rt_hittable_list_hit_test(box->sides, ray, t_min, t_max, record);
}

bool rt_box_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != hittable);
    assert(NULL != out_bb);

    assert(RT_HITTABLE_TYPE_BOX == hittable->type);
    rt_box_t *box = (rt_box_t *)hittable;

    *out_bb = rt_aabb(box->min, box->max);

    return true;
}

void rt_box_delete(rt_hittable_t *hittable)
{
    if (NULL == hittable)
    {
        return;
    }
    assert(RT_HITTABLE_TYPE_BOX == hittable->type);
    rt_box_t *box = (rt_box_t *)hittable;

    rt_hittable_list_deinit(box->sides);
    free(box);
}
