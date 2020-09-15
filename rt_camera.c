//
// Created by morozov on 15.09.2020.
//

#include "rt_camera.h"
#include <stdlib.h>
#include <assert.h>

static const double RT_CAMERA_ASPECT_RATIO = 16.0 / 9.0;
static const double RT_CAMERA_VIEWPORT_HEIGHT = 2.0;
static const double RT_CAMERA_VIEWPORT_WIDTH = RT_CAMERA_VIEWPORT_HEIGHT * RT_CAMERA_ASPECT_RATIO;
static const double RT_CAMERA_FOCAL_LENGTH = 1.0;

struct rt_camera_s
{
    point3_t origin;
    point3_t lower_left;
    vec3_t horizontal;
    vec3_t vertical;
};

rt_camera_t *rt_camera_new(void)
{
    rt_camera_t *result = calloc(1, sizeof(rt_camera_t));
    assert(NULL != result);

    result->origin = point3(0, 0, 0);
    result->horizontal = vec3(RT_CAMERA_VIEWPORT_WIDTH, 0, 0);
    result->vertical = vec3(0, RT_CAMERA_VIEWPORT_HEIGHT, 0);

    result->lower_left = vec3_diff(result->origin, vec3_scale(result->horizontal, 0.5));
    vec3_sub(&result->lower_left, vec3_scale(result->vertical, 0.5));
    vec3_sub(&result->lower_left, vec3(0, 0, RT_CAMERA_FOCAL_LENGTH));

    return result;
}

void rt_camera_delete(rt_camera_t *camera)
{
    free(camera);
}

ray_t rt_camera_get_ray(const rt_camera_t *camera, double u, double v)
{
    assert(NULL != camera);

    point3_t ray_direction = vec3_sum(camera->lower_left, vec3_scale(camera->horizontal, u));
    vec3_add(&ray_direction, vec3_scale(camera->vertical, v));
    vec3_sub(&ray_direction, camera->origin);

    return ray_init(camera->origin, ray_direction);
}
