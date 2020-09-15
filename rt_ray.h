//
// Created by morozov on 14.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_RAY_H
#define RAY_TRACING_ONE_WEEK_RT_RAY_H

typedef struct ray_s
{
    point3_t origin;
    vec3_t direction;
} ray_t;

static inline ray_t ray_init(vec3_t origin, vec3_t direction)
{
    ray_t result = {origin, direction};
    return result;
}

static inline point3_t ray_at(ray_t ray, double t)
{
    return vec3_sum(ray.origin, vec3_scale(ray.direction, t));
}

#endif //RAY_TRACING_ONE_WEEK_RT_RAY_H
