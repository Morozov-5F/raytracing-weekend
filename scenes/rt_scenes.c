/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_scenes.h"

#include <rt_weekend.h>

#include <rt_material.h>
#include <rt_hittable.h>
#include <rt_texture.h>
#include <rt_bvh.h>
#include <rt_aa_rect.h>
#include <rt_instance.h>
#include <string.h>
#include <assert.h>

rt_hittable_list_t *rt_scene_random(void)
{
    rt_material_t *ground_material =
        rt_mt_diffuse_new_with_texture(rt_texture_cp_new_with_colour(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9)));

    rt_hittable_list_t *world = rt_hittable_list_init(500);
    rt_hittable_list_add(world, rt_sphere_new(point3(0, -1000, 0), 1000, ground_material));

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
                    sphere_material = rt_mt_diffuse_new_with_albedo(albedo);
                    point3_t end_center = vec3_sum(center, vec3(0, rt_random_double(0, 0.5), 0));

                    object_to_add = rt_moving_sphere_new(center, end_center, 0.0, 1.0, 0.2, sphere_material);
                }
                else if (material_chooser < 0.95)
                {
                    colour_t albedo = vec3_random(0.5, 1);
                    double fuzz = rt_random_double(0, 0.5);
                    sphere_material = rt_mt_metal_new(albedo, fuzz);

                    object_to_add = rt_sphere_new(center, 0.2, sphere_material);
                }
                else
                {
                    sphere_material = rt_mt_dielectric_new(1.5);

                    object_to_add = rt_sphere_new(center, 0.2, sphere_material);
                }
                rt_hittable_list_add(world, object_to_add);
            }
        }
    }

    rt_material_t *material1 = rt_mt_dielectric_new(1.5);
    rt_hittable_list_add(world, rt_sphere_new(point3(0, 1, 0), 1.0, material1));

    rt_material_t *material2 = rt_mt_diffuse_new_with_albedo(colour(0.4, 0.2, 0.1));
    rt_hittable_list_add(world, rt_sphere_new(point3(-4, 1, 0), 1.0, material2));

    rt_material_t *material3 = rt_mt_metal_new(colour(0.7, 0.6, 0.5), 0.0);
    rt_hittable_list_add(world, rt_sphere_new(point3(4, 1, 0), 1.0, material3));

    rt_hittable_list_t *result = rt_hittable_list_init(1);
    rt_hittable_list_add(result, rt_bvh_node_new(world, 0.0, 1.0));

    rt_hittable_list_deinit(world);

    return result;
}

rt_hittable_list_t *rt_scene_two_spheres(void)
{
    rt_material_t *checker =
        rt_mt_diffuse_new_with_texture(rt_texture_cp_new_with_colour(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9)));

    rt_hittable_list_t *objects = rt_hittable_list_init(2);

    rt_hittable_list_add(objects, rt_sphere_new(point3(0, -10, 0), 10, checker));
    rt_hittable_list_add(objects, rt_sphere_new(point3(0, 10, 0), 10, rt_material_claim(checker)));

    return objects;
}

rt_hittable_list_t *rt_scene_two_perlin_spheres(void)
{
    rt_material_t *noisy = rt_mt_diffuse_new_with_texture(rt_texture_noise_new(4));

    rt_hittable_list_t *objects = rt_hittable_list_init(2);

    rt_hittable_list_add(objects, rt_sphere_new(point3(0, -1000, 0), 1000, noisy));
    rt_hittable_list_add(objects, rt_sphere_new(point3(0, 2, 0), 2, rt_material_claim(noisy)));

    return objects;
}

rt_hittable_list_t *rt_scene_earth(void)
{
    rt_material_t *earth_material =
        rt_mt_diffuse_new_with_texture(rt_texture_image_new("assets/textures/earth_projection.jpg"));

    rt_hittable_list_t *objects = rt_hittable_list_init(1);

    rt_hittable_list_add(objects, rt_sphere_new(point3(0, 0, 0), 2, earth_material));

    return objects;
}

rt_hittable_list_t *rt_scene_light_sample(void)
{
    rt_material_t *noisy = rt_mt_diffuse_new_with_texture(rt_texture_noise_new(4));

    rt_hittable_list_t *objects = rt_hittable_list_init(3);

    rt_hittable_list_add(objects, rt_sphere_new(point3(0, -1000, 0), 1000, noisy));
    rt_hittable_list_add(objects, rt_sphere_new(point3(0, 2, 0), 2, rt_material_claim(noisy)));

    rt_material_t *diffuse_light = rt_mt_dl_new_with_albedo(colour(0.7, 0.8, 0.6), 4);
    rt_hittable_list_add(objects, rt_aa_rect_new_xy(3, 5, 1, 3, -2, diffuse_light));

    return objects;
}

