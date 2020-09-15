//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_HITTABLE_H
#define RAY_TRACING_ONE_WEEK_HITTABLE_H

#include <stdbool.h>
#include "rtweekend.h"

typedef struct hit_record_s
{
    point3_t p;
    vec3_t normal;
    double t;

    bool front_face;
} hit_record_t;

void hit_record_set_front_face(hit_record_t *record, const ray_t *ray, const vec3_t *outward_normal);

typedef struct hittable_s hittable_t;

bool hittable_hit(const hittable_t *hittable, const ray_t *ray, double t_min, double t_max, hit_record_t *record);

void hittable_delete(hittable_t *hittable);

#endif //RAY_TRACING_ONE_WEEK_HITTABLE_H
