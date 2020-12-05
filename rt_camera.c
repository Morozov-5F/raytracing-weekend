/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "rt_camera.h"
#include <assert.h>
#include <stdlib.h>

struct rt_camera_s
{
    point3_t origin;
    point3_t lower_left;
    vec3_t horizontal;
    vec3_t vertical;
    double lens_radius;

    vec3_t u;
    vec3_t v;
    vec3_t w;

    double shutter_start_time;
    double shutter_end_time;
};

rt_camera_t *rt_camera_new(point3_t look_from, point3_t look_at, vec3_t up, double vertical_fov, double aspect_ratio,
                           double aperture, double focus_distance, double shutter_start_time, double shutter_end_time)
{
    rt_camera_t *result = calloc(1, sizeof(rt_camera_t));
    assert(NULL != result);

    double theta = RT_DEG_TO_RAD(vertical_fov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = viewport_height * aspect_ratio;

    result->w = vec3_normalized(vec3_diff(look_from, look_at));
    result->u = vec3_normalized(vec3_cross(up, result->w));
    result->v = vec3_cross(result->w, result->u);

    result->origin = look_from;
    result->horizontal = vec3_scale(result->u, viewport_width * focus_distance);
    result->vertical = vec3_scale(result->v, viewport_height * focus_distance);

    result->lower_left = vec3_diff(result->origin, vec3_scale(result->horizontal, 0.5));
    vec3_sub(&result->lower_left, vec3_scale(result->vertical, 0.5));
    vec3_sub(&result->lower_left, vec3_scale(result->w, focus_distance));

    result->lens_radius = aperture / 2;
    result->shutter_start_time = shutter_start_time;
    result->shutter_end_time = shutter_end_time;

    return result;
}

void rt_camera_delete(rt_camera_t *camera)
{
    free(camera);
}

ray_t rt_camera_get_ray(const rt_camera_t *camera, double s, double t)
{
    assert(NULL != camera);

    vec3_t random_vector_on_lens = vec3_scale(vec3_random_in_unit_disc(), camera->lens_radius);
    vec3_t offset =
        vec3_sum(vec3_scale(camera->u, random_vector_on_lens.x), vec3_scale(camera->v, random_vector_on_lens.y));

    point3_t ray_direction = vec3_sum(camera->lower_left, vec3_scale(camera->horizontal, s));
    vec3_add(&ray_direction, vec3_scale(camera->vertical, t));
    vec3_sub(&ray_direction, camera->origin);
    vec3_sub(&ray_direction, offset);

    return ray_init(vec3_sum(camera->origin, offset), ray_direction,
                    rt_random_double(camera->shutter_start_time, camera->shutter_end_time));
}