rt_hittable_list_t *rt_scene_cornell_box(void)
{
    rt_hittable_list_t *objects = rt_hittable_list_init(8);

    rt_material_t *red = rt_mt_diffuse_new_with_albedo(colour(0.65, 0.05, 0.05));
    rt_material_t *green = rt_mt_diffuse_new_with_albedo(colour(0.12, 0.45, 0.15));
    rt_material_t *white = rt_mt_diffuse_new_with_albedo(colour(0.73, 0.73, 0.73));

    // Walls
    rt_hittable_list_add(objects, rt_aa_rect_new_yz(0, 555, 0, 555, 555, green));
    rt_hittable_list_add(objects, rt_aa_rect_new_yz(0, 555, 0, 555, 0, red));

    rt_hittable_list_add(objects, rt_aa_rect_new_xz(0, 555, 0, 555, 0, white));
    rt_hittable_list_add(objects, rt_aa_rect_new_xz(0, 555, 0, 555, 555, rt_material_claim(white)));
    rt_hittable_list_add(objects, rt_aa_rect_new_xy(0, 555, 0, 555, 555, rt_material_claim(white)));
    // Boxes
    rt_hittable_t *box1 = rt_instance_new(rt_box_new(point3(0, 0, 0), point3(165, 330, 165), rt_material_claim(white)));
    rt_instance_rotate_y(box1, 15);
    rt_instance_translate(box1, vec3(265, 0, 295));
    rt_hittable_list_add(objects, box1);

    rt_hittable_t *box2 = rt_instance_new(rt_box_new(point3(0, 0, 0), point3(165, 165, 165), rt_material_claim(white)));
    rt_instance_rotate_y(box2, -18);
    rt_instance_translate(box2, vec3(130, 0, 65));
    rt_hittable_list_add(objects, box2);

    // Light source
    rt_material_t *diffuse_light = rt_mt_dl_new_with_albedo(colour(1, 1, 1), 15);
    rt_hittable_list_add(objects, rt_aa_rect_new_xz(213, 343, 227, 332, 554, diffuse_light));

    rt_hittable_list_t *result = rt_hittable_list_init(1);
    rt_hittable_list_add(result, rt_bvh_node_new(objects, 0, 1));

    rt_hittable_list_deinit(objects);

    return result;
}

rt_hittable_list_t *rt_scene_instance_test(void)
{
    rt_material_t *green = rt_mt_diffuse_new_with_albedo(colour(0.12, 0.45, 0.15));
    rt_material_t *red = rt_mt_diffuse_new_with_albedo(colour(0.65, 0.05, 0.05));

    rt_hittable_list_t *objects = rt_hittable_list_init(2);

    rt_hittable_t *box = rt_box_new(point3(-1, -1, -1), point3(1, 1, 1), green);
    rt_hittable_t *instance = rt_instance_new(box);
    rt_instance_rotate_y(instance, 0);
    rt_instance_translate(instance, point3(1, 0, 0));

    rt_hittable_list_add(objects, instance);
    rt_hittable_list_add(objects, rt_box_new(point3(-0.25, -0.25, -0.25), point3(0.25, 0.25, 0.25), red));

    rt_hittable_list_t *result = rt_hittable_list_init(1);
    rt_hittable_list_add(result, rt_bvh_node_new(objects, 0, 1));

    rt_hittable_list_deinit(objects);

    return result;
}

rt_hittable_list_t *rt_scene_cornell_box_smoke_boxes(void)
{
    rt_hittable_list_t *objects = rt_hittable_list_init(8);

    rt_material_t *red = rt_mt_diffuse_new_with_albedo(colour(0.65, 0.05, 0.05));
    rt_material_t *green = rt_mt_diffuse_new_with_albedo(colour(0.12, 0.45, 0.15));
    rt_material_t *white = rt_mt_diffuse_new_with_albedo(colour(0.73, 0.73, 0.73));

    // Walls
    rt_hittable_list_add(objects, rt_aa_rect_new_yz(0, 555, 0, 555, 555, green));
    rt_hittable_list_add(objects, rt_aa_rect_new_yz(0, 555, 0, 555, 0, red));

    rt_hittable_list_add(objects, rt_aa_rect_new_xz(0, 555, 0, 555, 0, white));
    rt_hittable_list_add(objects, rt_aa_rect_new_xz(0, 555, 0, 555, 555, rt_material_claim(white)));
    rt_hittable_list_add(objects, rt_aa_rect_new_xy(0, 555, 0, 555, 555, rt_material_claim(white)));
    // Boxes
    rt_hittable_t *box1 = rt_instance_new(rt_box_new(point3(0, 0, 0), point3(165, 330, 165), rt_material_claim(white)));
    rt_instance_rotate_y(box1, 15);
    rt_instance_translate(box1, vec3(265, 0, 295));
    rt_hittable_list_add(objects, rt_const_medium_new_with_colour(box1, 0.01, colour(0.0, 0.0, 0.0)));

    rt_hittable_t *box2 = rt_instance_new(rt_box_new(point3(0, 0, 0), point3(165, 165, 165), rt_material_claim(white)));
    rt_instance_rotate_y(box2, -18);
    rt_instance_translate(box2, vec3(130, 0, 65));
    rt_hittable_list_add(objects, rt_const_medium_new_with_colour(box2, 0.01, colour(1.0, 1.0, 1.0)));

    // Light source
    rt_material_t *diffuse_light = rt_mt_dl_new_with_albedo(colour(1, 1, 1), 7);
    rt_hittable_list_add(objects, rt_aa_rect_new_xz(113, 443, 127, 432, 554, diffuse_light));

    rt_hittable_t *bvh = rt_bvh_node_new(objects, 0, 1);
    rt_hittable_list_t *result = rt_hittable_list_init(1);
    rt_hittable_list_add(result, bvh);

    rt_hittable_list_deinit(objects);

    return result;
}

