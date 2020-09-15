#include <stdio.h>

#include "rtweekend.h"
#include "sphere.h"
#include "hittable_list.h"

colour_t ray_colour(const ray_t *ray, const hittable_list_t *list)
{
    hit_record_t record;
    if (hittable_list_hit_test(list, ray, 0, INFINITY, &record))
    {
        return vec3_scale(vec3_sum(record.normal, vec3(1, 1, 1)), 0.5);
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

    // Camera parameters
    const double VIEWPORT_HEIGHT = 2.0;
    const double VIEWPORT_WIDTH = ASPECT_RATIO * VIEWPORT_HEIGHT;
    const double FOCAL_LENGTH = 1.0;

    point3_t origin = point3(0, 0, 0);
    point3_t horizontal = point3(VIEWPORT_WIDTH, 0, 0);
    point3_t vertical = point3(0, VIEWPORT_HEIGHT, 0);
    point3_t lower_left_corner = origin;
    vec3_sub(&lower_left_corner, vec3_scale(horizontal, 0.5));
    vec3_sub(&lower_left_corner, vec3_scale(vertical, 0.5));
    vec3_sub(&lower_left_corner, vec3(0, 0, FOCAL_LENGTH));

    // World
    hittable_list_t world;
    hittable_list_init(&world, 2);
    hittable_list_add(&world, (hittable_t *)sphere_new(vec3(0, 0, -1), 0.5));
    hittable_list_add(&world, (hittable_t *)sphere_new(vec3(0, -100.5, -1), 100));

    // Render
    fprintf(stdout, "P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
    {
        fprintf(stderr, "\rScanlines remaining: %d", j);
        fflush(stderr);
        for (int i = 0; i < IMAGE_WIDTH; ++i)
        {
            double u = (double)i / (IMAGE_WIDTH - 1);
            double v = (double)j / (IMAGE_HEIGHT - 1);

            point3_t ray_direction = lower_left_corner;
            vec3_add(&ray_direction, vec3_scale(horizontal, u));
            vec3_add(&ray_direction, vec3_scale(vertical, v));
            vec3_sub(&ray_direction, origin);

            ray_t ray = ray_init(origin, ray_direction);
            colour_t pixel = ray_colour(&ray, &world);
            fprintf(stdout, "%d %d %d\n", (int)(pixel.x * 255.99), (int)(pixel.y * 255.99),
                    (int)(pixel.z * 255.99));
        }
    }
    fprintf(stderr, "\nDone\n");

    // Cleanup
    hittable_list_deinit(&world);

    return 0;
}
