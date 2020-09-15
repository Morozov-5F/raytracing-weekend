//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_HITTABLE_H
#define RAY_TRACING_ONE_WEEK_RT_HITTABLE_H

#include <stdbool.h>
#include "rtweekend.h"

#include "rt_hit.h"

typedef struct rt_hittable_s rt_hittable_t;

bool rt_hittable_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record);

void rt_hittable_delete(rt_hittable_t *hittable);

#endif //RAY_TRACING_ONE_WEEK_RT_HITTABLE_H