rt_hittable_list_t *rt_scene_showcase(void)
{
    const int BOXES_PER_SIDE = 20;

    // Add ground boxes
    rt_hittable_list_t *ground_boxes = rt_hittable_list_init(BOXES_PER_SIDE * BOXES_PER_SIDE);
    rt_material_t *ground = rt_mt_diffuse_new_with_albedo(colour(0.48, 0.83, 0.53));

    for (int i = 0; i < BOXES_PER_SIDE; ++i)
    {
        for (int j = 0; j < BOXES_PER_SIDE; ++j)
        {
            double w = 100.0;
            double x0 = -1000.0 + i * w;
            double z0 = -1000.0 + j * w;
            double y0 = 0.0;
            double x1 = x0 + w;
            double y1 = rt_random_double(1, 101);
            double z1 = z0 + w;

            rt_hittable_list_add(ground_boxes,
                                 rt_box_new(point3(x0, y0, z0), point3(x1, y1, z1), rt_material_claim(ground)));
        }
    }

    rt_hittable_list_t *objects = rt_hittable_list_init(40);
    rt_hittable_list_add(objects, rt_bvh_node_new(ground_boxes, 0, 1));

    // Add a light source
    rt_material_t *light = rt_mt_dl_new_with_albedo(colour(1, 1, 1), 7);
    rt_hittable_list_add(objects, rt_aa_rect_new_xz(123, 423, 147, 412, 554, light));

    // Add a moving sphere
    point3_t center1 = point3(400, 400, 200);
    point3_t center2 = vec3_sum(center1, vec3(30, 0, 0));

    rt_material_t *moving_sphere_material = rt_mt_diffuse_new_with_albedo(colour(0.7, 0.3, 0.1));
    rt_hittable_list_add(objects, rt_moving_sphere_new(center1, center2, 0, 1, 50, moving_sphere_material));

    // Add glass sphere
    rt_hittable_list_add(objects, rt_sphere_new(point3(260, 150, 45), 50, rt_mt_dielectric_new(1.5)));

    // Add metal sphere
    rt_hittable_t *metal_sphere = rt_sphere_new(point3(0, 150, 145), 50, rt_mt_metal_new(colour(0.8, 0.8, 0.9), 1.0));
    rt_hittable_list_add(objects, metal_sphere);

    // Sphere with glossy surface and fog inside
    rt_hittable_t *boundary = rt_sphere_new(point3(360, 150, 145), 70, rt_mt_dielectric_new(1.5));
    rt_hittable_list_add(objects, boundary);
    rt_hittable_list_add(objects,
                         rt_const_medium_new_with_colour(rt_hittable_claim(boundary), 0.2, colour(0.2, 0.4, 0.9)));

    // Global fog and distortion
    boundary = rt_sphere_new(point3(0, 0, 0), 5000, rt_mt_dielectric_new(1.5));
    rt_hittable_list_add(objects, rt_const_medium_new_with_colour(boundary, 0.0001, colour(1, 1, 1)));

    // Add Earth
    rt_material_t *earth = rt_mt_diffuse_new_with_texture(rt_texture_image_new("assets/textures/earth_projection.jpg"));
    rt_hittable_list_add(objects, rt_sphere_new(point3(400, 200, 400), 100, earth));

    // Add Perlin noise texture
    rt_texture_t *perlin = rt_texture_noise_new(0.1);
    rt_hittable_list_add(objects, rt_sphere_new(point3(220, 280, 300), 80, rt_mt_diffuse_new_with_texture(perlin)));

    const int NUMBER_OF_SPHERES = 1000;
    rt_hittable_list_t *spheres = rt_hittable_list_init(NUMBER_OF_SPHERES);
    rt_material_t *white = rt_mt_diffuse_new_with_albedo(colour(.73, .73, .73));

    for (int i = 0; i < NUMBER_OF_SPHERES; ++i)
    {
        rt_hittable_list_add(spheres, rt_sphere_new(vec3_random(0, 165), 10, rt_material_claim(white)));
    }

    rt_hittable_t *spheres_instance = rt_instance_new(rt_bvh_node_new(spheres, 0, 1));
    rt_instance_rotate_y(spheres_instance, 15);
    rt_instance_translate(spheres_instance, point3(-100, 270, 395));
    rt_hittable_list_add(objects, spheres_instance);

    rt_material_delete(white);
    rt_material_delete(ground);
    rt_hittable_list_deinit(ground_boxes);
    rt_hittable_list_deinit(spheres);

    return objects;
}

