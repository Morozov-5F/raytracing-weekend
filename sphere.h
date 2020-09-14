//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_SPHERE_H
#define RAY_TRACING_ONE_WEEK_SPHERE_H

#include <stdbool.h>

#include "hittable.h"
#include "ray.h"

typedef struct sphere_s
{
    hittable_t base;

    point3_t center;
    double radius;
} sphere_t;

sphere_t sphere_init(point3_t center, double radius);

bool sphere_hit(const sphere_t *sphere, const ray_t *ray, double t_min, double t_max, hit_record_t *record);

#endif //RAY_TRACING_ONE_WEEK_SPHERE_H
