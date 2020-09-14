//
// Created by morozov on 15.09.2020.
//

#include <assert.h>
#include "hittable.h"
#include "sphere.h"

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