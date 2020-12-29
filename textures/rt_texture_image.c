/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <rt_texture_shared.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

typedef struct rt_texture_image_s
{
    rt_texture_t base;

    unsigned char *image_data;
    int width, height;
    int bytes_per_scanline;
} rt_texture_image_t;

static colour_t rt_texture_image_value(const rt_texture_t *texture, double u, double v, const vec3_t *p);
static void rt_texture_image_delete(rt_texture_t *texture);

rt_texture_t *rt_texture_image_new(const char *filename)
{
    assert(NULL != filename);

    rt_texture_image_t *result = calloc(1, sizeof(rt_texture_image_t));
    assert(NULL != result);

    int channels_in_file = 3;
    result->image_data = stbi_load(filename, &result->width, &result->height, &channels_in_file, channels_in_file);
    if (NULL == result->image_data)
    {
        fprintf(stderr, "Unable to load texture image '%s'\n", filename);
    }
    rt_texture_init(&result->base, RT_TEXTURE_TYPE_IMAGE, rt_texture_image_value, rt_texture_image_delete);
    result->bytes_per_scanline = channels_in_file * result->width;

    return (rt_texture_t *)result;
}

static colour_t rt_texture_image_value(const rt_texture_t *texture, double u, double v, const vec3_t *p)
{
    assert(NULL != texture);
    assert(RT_TEXTURE_TYPE_IMAGE == texture->type);

    rt_texture_image_t *img = (rt_texture_image_t *)texture;

    if (NULL == img->image_data)
    {
        return colour(0, 1, 1);
    }

    u = rt_clamp(u, 0.0, 1.0);
    v = 1.0 - rt_clamp(v, 0.0, 1.0);

    int i = (int)(u * img->width);
    int j = (int)(v * img->height);

    i = i >= img->width ? img->width - 1 : i;
    j = j >= img->height ? img->width - 1 : j;

    unsigned char *pixel = img->image_data + i * 3 + j * img->bytes_per_scanline;
    return vec3_scale(colour(pixel[0], pixel[1], pixel[2]), 1.0 / 255.0);
}

static void rt_texture_image_delete(rt_texture_t *texture)
{
    if (NULL == texture)
    {
        return;
    }
    assert(RT_TEXTURE_TYPE_IMAGE == texture->type);
    rt_texture_image_t *img = (rt_texture_image_t *)texture;

    stbi_image_free(img->image_data);
    free(img);
}
