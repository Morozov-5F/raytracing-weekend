/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_texture.h"
#include "rt_texture_solid_colour.h"
#include "rt_texture_checker_pattern.h"
#include "rt_texture_noise.h"
#include "rt_texture_image.h"
#include <rt_texture_shared.h>
#include <assert.h>

colour_t rt_texture_value(const rt_texture_t *texture, double u, double v, const vec3_t *p)
{
    assert(NULL != texture);

    colour_t result;
    switch (texture->type)
    {
        case RT_TEXTURE_TYPE_SOLID_COLOUR:
            result = rt_texture_sc_value((const rt_texture_sc_t *)texture, u, v, p);
            break;

        case RT_TEXTURE_TYPE_CHECKER:
            result = rt_texture_cp_value((const rt_texture_cp_t *)texture, u, v, p);
            break;

        case RT_TEXTURE_TYPE_NOISE:
            result = rt_texture_noise_value((const rt_texture_noise_t *)texture, u, v, p);
            break;

        case RT_TEXTURE_TYPE_IMAGE:
            result = rt_texture_image_value((const rt_texture_image_t *)texture, u, v, p);
            break;

        default:
            assert(0);
    }
    return result;
}

rt_texture_t *rt_texture_claim(rt_texture_t *texture)
{
    assert(NULL != texture);

    texture->refcount++;
    return texture;
}

void rt_texture_delete(rt_texture_t *texture)
{
    assert(NULL != texture);

    if (--texture->refcount > 0)
    {
        return;
    }

    switch (texture->type)
    {
        case RT_TEXTURE_TYPE_SOLID_COLOUR:
            rt_texture_sc_delete((rt_texture_sc_t *)texture);
            break;

        case RT_TEXTURE_TYPE_CHECKER:
            rt_texture_cp_delete((rt_texture_cp_t *)texture);
            break;

        case RT_TEXTURE_TYPE_NOISE:
            rt_texture_noise_delete((rt_texture_noise_t *)texture);
            break;

        case RT_TEXTURE_TYPE_IMAGE:
            rt_texture_image_delete((rt_texture_image_t *)texture);
            break;

        default:
            assert(0);
    }
}

void rt_texture_init(rt_texture_t *texture, rt_texture_type_t type)
{
    assert(NULL != texture);
    texture->refcount = 1;
    texture->type = type;
}
