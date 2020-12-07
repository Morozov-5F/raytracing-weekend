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
    int cells_in_tile;

    double *randoms;
    int *x;
    int *y;
    int *z;
};

static int *generate_permutation(size_t size);
static void permutate(int *array, size_t size);

rt_perlin_t *rt_perlin_new(void)
{
    rt_perlin_t *result = calloc(1, sizeof(rt_perlin_t));
    assert(NULL != result);

    result->tile_size = 255;
    result->cells_in_tile = 4;

    result->x = generate_permutation(result->tile_size);
    result->y = generate_permutation(result->tile_size);
    result->z = generate_permutation(result->tile_size);

    result->randoms = malloc(sizeof(double) * result->tile_size);
    assert(NULL != result->randoms);
    for (size_t i = 0; i < result->tile_size; ++i)
    {
        result->randoms[i] = rt_random_double(0, 1.0);
    }

    return result;
}

double rt_perlin_noise(const rt_perlin_t *perlin, point3_t point)
{
    assert(NULL != perlin);

    int i = (int)(perlin->cells_in_tile * point.x) & perlin->tile_size;
    int j = (int)(perlin->cells_in_tile * point.y) & perlin->tile_size;
    int k = (int)(perlin->cells_in_tile * point.z) & perlin->tile_size;

    return perlin->randoms[perlin->x[i] ^ perlin->y[j] ^ perlin->z[k]];
}

double rt_perlin_blocky_noise(const rt_perlin_t *perlin, point3_t point)
{
    assert(NULL != perlin);
    // TODO: Figure out simple random tiled texture
    return 1.0;
}

void rt_perlin_delete(rt_perlin_t *perlin)
{
    if (NULL == perlin)
    {
        return;
    }
    free(perlin->x);
    free(perlin->y);
    free(perlin->z);
    free(perlin);
}

static int *generate_permutation(size_t size)
{
    int *permutation = malloc(sizeof(int) * size);
    assert(NULL != permutation);

    for (size_t i = 0; i < size; ++i)
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
