//
// Created by morozov on 15.09.2020.
//

#include <assert.h>
#include "rt_hittable.h"
#include "rt_hittable_shared.h"
#include "rt_sphere.h"

void hit_record_set_front_face(hit_record_t *record, const ray_t *ray, const vec3_t *outward_normal)
{
    assert(NULL != record);
    assert(NULL != ray);
    assert(NULL != outward_normal);

    record->front_face = vec3_dot(*outward_normal, ray->direction) < 0;
    record->normal = record->front_face ? *outward_normal : vec3_negate(outward_normal);
}

bool hittable_hit(const hittable_t *hittable, const ray_t *ray, double t_min, double t_max, hit_record_t *record)
{
    assert(NULL != hittable);

    switch (hittable->type)
    {
        case HITTABLE_TYPE_SPHERE:
            return sphere_hit((const sphere_t *)hittable, ray, t_min, t_max, record);
        default:
            assert(0);
    }
    return false;
}

void hittable_delete(hittable_t *hittable)
{
    switch (hittable->type)
    {
        case HITTABLE_TYPE_SPHERE:
            sphere_delete((sphere_t *)hittable);
            break;
        default:
            assert(0);
    }
}
