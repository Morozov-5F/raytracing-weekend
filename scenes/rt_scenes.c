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
#include <rt_material_diffuse_light.h>

#include <rt_hittable.h>
#include <rt_sphere.h>
#include <rt_moving_sphere.h>
#include <rt_bvh.h>
#include <rt_const_medium.h>

#include <rt_texture.h>
#include <rt_texture_checker_pattern.h>
#include <rt_texture_noise.h>
#include <rt_texture_image.h>
#include <rt_aa_rect.h>
#include <rt_box.h>
#include <rt_instance.h>

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
    rt_material_t *noisy = (rt_material_t *)rt_mt_diffuse_new_with_texture((rt_texture_t *)rt_texture_noise_new(4));

    rt_hittable_list_t *objects = rt_hittable_list_init(2);

    rt_hittable_list_add(objects, (rt_hittable_t *)rt_sphere_new(point3(0, -1000, 0), 1000, noisy));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_sphere_new(point3(0, 2, 0), 2, rt_material_claim(noisy)));

    return objects;
}

rt_hittable_list_t *rt_scene_earth(void)
{
    rt_material_t *earth_material = (rt_material_t *)rt_mt_diffuse_new_with_texture(
        (rt_texture_t *)rt_texture_image_new("assets/textures/earth_projection.jpg"));

    rt_hittable_list_t *objects = rt_hittable_list_init(1);

    rt_hittable_list_add(objects, (rt_hittable_t *)rt_sphere_new(point3(0, 0, 0), 2, earth_material));

    return objects;
}

rt_hittable_list_t *rt_scene_light_sample(void)
{
    rt_material_t *noisy = (rt_material_t *)rt_mt_diffuse_new_with_texture((rt_texture_t *)rt_texture_noise_new(4));

    rt_hittable_list_t *objects = rt_hittable_list_init(3);

    rt_hittable_list_add(objects, (rt_hittable_t *)rt_sphere_new(point3(0, -1000, 0), 1000, noisy));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_sphere_new(point3(0, 2, 0), 2, rt_material_claim(noisy)));

    rt_material_t *diffuse_light = (rt_material_t *)rt_mt_dl_new_with_albedo(colour(0.7, 0.8, 0.6), 4);
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_xy(3, 5, 1, 3, -2, diffuse_light));

    return objects;
}

rt_hittable_list_t *rt_scene_cornell_box(void)
{
    rt_hittable_list_t *objects = rt_hittable_list_init(8);

    rt_material_t *red = (rt_material_t *)rt_mt_diffuse_new_with_albedo(colour(0.65, 0.05, 0.05));
    rt_material_t *green = (rt_material_t *)rt_mt_diffuse_new_with_albedo(colour(0.12, 0.45, 0.15));
    rt_material_t *white = (rt_material_t *)rt_mt_diffuse_new_with_albedo(colour(0.73, 0.73, 0.73));

    // Walls
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_yz(0, 555, 0, 555, 555, green));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_yz(0, 555, 0, 555, 0, red));

    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_xz(0, 555, 0, 555, 0, white));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_xz(0, 555, 0, 555, 555, rt_material_claim(white)));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_xy(0, 555, 0, 555, 555, rt_material_claim(white)));
    // Boxes
    rt_instance_t *box1 =
        rt_instance_new((rt_hittable_t *)rt_box_new(point3(0, 0, 0), point3(165, 330, 165), rt_material_claim(white)));
    rt_instance_rotate_y(box1, 15);
    rt_instance_translate(box1, vec3(265, 0, 295));
    rt_hittable_list_add(objects, (rt_hittable_t *)box1);

    rt_instance_t *box2 =
        rt_instance_new((rt_hittable_t *)rt_box_new(point3(0, 0, 0), point3(165, 165, 165), rt_material_claim(white)));
    rt_instance_rotate_y(box2, -18);
    rt_instance_translate(box2, vec3(130, 0, 65));
    rt_hittable_list_add(objects, (rt_hittable_t *)box2);

    // Light source
    rt_material_t *diffuse_light = (rt_material_t *)rt_mt_dl_new_with_albedo(colour(1, 1, 1), 15);
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_xz(213, 343, 227, 332, 554, diffuse_light));

    rt_bvh_node_t *bvh = rt_bvh_node_new(objects, 0, 1);
    rt_hittable_list_t *result = rt_hittable_list_init(1);
    rt_hittable_list_add(result, (rt_hittable_t *)bvh);

    rt_hittable_list_deinit(objects);

    return result;
}

