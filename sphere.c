//
// Created by morozov on 15.09.2020.
//

#include <assert.h>
#include "sphere.h"
#include "vec3.h"

sphere_t sphere_init(point3_t center, double radius)
{
    assert(radius > 0);

    sphere_t result = {
            .base.type = HITTABLE_TYPE_SPHERE,
            .radius = radius,
            .center = center,
    };
    return result;
}

bool sphere_hit(const sphere_t *sphere, const ray_t *ray, double t_min, double t_max, hit_record_t *record)
{
    assert(NULL != sphere);

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
        vec3_t outward_normal = vec3_scale(vec3_diff(record->p, sphere->center), 1.0 / sphere->radius);
        hit_record_set_front_face(record, ray, &outward_normal);
    }

    return true;
}
