//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_HITTABLE_H
#define RAY_TRACING_ONE_WEEK_HITTABLE_H

#include <stdbool.h>
#include "vec3.h"
#include "ray.h"

typedef struct hit_record_s
{
    point3_t p;
    vec3_t normal;
    double t;

    bool front_face;
} hit_record_t;

void hit_record_set_front_face(hit_record_t *record, const ray_t *ray, const vec3_t *outward_normal);

typedef enum hittable_type_e
{
    HITTABLE_TYPE_UNKNOWN = -1,
    HITTABLE_TYPE_SPHERE = 0,
} hittable_type_t;

typedef struct hittable_s
{
    hittable_type_t type;
} hittable_t;

bool hittable_hit(const hittable_t *hittable, const ray_t *ray, double t_min, double t_max, hit_record_t *record);

#endif //RAY_TRACING_ONE_WEEK_HITTABLE_H