rt_hittable_list_t *rt_scene_instance_test(void)
{
    rt_material_t *green = (rt_material_t *)rt_mt_diffuse_new_with_albedo(colour(0.12, 0.45, 0.15));
    rt_material_t *red = (rt_material_t *)rt_mt_diffuse_new_with_albedo(colour(0.65, 0.05, 0.05));

    rt_hittable_list_t *objects = rt_hittable_list_init(2);

    rt_box_t *box = rt_box_new(point3(-1, -1, -1), point3(1, 1, 1), green);
    rt_instance_t *instance = rt_instance_new((rt_hittable_t *)box);
    rt_instance_rotate_y(instance, 0);
    rt_instance_translate(instance, point3(1, 0, 0));

    rt_hittable_list_add(objects, (rt_hittable_t *)instance);
    rt_hittable_list_add(objects,
                         (rt_hittable_t *)rt_box_new(point3(-0.25, -0.25, -0.25), point3(0.25, 0.25, 0.25), red));

    rt_bvh_node_t *bvh = rt_bvh_node_new(objects, 0, 1);
    rt_hittable_list_t *result = rt_hittable_list_init(1);
    rt_hittable_list_add(result, (rt_hittable_t *)bvh);

    return result;
}

rt_hittable_list_t *rt_scene_cornell_box_smoke_boxes(void)
{
    rt_hittable_list_t *objects = rt_hittable_list_init(8);

    rt_material_t *red = (rt_material_t *)rt_mt_diffuse_new_with_albedo(colour(0.65, 0.05, 0.05));
    rt_material_t *green = (rt_material_t *)rt_mt_diffuse_new_with_albedo(colour(0.12, 0.45, 0.15));
    rt_material_t *white = (rt_material_t *)rt_mt_diffuse_new_with_albedo(colour(0.73, 0.73, 0.73));

    // Walls
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_yz(0, 555, 0, 555, 555, green));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_yz(0, 555, 0, 555, 0, red));

    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_xz(0, 555, 0, 555, 0, white));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_xz(0, 555, 0, 555, 555, rt_material_claim(white)));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_xy(0, 555, 0, 555, 555, rt_material_claim(white)));
    // Boxes
    rt_instance_t *box1 =
        rt_instance_new((rt_hittable_t *)rt_box_new(point3(0, 0, 0), point3(165, 330, 165), rt_material_claim(white)));
    rt_instance_rotate_y(box1, 15);
    rt_instance_translate(box1, vec3(265, 0, 295));
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_const_medium_new_with_colour((rt_hittable_t *)box1, 0.01, colour(0.0, 0.0, 0.0)));

    rt_instance_t *box2 =
        rt_instance_new((rt_hittable_t *)rt_box_new(point3(0, 0, 0), point3(165, 165, 165), rt_material_claim(white)));
    rt_instance_rotate_y(box2, -18);
    rt_instance_translate(box2, vec3(130, 0, 65));
    rt_hittable_list_add(
        objects, (rt_hittable_t *)rt_const_medium_new_with_colour((rt_hittable_t *)box2, 0.01, colour(1.0, 1.0, 1.0)));

    // Light source
    rt_material_t *diffuse_light = (rt_material_t *)rt_mt_dl_new_with_albedo(colour(1, 1, 1), 7);
    rt_hittable_list_add(objects, (rt_hittable_t *)rt_aa_rect_new_xz(113, 443, 127, 432, 554, diffuse_light));

    rt_bvh_node_t *bvh = rt_bvh_node_new(objects, 0, 1);
    rt_hittable_list_t *result = rt_hittable_list_init(1);
    rt_hittable_list_add(
        result, (rt_hittable_t *)bvh);

    rt_hittable_list_deinit(objects);

    return result;
}