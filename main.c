/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>

#include "hittables/rt_hittable_list.h"
#include "materials/rt_material.h"
#include "rt_camera.h"
#include "rt_colour.h"
#include "rt_weekend.h"
#include <errno.h>
#include <string.h>
#include <scenes/rt_scenes.h>

static colour_t ray_colour(const ray_t *ray, const rt_hittable_list_t *list, int child_rays)
{
    if (child_rays <= 0)
    {
        return colour(0, 0, 0);
    }

    rt_hit_record_t record;
    if (rt_hittable_list_hit_test(list, ray, 0.001, INFINITY, &record))
    {
        ray_t scattered;
        colour_t attenuation;
        if (rt_material_scatter(record.material, ray, &record, &attenuation, &scattered))
        {
            return vec3_multiply(attenuation, ray_colour(&scattered, list, child_rays - 1));
        }
        return colour(0, 0, 0);
    }
    point3_t unit_direction = vec3_normalized(ray->direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    return vec3_lerp(vec3(1, 1, 1), vec3(0.5, 0.7, 1), t);
}

int main(int argc, char const *argv[])
{
    // Image parameters
    const double ASPECT_RATIO = 3.0 / 2.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = (int)(IMAGE_WIDTH / ASPECT_RATIO);
    const int SAMPLES_PER_PIXEL = 100;
    const int CHILD_RAYS = 50;

    // Declare Camera parameters
    point3_t look_from, look_at;
    vec3_t up =  point3(0, 1, 0);
    double focus_distance = 10.0, aperture = 0.0, vertical_fov = 40.0;

    // World
    rt_hittable_list_t *world = NULL;
    rt_scene_id_t scene_id = RT_SCENE_TWO_PERLIN_SPHERES;

    // Select a scene from a pre-defined one
    switch (scene_id)
    {
        case RT_SCENE_RANDOM:
            look_from = point3(13, 2, 3);
            look_at = point3(0, 0, 0);
            aperture = 0.1;
            vertical_fov = 20.0;

            world = rt_scene_random();
            break;

        case RT_SCENE_TWO_SPHERES:
            look_from = point3(13, 2, 3);
            look_at = point3(0, 0, 0);
            vertical_fov = 20.0;

            world = rt_scene_two_spheres();
            break;

        case RT_SCENE_TWO_PERLIN_SPHERES:
            look_from = point3(13, 2, 3);
            look_at = point3(0, 0, 0);
            vertical_fov = 20.0;

            world = rt_scene_two_perlin_spheres();
            break;
    }

    rt_camera_t *camera =
        rt_camera_new(look_from, look_at, up, vertical_fov, ASPECT_RATIO, aperture, focus_distance, 0.0, 1.0);

    FILE *out_file = stdout;
    if (argc == 2)
    {
        out_file = fopen(argv[1], "w");
        if (NULL == out_file)
        {
            fprintf(stderr, "Unable to open file %s: %s", argv[1], strerror(errno));
            goto cleanup;
        }
    }
    // Render
    fprintf(out_file, "P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
    {
        fprintf(stderr, "\rScanlines remaining: %d", j);
        fflush(stderr);
        for (int i = 0; i < IMAGE_WIDTH; ++i)
        {
            colour_t pixel = colour(0, 0, 0);
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
            {
                double u = (double)(i + rt_random_double(0, 1)) / (IMAGE_WIDTH - 1);
                double v = (double)(j + rt_random_double(0, 1)) / (IMAGE_HEIGHT - 1);

                ray_t ray = rt_camera_get_ray(camera, u, v);
                vec3_add(&pixel, ray_colour(&ray, world, CHILD_RAYS));
            }
            rt_write_colour(out_file, pixel, SAMPLES_PER_PIXEL);
        }
    }
    fprintf(stderr, "\nDone\n");
cleanup:
    // Cleanup
    rt_hittable_list_deinit(world);
    rt_camera_delete(camera);

    return 0;
}
