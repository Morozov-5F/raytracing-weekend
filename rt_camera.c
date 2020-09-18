//
// Created by morozov on 15.09.2020.
//

#include "rt_camera.h"
#include <stdlib.h>
#include <assert.h>

struct rt_camera_s
{
    point3_t origin;
    point3_t lower_left;
    vec3_t horizontal;
    vec3_t vertical;
};

rt_camera_t *rt_camera_new(point3_t look_from, point3_t look_at, vec3_t up, double vertical_fov, double aspect_ratio)
{
    rt_camera_t *result = calloc(1, sizeof(rt_camera_t));
    assert(NULL != result);

    double theta = RT_DEG_TO_RAD(vertical_fov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = viewport_height * aspect_ratio;

    vec3_t w = vec3_normalized(vec3_diff(look_from, look_at));
    vec3_t u = vec3_normalized(vec3_cross(up, w));
    vec3_t v = vec3_cross(w, u);

    result->origin = look_from;
    result->horizontal = vec3_scale(u, viewport_width);
    result->vertical = vec3_scale(v, viewport_height);

    result->lower_left = vec3_diff(result->origin, vec3_scale(result->horizontal, 0.5));
    vec3_sub(&result->lower_left, vec3_scale(result->vertical, 0.5));
    vec3_sub(&result->lower_left, w);

    return result;
}

void rt_camera_delete(rt_camera_t *camera)
{
    free(camera);
}

ray_t rt_camera_get_ray(const rt_camera_t *camera, double s, double t)
{
    assert(NULL != camera);

    point3_t ray_direction = vec3_sum(camera->lower_left, vec3_scale(camera->horizontal, s));
    vec3_add(&ray_direction, vec3_scale(camera->vertical, t));
    vec3_sub(&ray_direction, camera->origin);

    return ray_init(camera->origin, ray_direction);
}
