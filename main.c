#include <stdio.h>
#include "vec3.h"
#include "ray.h"

#include <stdbool.h>

bool hit_sphere(const point3_t *center, double radius, const ray_t *ray)
{
    vec3_t ac = vec3_diff(ray->origin, *center);
    double a = vec3_dot(ray->direction, ray->direction);
    double b = vec3_dot(ray->direction, ac);
    double c = vec3_dot(ac, ac) - radius * radius;

    double discriminant_4 = b * b - a * c;
    return discriminant_4 >= 0;
}

colour_t ray_colour(const ray_t *ray)
{
    point3_t sphere_center = vec3(0, 0, -1);
    if (hit_sphere(&sphere_center, 0.5, ray))
    {
        return vec3(1, 0, 0);
    }

    point3_t unit_direction = vec3_normalized(ray->direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    return vec3_lerp(vec3(1, 1, 1), vec3(0.5, 0.7, 1), t);
}

int main()
{
    // Image parameters
    const double ASPECT_RATIO = 16.0 / 9.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = (int)(IMAGE_WIDTH / ASPECT_RATIO);

    // Camera parameters
    const double VIEWPORT_HEIGHT = 2.0;
    const double VIEWPORT_WIDTH = ASPECT_RATIO * VIEWPORT_HEIGHT;
    const double FOCAL_LENGTH = 1.0;

    point3_t origin = vec3(0, 0, 0);
    point3_t horizontal = vec3(VIEWPORT_WIDTH, 0, 0);
    point3_t vertical = vec3(0, VIEWPORT_HEIGHT, 0);
    point3_t lower_left_corner = origin;
    vec3_sub(&lower_left_corner, vec3_scale(horizontal, 0.5));
    vec3_sub(&lower_left_corner, vec3_scale(vertical, 0.5));
    vec3_sub(&lower_left_corner, vec3(0, 0, FOCAL_LENGTH));

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
            colour_t pixel = ray_colour(&ray);
            fprintf(stdout, "%d %d %d\n", (int)(pixel.x * 255.99), (int)(pixel.y * 255.99),
                    (int)(pixel.z * 255.99));
        }
    }
    fprintf(stderr, "\nDone\n");

    return 0;
}