rt_hittable_list_t *rt_scene_metal_test(void)
{
    rt_material_t *checker =
        rt_mt_diffuse_new_with_texture(rt_texture_cp_new_with_colour(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9)));

    rt_hittable_list_t *objects = rt_hittable_list_init(5);

    rt_hittable_list_add(objects, rt_sphere_new(point3(0, -1000, 0), 1000, checker));

    rt_hittable_list_add(objects, rt_sphere_new(point3(-2, 2, 0), 1, rt_mt_metal_new(colour(0.8, 0.8, 0.9), 0.0)));
    rt_hittable_list_add(objects, rt_sphere_new(point3(0, 2, 0), 1, rt_mt_metal_new(colour(0.8, 0.8, 0.9), 0.5)));
    rt_hittable_list_add(objects, rt_sphere_new(point3(2, 2, 0), 1, rt_mt_metal_new(colour(0.8, 0.8, 0.9), 1.0)));

    rt_hittable_t *box_instance =
        rt_instance_new(rt_box_new(point3(-1, 0, 10), point3(1, 2, 12), rt_mt_metal_new(colour(0.8, 0.8, 0.9), 0.0)));
    rt_instance_rotate_y(box_instance, 10);
    rt_hittable_list_add(objects, box_instance);

    return objects;
}

typedef struct rt_scenes_s
{
    rt_scene_id_t id;
    const char *name;
    const char *desc;
} rt_scenes_t;

static rt_scenes_t gs_scenes[] = {
    {RT_SCENE_RANDOM, "random_spheres", "Scene from \'Ray Tracing In One Weekend\' cover (with checker texture)"},
    {RT_SCENE_TWO_SPHERES, "two_spheres", "Two checkered spheres"},
    {RT_SCENE_TWO_PERLIN_SPHERES, "perlin_spheres", "Two spheres with Perlin-based textures"},
    {RT_SCENE_EARTH, "earth", "Sphere with Earth texture"},
    {RT_SCENE_LIGHT_SAMPLE, "light_sample", "Sphere with a light source near it"},
    {RT_SCENE_CORNELL_BOX, "cornell_box", "Cornell box scene"},
    {RT_SCENE_INSTANCE_TEST, "instance_test", "Several rotated and translated cubes"},
    {RT_SCENE_CORNELL_SMOKE, "cornell_smoke", "Cornell box scene but boxes as made of smoke"},
    {RT_SCENE_SHOWCASE, "showcase", "Scene from \'Ray Tracing: The Next Week\' cover"},
    {RT_SCENE_METAL_TEST, "metal_test", "Test for metal texture (several metal spheres)"},
};

rt_scene_id_t rt_scene_get_id_by_name(const char *name)
{
    if (NULL == name)
    {
        return RT_SCENE_NONE;
    }

    for (int i = 0; i < sizeof(gs_scenes)/sizeof(gs_scenes[0]); ++i)
    {
        if (0 == strcmp(gs_scenes[i].name, name))
        {
            return gs_scenes[i].id;
        }
    }

    return RT_SCENE_NONE;
}

const char *rt_scene_get_name_by_id(rt_scene_id_t scene_id)
{
    for (int i = 0; i < sizeof(gs_scenes)/sizeof(gs_scenes[0]); ++i)
    {
        if (gs_scenes[i].id == scene_id)
        {
            return gs_scenes[i].name;
        }
    }

    return NULL;
}

void rt_scene_print_scenes_info(FILE *to)
{
    assert(NULL != to);

    for (int i = 0; i < sizeof(gs_scenes)/sizeof(gs_scenes[0]); ++i)
    {
        fprintf(to, "\t%-30s  %s\n", gs_scenes[i].name, gs_scenes[i].desc);
    }
}
