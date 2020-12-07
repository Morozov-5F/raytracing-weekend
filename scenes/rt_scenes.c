/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_scenes.h"

#include <rt_weekend.h>

#include <rt_material.h>
#include <rt_material_diffuse.h>
#include <rt_material_metal.h>
#include <rt_material_dielectric.h>

#include <rt_hittable.h>
#include <rt_sphere.h>
#include <rt_moving_sphere.h>
#include <rt_bvh.h>

#include <rt_texture.h>
#include <rt_texture_checker_pattern.h>

rt_hittable_list_t *rt_scene_random(void)
{
    rt_material_t *ground_material = (rt_material_t *)rt_mt_diffuse_new_with_texture(
        (rt_texture_t *)rt_texture_cp_new_with_colour(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9)));

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
                rt_hittable_t *object_to_add = NULL;
                rt_material_t *sphere_material = NULL;
                if (material_chooser < 0.8)
                {
                    colour_t albedo = vec3_multiply(vec3_random(0, 1), vec3_random(0, 1));
                    sphere_material = (rt_material_t *)rt_mt_diffuse_new_with_albedo(albedo);
                    point3_t end_center = vec3_sum(center, vec3(0, rt_random_double(0, 0.5), 0));

                    object_to_add =
                        (rt_hittable_t *)rt_moving_sphere_new(center, end_center, 0.0, 1.0, 0.2, sphere_material);
                }
                else if (material_chooser < 0.95)
                {
                    colour_t albedo = vec3_random(0.5, 1);
                    double fuzz = rt_random_double(0, 0.5);
                    sphere_material = (rt_material_t *)rt_mt_metal_new(albedo, fuzz);

                    object_to_add = (rt_hittable_t *)rt_sphere_new(center, 0.2, sphere_material);
                }
                else
                {
                    sphere_material = (rt_material_t *)rt_mt_dielectric_new(1.5);

                    object_to_add = (rt_hittable_t *)rt_sphere_new(center, 0.2, sphere_material);
                }
                rt_hittable_list_add(world, object_to_add);
            }
        }
    }

    rt_material_t *material1 = (rt_material_t *)rt_mt_dielectric_new(1.5);
    rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(point3(0, 1, 0), 1.0, material1));

    rt_material_t *material2 = (rt_material_t *)rt_mt_diffuse_new_with_albedo(colour(0.4, 0.2, 0.1));
    rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(point3(-4, 1, 0), 1.0, material2));

    rt_material_t *material3 = (rt_material_t *)rt_mt_metal_new(colour(0.7, 0.6, 0.5), 0.0);
    rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(point3(4, 1, 0), 1.0, material3));

    // TODO: Deal with the world memory leaking later: add reference counting for all the hittables like I did with
    //  materials.
    rt_hittable_list_t *result = rt_hittable_list_init(1);
    rt_bvh_node_t *node = rt_bvh_node_new(world, 0.0, 1.0);
    rt_hittable_list_add(result, (rt_hittable_t *)node);

    rt_hittable_list_deinit(world);

    return result;
}

rt_hittable_list_t *rt_scene_two_spheres(void)
{
    rt_material_t *checker = (rt_material_t *)rt_mt_diffuse_new_with_texture(
        (rt_texture_t *)rt_texture_cp_new_with_colour(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9)));

    rt_hittable_list_t *objects = rt_hittable_list_init(2);

    rt_hittable_list_add(objects, (rt_hittable_t *)rt_sphere_new(point3(0, -10, 0), 10, checker));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_sphere_new(point3(0, 10, 0), 10, rt_material_claim(checker)));

    return objects;
}

rt_hittable_list_t *rt_scene_two_perlin_spheres(void)
{
    rt_material_t *checker = (rt_material_t *)rt_mt_diffuse_new_with_texture(
        (rt_texture_t *)rt_texture_cp_new_with_colour(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9)));

    rt_hittable_list_t *objects = rt_hittable_list_init(2);

    rt_hittable_list_add(objects, (rt_hittable_t *)rt_sphere_new(point3(0, -1000, 0), 1000, checker));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_sphere_new(point3(0, 2, 0), 2, rt_material_claim(checker)));

    return objects;
}
