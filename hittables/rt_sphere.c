/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "rt_hittable.h"
#include "rt_hittable_shared.h"
#include <assert.h>
#include <stdlib.h>

typedef struct rt_sphere_s
{
    rt_hittable_t base;

    point3_t center;
    double radius;
    rt_material_t *material;
} rt_sphere_t;

static rt_sphere_t rt_sphere_init(point3_t center, double radius, rt_material_t *material);
static bool rt_sphere_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                          rt_hit_record_t *record);
static bool rt_sphere_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);
static void rt_sphere_delete(rt_hittable_t *hittable);

rt_hittable_t *rt_sphere_new(point3_t center, double radius, rt_material_t *material)
{
    rt_sphere_t *sphere = calloc(1, sizeof(rt_sphere_t));
    assert(NULL != sphere);

    *sphere = rt_sphere_init(center, radius, material);
    return (rt_hittable_t *)sphere;
}

bool rt_sphere_hit_test_generic(point3_t center, double radius, rt_material_t *material, const ray_t *ray, double t_min,
                                double t_max, rt_hit_record_t *record)
{
    assert(NULL != ray);

    vec3_t ac = vec3_diff(ray->origin, center);
    double a = vec3_length_squared(ray->direction);
    double half_b = vec3_dot(ray->direction, ac);
    double c = vec3_length_squared(ac) - radius * radius;

    double discriminant_4 = half_b * half_b - a * c;
    if (discriminant_4 < 0) // No intersection
    {
        return false;
    }

    double disc_root = sqrt(discriminant_4);
    double t = (-half_b - disc_root) / a;
    if (t >= t_max || t <= t_min)
    {
        t = (-half_b + disc_root) / a;
        if (t >= t_max || t <= t_min)
        {
            return false;
        }
    }

    if (NULL != record)
    {
        record->t = t;
        record->p = ray_at(*ray, t);
        record->material = material;
        vec3_t outward_normal = vec3_scale(vec3_diff(record->p, center), 1.0 / radius);
        rt_hit_record_set_front_face(record, ray, &outward_normal);

        rt_get_sphere_uv(&outward_normal, &record->u, &record->v);
    }

    return true;
}

void rt_get_sphere_uv(const point3_t *p, double *u, double *v)
{
    assert(NULL != p);
    assert(NULL != u);
    assert(NULL != v);

    double theta = acos(-p->y);
    double phi = atan2(-p->z, p->x) + PI;

    *u = phi / (2 * PI);
    *v = theta / PI;
}

static rt_sphere_t rt_sphere_init(point3_t center, double radius, rt_material_t *material)
{
    assert(NULL != material);

    rt_sphere_t result = {
        .radius = radius,
        .center = center,
        .material = material,
    };
    rt_hittable_init(&result.base, RT_HITTABLE_TYPE_SPHERE, rt_sphere_hit, rt_sphere_bb, rt_sphere_delete);
    return result;
}

static bool rt_sphere_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                          rt_hit_record_t *record)
{
    assert(NULL != hittable);
    assert(RT_HITTABLE_TYPE_SPHERE == hittable->type);

    rt_sphere_t *sphere = (rt_sphere_t *)hittable;
    return rt_sphere_hit_test_generic(sphere->center, sphere->radius, sphere->material, ray, t_min, t_max, record);
}

static bool rt_sphere_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != hittable);
    assert(NULL != out_bb);
    assert(RT_HITTABLE_TYPE_SPHERE == hittable->type);

    rt_sphere_t *sphere = (rt_sphere_t *)hittable;
    vec3_t offset = vec3(sphere->radius, sphere->radius, sphere->radius);
    out_bb->min = vec3_diff(sphere->center, offset);
    out_bb->max = vec3_sum(sphere->center, offset);

    return true;
}

static void rt_sphere_delete(rt_hittable_t *hittable)
{
    if (NULL == hittable)
    {
        return;
    }
    assert(RT_HITTABLE_TYPE_SPHERE == hittable->type);

    rt_sphere_t *sphere = (rt_sphere_t *)hittable;

    rt_material_delete(sphere->material);
    free(sphere);
}
