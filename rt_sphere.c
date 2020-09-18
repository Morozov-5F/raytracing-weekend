//
// Created by morozov on 15.09.2020.
//

#include <assert.h>
#include <stdlib.h>
#include "rt_sphere.h"
#include "rt_vec3.h"
#include "rt_hittable_shared.h"
#include "rt_material.h"

struct rt_sphere_s
{
    rt_hittable_t base;

    point3_t center;
    double radius;
    rt_material_t *material;
};

rt_sphere_t rt_sphere_init(point3_t center, double radius, rt_material_t *material)
{
    assert(NULL != material);

    rt_sphere_t result = {
            .base.type = RT_HITTABLE_TYPE_SPHERE,
            .radius = radius,
            .center = center,
            .material = material,
    };
    rt_material_claim(material);
    return result;
}

bool rt_sphere_hit(const rt_sphere_t *sphere, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record)
{
    assert(NULL != sphere);
    assert(NULL != ray);

    vec3_t ac = vec3_diff(ray->origin, sphere->center);
    double a = vec3_length_squared(ray->direction);
    double half_b = vec3_dot(ray->direction, ac);
    double c = vec3_length_squared(ac) - sphere->radius * sphere->radius;

    double discriminant_4 = half_b * half_b - a * c;
    if (discriminant_4 < 0) // No intersection
    {
        return false;
    }

    double disc_root = sqrt(discriminant_4);
    double t = (-half_b - disc_root) / a;
    if (t > t_max || t < t_min)
    {
        t = (-half_b + disc_root) / a;
        if (t > t_max || t < t_min)
        {
            return false;
        }
    }

    if (NULL != record)
    {
        record->t = t;
        record->p = ray_at(*ray, t);
        record->material = sphere->material;
        vec3_t outward_normal = vec3_scale(vec3_diff(record->p, sphere->center), 1.0 / sphere->radius);
        rt_hit_record_set_front_face(record, ray, &outward_normal);
    }

    return true;
}

rt_sphere_t *rt_sphere_new(point3_t center, double radius, rt_material_t *material)
{
    rt_sphere_t *sphere = calloc(1, sizeof(rt_sphere_t));
    assert(NULL != sphere);

    *sphere = rt_sphere_init(center, radius, material);
    return sphere;
}

void rt_sphere_delete(rt_sphere_t *sphere)
{
    rt_material_delete(sphere->material);

    free(sphere);
}
