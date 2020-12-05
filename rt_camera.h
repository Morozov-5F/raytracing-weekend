/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_CAMERA_H
#define RAY_TRACING_ONE_WEEK_RT_CAMERA_H

#include "rt_weekend.h"

typedef struct rt_camera_s rt_camera_t;

rt_camera_t *rt_camera_new(point3_t look_from, point3_t look_at, vec3_t up, double vertical_fov, double aspect_ratio,
                           double aperture, double focus_distance, double shutter_start_time, double shutter_end_time);

ray_t rt_camera_get_ray(const rt_camera_t *camera, double s, double t);

void rt_camera_delete(rt_camera_t *camera);

#endif // RAY_TRACING_ONE_WEEK_RT_CAMERA_H
