/**
 * Copyright (c) 2026, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <rt_window.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct rt_window_s
{
    Display *display;
    Window window;
    Atom window_close;
};

static XImage *convert_colour_to_bitmap(rt_window_t *window, const colour_t *image, size_t width, size_t height,
                                        size_t samples_per_pixel);

rt_window_t *rt_window_init(const char *window_title, size_t width, size_t height)
{
    rt_window_t *handle = (rt_window_t *)calloc(1, sizeof(rt_window_t));
    if (NULL == handle)
    {
        return NULL;
    }

    // First, connect to the X11 display server and get the root window out of it
    Display *display = XOpenDisplay(NULL);
    if (NULL == display)
    {
        fprintf(stderr, "rt_window_init: cannot open display\n");
        free(handle);
        return NULL;
    }
    Window rootWindow = XDefaultRootWindow(display);

    // Next, we can create the simple window based on the root window we've just got
    Window window = XCreateSimpleWindow(display, rootWindow, 0, 0, width, height, 0, 0, 0);
    XStoreName(display, window, window_title);
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // Tell the display server to handle the close button (WM_DELETE_WINDOW)
    Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete, 1);

    handle->display = display;
    handle->window = window;
    handle->window_close = wm_delete;

    return handle;
}

bool rt_window_show(rt_window_t *window)
{
    // Showing the windows means mapping it in X11
    XMapWindow(window->display, window->window);
    XFlush(window->display);
    return false;
}

void rt_window_process(rt_window_t *window)
{
    XEvent event;
    while (1)
    {
        XNextEvent(window->display, &event);
        switch (event.type)
        {
            case Expose:
                break;
            case KeyPress:
                break;
            case ClientMessage:
                // Handle closing the window
                if ((Atom)event.xclient.data.l[0] == window->window_close)
                {
                    return;
                }
                break;
        }
    }
}

void rt_window_close(rt_window_t *window)
{
    XDestroyWindow(window->display, window->window);
    XCloseDisplay(window->display);

    free(window);
}

bool rt_window_display_image(rt_window_t *window, colour_t *image, size_t width, size_t height,
                             size_t samples_per_pixel)
{
    XImage *bitmap = convert_colour_to_bitmap(window, image, width, height, samples_per_pixel);

    GC gc = XDefaultGC(window->display, XDefaultScreen(window->display));

    // Copy the image to the window
    XPutImage(window->display, window->window, gc, bitmap, 0, 0, // source x, y
              0, 0,                                              // destination x, y
              width, height);

    XFlush(window->display);
    XDestroyImage(bitmap);

    return false;
}

static XImage *convert_colour_to_bitmap(rt_window_t *window, const colour_t *image, size_t width, size_t height,
                                        size_t samples_per_pixel)
{
    uint32_t *pixel_data = calloc(width * height, sizeof(uint32_t));

    if (NULL == pixel_data)
    {
        return NULL;
    }

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            int r, g, b;
            rt_convert_to_rgb(image[(height - y - 1) * width + x], samples_per_pixel, &r, &g, &b);
            pixel_data[y * width + x] = (r << 16) | (g << 8) | b;
        }
    }

    XImage *bitmap = XCreateImage(window->display, XDefaultVisual(window->display, XDefaultScreen(window->display)),
                                  XDefaultDepth(window->display, XDefaultScreen(window->display)), ZPixmap, 0,
                                  (char *)pixel_data, width, height, 32, 0);

    return bitmap;
}