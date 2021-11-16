/**
 * Copyright (c) 2021, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include <float.h>
#include "rt_hittable.h"
#include "rt_hittable_shared.h"

typedef struct rt_triangle_s
{
    rt_hittable_t base;

    point3_t a, b, c;
    vec3_t ba, ca;
    vec3_t n;

    vec3_t min, max;

    rt_material_t *material;
} rt_triangle_t;

static rt_triangle_t rt_triangle_init(point3_t a, point3_t b, point3_t c, rt_material_t *material);
static bool rt_triangle_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                            rt_hit_record_t *record);
static bool rt_triangle_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);
static void rt_triangle_delete(rt_hittable_t *hittable);

rt_hittable_t *rt_triangle_new(point3_t a, point3_t b, point3_t c, rt_material_t *material)
{
    rt_triangle_t *triangle = (rt_triangle_t *)calloc(1, sizeof(rt_triangle_t));
    assert(NULL != triangle);

    *triangle = rt_triangle_init(a, b, c, material);
    return (rt_hittable_t *)triangle;
}

static rt_triangle_t rt_triangle_init(point3_t a, point3_t b, point3_t c, rt_material_t *material)
{
    assert(NULL != material);

    rt_triangle_t result = {
        .a = a,
        .b = b,
        .c = c,

        .ba = vec3_diff(a, b),
        .ca = vec3_diff(a, c),

        .n = vec3_normalized(vec3_cross(vec3_diff(b, a), vec3_diff(c, a))),

        .min = vec3(fmin(fmin(a.x, b.x), c.x), fmin(fmin(a.y, b.y), c.y), fmin(fmin(a.z, b.z), c.z)),
        .max = vec3(fmax(fmax(a.x, b.x), c.x), fmax(fmax(a.y, b.y), c.y), fmax(fmax(a.z, b.z), c.z)),

        .material = material,
    };

    rt_hittable_init(&result.base, RT_HITTABLE_TYPE_TRIANGLE, rt_triangle_hit, rt_triangle_bb, rt_triangle_delete);
    return result;
}

static bool rt_triangle_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                            rt_hit_record_t *record)
{
    assert(NULL != hittable);
    assert(RT_HITTABLE_TYPE_TRIANGLE == hittable->type);

    const rt_triangle_t *triangle = (const rt_triangle_t *)hittable;

    // Calculate system determinant and compare it against zero
    double delta = triangle->ba.x * (triangle->ca.y * ray->direction.z - ray->direction.y * triangle->ca.z) -
                   triangle->ca.x * (triangle->ba.y * ray->direction.z - ray->direction.y * triangle->ba.z) +
                   ray->direction.x * (triangle->ba.y * triangle->ca.z - triangle->ca.y * triangle->ba.z);
    if (fabs(delta) < 10 * FLT_EPSILON)
    {
        return false;
    }

    // Find partial determinants
    vec3_t oa = vec3_diff(triangle->a, ray->origin);
    double delta1 = oa.x * (triangle->ca.y * ray->direction.z - ray->direction.y * triangle->ca.z) -
                    triangle->ca.x * (oa.y * ray->direction.z - ray->direction.y * oa.z) +
                    ray->direction.x * (oa.y * triangle->ca.z - triangle->ca.y * oa.z);
    double beta = delta1 / delta;
    double delta2 = triangle->ba.x * (oa.y * ray->direction.z - ray->direction.y * oa.z) -
                    oa.x * (triangle->ba.y * ray->direction.z - ray->direction.y * triangle->ba.z) +
                    ray->direction.x * (triangle->ba.y * oa.z - oa.y * triangle->ba.z);
    double gamma = delta2 / delta;
    double delta3 = triangle->ba.x * (triangle->ca.y * oa.z - oa.y * triangle->ca.z) -
                    triangle->ca.x * (triangle->ba.y * oa.z - oa.y * triangle->ba.z) +
                    oa.x * (triangle->ba.y * triangle->ca.z - triangle->ca.y * triangle->ba.z);
    double t = delta3 / delta;

    if (t >= t_max || t <= t_min || beta < 0 || gamma < 0 || (beta + gamma) > 1)
    {
        return false;
    }

    if (NULL != record)
    {
        record->t = t;
        record->p = ray_at(*ray, t);
        record->material = triangle->material;
        rt_hit_record_set_front_face(record, ray, &triangle->n);

        record->u = beta;
        record->v = gamma;
    }

    return true;
}

static bool rt_triangle_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != hittable);
    assert(RT_HITTABLE_TYPE_TRIANGLE == hittable->type);
    assert(NULL != out_bb);

    const rt_triangle_t *triangle = (const rt_triangle_t *)hittable;

    *out_bb = rt_aabb(triangle->min, triangle->max);

    return true;
}

static void rt_triangle_delete(rt_hittable_t *hittable)
{
    if (NULL == hittable)
    {
        return;
    }
    assert(RT_HITTABLE_TYPE_TRIANGLE == hittable->type);

    rt_triangle_t *triangle = (rt_triangle_t *)hittable;

    rt_material_delete(triangle->material);
    free(triangle);
}
