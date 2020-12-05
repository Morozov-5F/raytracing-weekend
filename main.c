/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>

#include "hittables/rt_hittable_list.h"
#include "hittables/rt_sphere.h"
#include "materials/rt_material.h"
#include "materials/rt_material_dielectric.h"
#include "materials/rt_material_diffuse.h"
#include "materials/rt_material_metal.h"
#include "rt_camera.h"
#include "rt_colour.h"
#include "rt_weekend.h"
#include <errno.h>
#include <string.h>

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

static rt_hittable_list_t *random_scene(void)
{
    rt_material_t *ground_material = (rt_material_t *)rt_mt_diffuse_new(colour(0.5, 0.5, 0.5));

    rt_hittable_list_t *world = rt_hittable_list_init(500);
    rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            double material_chooser = rt_random_double(0, 1);
            point3_t center = vec3(a + 0.9 * rt_random_double(0, 1), 0.2, b + 0.9 * rt_random_double(0, 1));

            if (vec3_length(vec3_diff(center, point3(4, 0.2, 0))) > 0.9)
            {
                rt_material_t *sphere_material = NULL;
                if (material_chooser < 0.8)
                {
                    colour_t albedo = vec3_multiply(vec3_random(0, 1), vec3_random(0, 1));
                    sphere_material = (rt_material_t *)rt_mt_diffuse_new(albedo);
                } else if (material_chooser < 0.95)
                {
                    colour_t albedo = vec3_random(0.5, 1);
                    double fuzz = rt_random_double(0, 0.5);
                    sphere_material = (rt_material_t *)rt_mt_metal_new(albedo, fuzz);
                } else
                {
                    sphere_material = (rt_material_t *)rt_mt_dielectric_new(1.5);
                }
                rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(center, 0.2, sphere_material));
                rt_material_delete(sphere_material);
            }
        }
    }

    rt_material_t *material1 = (rt_material_t *)rt_mt_dielectric_new(1.5);
    rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(point3(0, 1, 0), 1.0, material1));

    rt_material_t *material2 = (rt_material_t *)rt_mt_diffuse_new(colour(0.4, 0.2, 0.1));
    rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(point3(-4, 1, 0), 1.0, material2));

    rt_material_t *material3 = (rt_material_t *)rt_mt_metal_new(colour(0.7, 0.6, 0.5), 0.0);
    rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(point3(4, 1, 0), 1.0, material3));

    rt_material_delete(material1);
    rt_material_delete(material2);
    rt_material_delete(material3);
    rt_material_delete(ground_material);
    return world;
}

int main(int argc, char const *argv[])
{
    // Image parameters
    const double ASPECT_RATIO = 3.0 / 2.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = (int)(IMAGE_WIDTH / ASPECT_RATIO);
    const int SAMPLES_PER_PIXEL = 40;
    const int CHILD_RAYS = 50;

    // Camera parameters
    point3_t look_from = point3(13, 2, 3);
    point3_t look_at = point3(0, 0, 0);
    vec3_t up = point3(0, 1, 0);
    double focus_distance = 10;
    double aperture = 0.1;
    rt_camera_t *camera = rt_camera_new(look_from, look_at, up, 20, ASPECT_RATIO, aperture, focus_distance);

    // World
    rt_hittable_list_t *world = random_scene();

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
