//
// Created by morozov on 16.09.2020.
//
#include <assert.h>
#include "rt_hit.h"

void rt_hit_record_set_front_face(rt_hit_record_t *record, const ray_t *ray, const vec3_t *outward_normal)
{
    assert(NULL != record);
    assert(NULL != ray);
    assert(NULL != outward_normal);

    record->front_face = vec3_dot(*outward_normal, ray->direction) < 0;
    record->normal = record->front_face ? *outward_normal : vec3_negate(outward_normal);
}

