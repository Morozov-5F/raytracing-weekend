//
// Created by morozov on 16.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_HIT_H
#define RAY_TRACING_ONE_WEEK_RT_HIT_H

#include <stdbool.h>
#include "rtweekend.h"

typedef struct rt_hit_record_s
{
    point3_t p;
    vec3_t normal;
    void *material;

    double t;

    bool front_face;
} rt_hit_record_t;

void rt_hit_record_set_front_face(rt_hit_record_t *record, const ray_t *ray, const vec3_t *outward_normal);

#endif //RAY_TRACING_ONE_WEEK_RT_HIT_H
