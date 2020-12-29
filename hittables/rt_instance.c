/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_instance.h"
#include <rt_hittable_shared.h>
#include <assert.h>
#include <string.h>

typedef struct rt_instance_s
{
    rt_hittable_t base;

    rt_hittable_t *hittable;

    vec3_t offset;

    rt_matrix3_t transform_matrix_ray;
    rt_matrix3_t transform_matrix_bb;

    rt_aabb_t bounding_box;
} rt_instance_t;

static bool rt_instance_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                            rt_hit_record_t *record);
static bool rt_instance_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);
static void rt_instance_delete(rt_hittable_t *instance);

rt_hittable_t *rt_instance_new(rt_hittable_t *hittable)
{
    rt_instance_t *result = calloc(1, sizeof(rt_instance_t));
    assert(NULL != result);

    result->hittable = hittable;
    rt_hittable_init(&result->base, RT_HITTABLE_TYPE_INSTANCE, rt_instance_hit, rt_instance_bb, rt_instance_delete);

    result->offset = vec3(0.0, 0.0, 0.0);
    result->transform_matrix_ray = rt_matrix_identity();
    result->transform_matrix_bb = rt_matrix_identity();

    return (rt_hittable_t *)result;
}

void rt_instance_translate(rt_hittable_t *instance, point3_t offset)
{
    assert(NULL != instance);

    assert(RT_HITTABLE_TYPE_INSTANCE == instance->type);
    rt_instance_t *i = (rt_instance_t *)instance;

    vec3_add(&i->offset, offset);
}

void rt_instance_rotate_y(rt_hittable_t *instance, double y)
{
    assert(NULL != instance);

    assert(RT_HITTABLE_TYPE_INSTANCE == instance->type);
    rt_instance_t *i = (rt_instance_t *)instance;

    i->transform_matrix_ray = rt_matrix_rotation_y(-RT_DEG_TO_RAD(y));
    i->transform_matrix_bb = rt_matrix_rotation_y(RT_DEG_TO_RAD(y));
}

static bool rt_instance_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                            rt_hit_record_t *record)
{
    assert(NULL != hittable);
    assert(NULL != ray);

    assert(RT_HITTABLE_TYPE_INSTANCE == hittable->type);
    rt_instance_t *instance = (rt_instance_t *)hittable;

    ray_t transformed_ray = ray_init(vec3_diff(ray->origin, instance->offset), ray->direction, ray->time);
    transformed_ray.origin = rt_mat3_mul_vec3(&instance->transform_matrix_ray, &transformed_ray.origin);
    transformed_ray.direction = rt_mat3_mul_vec3(&instance->transform_matrix_ray, &transformed_ray.direction);

    if (!rt_hittable_hit(instance->hittable, &transformed_ray, t_min, t_max, record))
    {
        return false;
    }

    record->p = rt_mat3_mul_vec3(&instance->transform_matrix_bb, &record->p);
    vec3_add(&record->p, instance->offset);

    vec3_t new_normal = rt_mat3_mul_vec3(&instance->transform_matrix_bb, &record->normal);
    rt_hit_record_set_front_face(record, &transformed_ray, &new_normal);

    return true;
}

static bool rt_instance_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != hittable);
    assert(NULL != out_bb);

    assert(RT_HITTABLE_TYPE_INSTANCE == hittable->type);
    rt_instance_t *instance = (rt_instance_t *)hittable;

    rt_aabb_t hittable_bb;
    if (!rt_hittable_bb(instance->hittable, time0, time1, &hittable_bb))
    {
        return false;
    }

    hittable_bb = rt_aabb(vec3_sum(hittable_bb.min, instance->offset), vec3_sum(hittable_bb.max, instance->offset));

    vec3_t min = hittable_bb.min, max = hittable_bb.max;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                point3_t point = point3(i * hittable_bb.max.x + (1 - i) * hittable_bb.min.x,
                                        j * hittable_bb.max.y + (1 - j) * hittable_bb.min.y,
                                        k * hittable_bb.max.z + (1 - k) * hittable_bb.min.z);

                vec3_t tester = rt_mat3_mul_vec3(&instance->transform_matrix_bb, &point);

                for (int c = 0; c < 3; c++)
                {
                    min.components[c] = fmin(min.components[c], tester.components[c]);
                    max.components[c] = fmax(max.components[c], tester.components[c]);
                }
            }
        }
    }

    *out_bb = rt_aabb(min, max);

    return true;
}

static void rt_instance_delete(rt_hittable_t *instance)
{
    if (NULL == instance)
    {
        return;
    }

    assert(NULL != instance);

    assert(RT_HITTABLE_TYPE_INSTANCE == instance->type);
    rt_instance_t *i = (rt_instance_t *)instance;

    rt_hittable_delete(i->hittable);
    free(instance);
}
