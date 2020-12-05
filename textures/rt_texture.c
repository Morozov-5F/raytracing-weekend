/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_texture.h"
#include "rt_texture_solid_colour.h"
#include <rt_texture_shared.h>
#include <assert.h>

colour_t rt_texture_value(const rt_texture_t *texture, double u, double v, const vec3_t *p)
{
    colour_t result;
    switch (texture->type)
    {
        case RT_TEXTURE_TYPE_SOLID_COLOUR:
            result = rt_texture_sc_value((const rt_texture_sc_t *)texture, u, v, p);
            break;
        default:
            assert(0);
    }
    return result;
}

void rt_texture_delete(rt_texture_t *texture)
{
    switch (texture->type)
    {
        case RT_TEXTURE_TYPE_SOLID_COLOUR:
            rt_texture_sc_delete((rt_texture_sc_t *)texture);
            break;

        default:
            assert(0);
    }
}
