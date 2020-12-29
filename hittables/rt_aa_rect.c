/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_aa_rect.h"
#include <rt_hittable_shared.h>
#include <assert.h>

typedef struct rt_aa_rect_s
{
    rt_hittable_t base;
    rt_material_t *material;
    rt_aa_rect_type_t type;
    double axis1_min, axis1_max, axis2_min, axis2_max, k;

    int axis_1, axis_2, axis_k;
    vec3_t outward_normal;
} rt_aa_rect_t;

static bool rt_aa_rect_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                           rt_hit_record_t *record);
static bool rt_aa_rect_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);
static void rt_aa_rect_delete(rt_hittable_t *hittable);

rt_hittable_t *rt_aa_rect_new(rt_aa_rect_type_t type, double axis1_min, double axis1_max, double axis2_min,
                              double axis2_max, double k, rt_material_t *material)
{
    rt_aa_rect_t *result = calloc(1, sizeof(rt_aa_rect_t));
    assert(NULL != result);

    result->material = material;

    switch (type)
    {
        case RT_AA_RECT_TYPE_XY:
            result->outward_normal = vec3(0, 0, 1);
            result->axis_1 = VEC3_AXIS_X;
            result->axis_2 = VEC3_AXIS_Y;
            result->axis_k = VEC3_AXIS_Z;
            break;

        case RT_AA_RECT_TYPE_YZ:
            result->outward_normal = vec3(1, 0, 0);
            result->axis_1 = VEC3_AXIS_Y;
            result->axis_2 = VEC3_AXIS_Z;
            result->axis_k = VEC3_AXIS_X;
            break;

        case RT_AA_RECT_TYPE_XZ:
            result->outward_normal = vec3(0, 1, 0);
            result->axis_1 = VEC3_AXIS_X;
            result->axis_2 = VEC3_AXIS_Z;
            result->axis_k = VEC3_AXIS_Y;
            break;
        default:
            assert(0);
    }

    result->axis1_min = axis1_min;
    result->axis2_min = axis2_min;
    result->k = k;
    result->axis1_max = axis1_max;
    result->axis2_max = axis2_max;

    rt_hittable_init(&result->base, RT_HITTABLE_TYPE_AA_RECT, rt_aa_rect_hit, rt_aa_rect_bb, rt_aa_rect_delete);

    return (rt_hittable_t *)result;
}

bool rt_aa_rect_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record)
{
    assert(NULL != hittable);
    assert(NULL != ray);

    assert(RT_HITTABLE_TYPE_AA_RECT == hittable->type);
    rt_aa_rect_t *rect = (rt_aa_rect_t *)hittable;

    double t = (rect->k - ray->origin.components[rect->axis_k]) / ray->direction.components[rect->axis_k];
    if (t >= t_max || t <= t_min)
    {
        return false;
    }

    point3_t hit = ray_at(*ray, t);
    double hit_axis_1 = hit.components[rect->axis_1];
    double hit_axis_2 = hit.components[rect->axis_2];
    if (hit_axis_1 < rect->axis1_min || hit_axis_1 > rect->axis1_max || hit_axis_2 < rect->axis2_min ||
        hit_axis_2 > rect->axis2_max)
    {
        return false;
    }

    if (NULL != record)
    {
        record->material = rect->material;
        record->u = (hit_axis_1 - rect->axis1_min) / (rect->axis1_max - rect->axis1_min);
        record->v = (hit_axis_2 - rect->axis2_min) / (rect->axis2_max - rect->axis2_min);
        record->t = t;
        record->p = hit;

        rt_hit_record_set_front_face(record, ray, &rect->outward_normal);
    }

    return true;
}

bool rt_aa_rect_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != hittable);
    assert(NULL != out_bb);

    assert(RT_HITTABLE_TYPE_AA_RECT == hittable->type);
    rt_aa_rect_t *rect = (rt_aa_rect_t *)hittable;

    out_bb->min.components[rect->axis_1] = rect->axis1_min;
    out_bb->max.components[rect->axis_1] = rect->axis1_max;

    out_bb->min.components[rect->axis_2] = rect->axis2_min;
    out_bb->max.components[rect->axis_2] = rect->axis2_max;

    out_bb->min.components[rect->axis_k] = rect->k - 0.0001;
    out_bb->max.components[rect->axis_k] = rect->k + 0.0001;

    return true;
}

void rt_aa_rect_delete(rt_hittable_t *hittable)
{
    if (NULL == hittable)
    {
        return;
    }
    assert(RT_HITTABLE_TYPE_AA_RECT == hittable->type);
    rt_aa_rect_t *rect = (rt_aa_rect_t *)hittable;

    rt_material_delete(rect->material);
    free(rect);
}
