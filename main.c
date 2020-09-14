#include <stdio.h>
#include "vec3.h"

int main()
{
    // Image parameters
    int image_width = 256;
    int image_height = 256;

    // Render
    fprintf(stdout, "P3\n%d %d\n255\n", image_width, image_height);
    for (int j = 0; j < image_height; ++j)
    {
        fprintf(stderr, "\rScanlines remaining: %d", j);
        fflush(stderr);
        for (int i = 0; i < image_width; ++i)
        {
            colour pixel = vec3((double) i / (image_width - 1), (double) (image_height - j - 1) / (image_height - 1),
                                0.25);

            fprintf(stdout, "%d %d %d\n", (int) (vec3_x(&pixel) * 255.999), (int) (vec3_y(&pixel) * 255.99),
                    (int) (vec3_z(&pixel) * 255.99));
        }
    }
    fprintf(stderr, "\nDone\n");

    return 0;
}
