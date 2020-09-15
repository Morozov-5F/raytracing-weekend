//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_SPHERE_H
#define RAY_TRACING_ONE_WEEK_RT_SPHERE_H

#include <stdbool.h>

#include "rt_hittable.h"
#include "rtweekend.h"

typedef struct rt_sphere_s rt_sphere_t;

rt_sphere_t *rt_sphere_new(point3_t center, double radius);
void rt_sphere_delete(rt_sphere_t *sphere);

bool rt_sphere_hit(const rt_sphere_t *sphere, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record);

#endif //RAY_TRACING_ONE_WEEK_RT_SPHERE_H
