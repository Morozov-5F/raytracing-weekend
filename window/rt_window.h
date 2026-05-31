/**
 * Copyright (c) 2024, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdbool.h>
#include <rt_colour.h>

typedef struct rt_window_s rt_window_t;

rt_window_t *rt_window_init(const char *window_title, size_t width, size_t height);

bool rt_window_show(rt_window_t *window);

void rt_window_process(rt_window_t *window);

void rt_window_close(rt_window_t *window);

bool rt_window_display_image(rt_window_t *window, colour_t *image, size_t width, size_t height, size_t samples_per_pixel);
