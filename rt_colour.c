//
// Created by morozov on 16.09.2020.
//

#include <assert.h>
#include "rt_colour.h"

void rt_write_colour(FILE *stream, colour_t pixel_colour)
{
    assert(NULL != stream);

    fprintf(stream, "%d %d %d\n", (int)(pixel_colour.x * 255.99), (int)(pixel_colour.y * 255.99),
            (int)(pixel_colour.z * 255.99));
}
