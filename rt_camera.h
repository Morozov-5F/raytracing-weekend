//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_CAMERA_H
#define RAY_TRACING_ONE_WEEK_RT_CAMERA_H

#include "rtweekend.h"

typedef struct rt_camera_s rt_camera_t;

rt_camera_t *rt_camera_new(point3_t look_from, point3_t look_at, vec3_t up, double vertical_fov, double aspect_ratio,
                           double aperture, double focus_distance);

ray_t rt_camera_get_ray(const rt_camera_t *camera, double s, double t);

void rt_camera_delete(rt_camera_t *camera);

#endif //RAY_TRACING_ONE_WEEK_RT_CAMERA_H
