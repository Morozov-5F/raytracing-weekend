#include <stdio.h>

#include "rtweekend.h"
#include "rt_sphere.h"
#include "rt_hittable_list.h"
#include "rt_camera.h"
#include "rt_colour.h"
#include "rt_material.h"
#include "rt_material_diffuse.h"

colour_t ray_colour(const ray_t *ray, const rt_hittable_list_t *list, int child_rays)
{
    if (child_rays <= 0)
    {
        return colour3(0, 0, 0);
    }

    rt_hit_record_t record;
    if (rt_hittable_list_hit_test(list, ray, 0.01, INFINITY, &record))
    {
        ray_t scattered;
        colour_t attenuation;
        if (rt_material_scatter(record.material, &record, &attenuation, &scattered))
        {
            return vec3_multiply(attenuation, ray_colour(&scattered, list, child_rays - 1));
        }
        return colour3(0, 0, 0);
    }
    point3_t unit_direction = vec3_normalized(ray->direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    return vec3_lerp(vec3(1, 1, 1), vec3(0.5, 0.7, 1), t);
}

int main()
{
    // Image parameters
    const double ASPECT_RATIO = 16.0 / 9.0;
    const int IMAGE_WIDTH = 1024;
    const int IMAGE_HEIGHT = (int)(IMAGE_WIDTH / ASPECT_RATIO);
    const int SAMPLES_PER_PIXEL = 100;
    const int CHILD_RAYS = 50;

    // Camera parameters
    rt_camera_t *camera = rt_camera_new();

    // Materials
    rt_material_t *small_sphere_material = (rt_material_t *)rt_mt_diffuse_new(colour3(0.3, 0.3, 0.3));
    rt_material_t *big_sphere_material = (rt_material_t *)rt_mt_diffuse_new(colour3(0.1, 0.1, 0.5));

    // World
    rt_hittable_list_t *world = rt_hittable_list_init(2);
    rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(vec3(0, 0, -1), 0.5, small_sphere_material));
    rt_hittable_list_add(world, (rt_hittable_t *)rt_sphere_new(vec3(0, -100.5, -1), 100, big_sphere_material));

    // Render
    fprintf(stdout, "P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
    {
        fprintf(stderr, "\rScanlines remaining: %d", j);
        fflush(stderr);
        for (int i = 0; i < IMAGE_WIDTH; ++i)
        {
            colour_t pixel = colour3(0, 0, 0);
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
            {
                double u = (double)(i + rt_random_double(0, 1))  / (IMAGE_WIDTH - 1);
                double v = (double)(j + rt_random_double(0, 1)) / (IMAGE_HEIGHT - 1);

                ray_t ray = rt_camera_get_ray(camera, u, v);
                vec3_add(&pixel, ray_colour(&ray, world, CHILD_RAYS));
            }
            rt_write_colour(stdout, pixel, SAMPLES_PER_PIXEL);
        }
    }
    fprintf(stderr, "\nDone\n");

    // Cleanup
    rt_hittable_list_deinit(world);
    rt_camera_delete(camera);

    rt_material_delete(small_sphere_material);
    rt_material_delete(big_sphere_material);

    return 0;
}
