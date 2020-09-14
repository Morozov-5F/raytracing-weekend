//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_SPHERE_H
#define RAY_TRACING_ONE_WEEK_SPHERE_H

#include <stdbool.h>

#include "hittable.h"
#include "ray.h"

typedef struct sphere_s sphere_t;

sphere_t *sphere_new(point3_t center, double radius);
void sphere_delete(sphere_t *sphere);

bool sphere_hit(const sphere_t *sphere, const ray_t *ray, double t_min, double t_max, hit_record_t *record);

#endif //RAY_TRACING_ONE_WEEK_SPHERE_H
