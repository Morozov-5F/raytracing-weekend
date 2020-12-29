/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_perlin.h"
#include "rt_colour.h"

struct rt_perlin_s
{
    int tile_size;

    vec3_t *random_vectors;
    int *x;
    int *y;
    int *z;
};

static int *generate_permutation(size_t size);
static void permutate(int *array, size_t size);
static double_t perlin_interpolation(vec3_t c[2][2][2], double u, double v, double w);

rt_perlin_t *rt_perlin_new(void)
{
    rt_perlin_t *result = calloc(1, sizeof(rt_perlin_t));
    assert(NULL != result);

    result->tile_size = 255;

    result->x = generate_permutation(result->tile_size + 1);
    result->y = generate_permutation(result->tile_size + 1);
    result->z = generate_permutation(result->tile_size + 1);

    result->random_vectors = calloc(result->tile_size, sizeof(vec3_t));
    assert(NULL != result->random_vectors);
    for (size_t i = 0; i < result->tile_size; ++i)
    {
        result->random_vectors[i] = vec3_normalized(vec3_random(-1, 1));
    }

    return result;
}

double rt_perlin_noise(const rt_perlin_t *perlin, point3_t point)
{
    assert(NULL != perlin);

    double dx = point.x - floor(point.x);
    double dy = point.y - floor(point.y);
    double dz = point.z - floor(point.z);

    int i = (int)floor(point.x);
    int j = (int)floor(point.y);
    int k = (int)floor(point.z);

    vec3_t c[2][2][2];
    for (int di = 0; di < 2; di++)
    {
        for (int dj = 0; dj < 2; dj++)
        {
            for (int dk = 0; dk < 2; dk++)
            {
                c[di][dj][dk] = perlin->random_vectors[perlin->x[(i + di) & 255] ^ perlin->y[(j + dj) & 255] ^
                                                       perlin->z[(k + dk) & 255]];
            }
        }
    }

    return perlin_interpolation(c, dx, dy, dz);
}

double rt_perlin_blocky_noise(const rt_perlin_t *perlin, point3_t point)
{
    assert(NULL != perlin);
    // TODO: Figure out simple random tiled texture
    return 1.0;
}

double rt_perlin_turbulence(const rt_perlin_t *perlin, point3_t p, int depth)
{
    double accum = 0.0;
    vec3_t temp_p = p;
    double weight = 1.0;

    for (int i = 0; i < depth; i++)
    {
        accum += weight * rt_perlin_noise(perlin, temp_p);
        weight *= 0.5;
        vec3_scale_in_place(&temp_p, 2);
    }

    return fabs(accum);
}

void rt_perlin_delete(rt_perlin_t *perlin)
{
    if (NULL == perlin)
    {
        return;
    }
    free(perlin->random_vectors);
    free(perlin->x);
    free(perlin->y);
    free(perlin->z);
    free(perlin);
}

static int *generate_permutation(size_t size)
{
    int *permutation = malloc(sizeof(int) * size);
    assert(NULL != permutation);

    for (int i = 0; i < size; ++i)
    {
        permutation[i] = i;
    }

    permutate(permutation, size);

    return permutation;
}

static void permutate(int *array, size_t size)
{
    assert(NULL != array);

    for (int i = 0; i < size; ++i)
    {
        int target = i + rand() % (size - i);
        int tmp = array[i];
        array[i] = array[target];
        array[target] = tmp;
    }
}

static double_t perlin_interpolation(vec3_t c[2][2][2], double u, double v, double w)
{
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);

    double accum = 0.0;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                vec3_t weight = vec3(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) *
                         vec3_dot(c[i][j][k], weight);
            }
        }
    }
    return accum;
}
