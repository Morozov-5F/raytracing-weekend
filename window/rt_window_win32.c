/**
 * Copyright (c) 2024, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "rt_window.h"

#include <stdio.h>
#include <stdlib.h>
#include <rt_colour.h>

#include <windows.h>

struct rt_window_s
{
    WNDCLASS cls;
    HWND window;
};

static LRESULT rt_win32_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

rt_window_t *rt_window_init(const char *window_title, size_t width, size_t height)
{
    rt_window_t *new_window = (rt_window_t *)calloc(1, sizeof(rt_window_t));
    if (NULL == new_window)
    {
        return NULL;
    }

    // First, create a window class
    new_window->cls.lpfnWndProc = (WNDPROC)rt_win32_wnd_proc;
    new_window->cls.hInstance = GetModuleHandle(0);
    new_window->cls.lpszClassName = "rt_wnd";

    // Then we should register it and get an atom which we can use to create a window later
    ATOM register_result = RegisterClassA(&new_window->cls);

    // Now we can create a windows with specified parameters
    new_window->window = CreateWindow(register_result, window_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                      width, height, NULL, NULL, new_window->cls.hInstance, NULL);

    if (NULL == new_window->window)
    {
        UnregisterClassA("rt_wnd", new_window->cls.hInstance);
        free(new_window);

        new_window = NULL;
    }

    return new_window;
}

bool rt_window_show(rt_window_t *window)
{
    return ShowWindow(window->window, SW_NORMAL);
}

void rt_window_process(rt_window_t *window)
{
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void rt_window_close(rt_window_t *window)
{

}

bool rt_window_display_image(rt_window_t *window, vec3_t *image, size_t width, size_t height, size_t samples_per_pixel)
{
    if (NULL == window || NULL == image)
    {
        return false;
    }

    // TODO: This routine is VERY slow, need to optimize it, maybe by displaying the full bitmap?
    HDC dc = GetDC(window->window);
    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            int r, g, b;
            rt_convert_to_rgb(image[j * width + i], samples_per_pixel, &r, &g, &b);
            SetPixel(dc, i, height - j, RGB(r, g, b));
        }
    }
    ReleaseDC(window->window, dc);
}

static LRESULT rt_win32_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Dispatch the messages that can be received.
    switch (message)
    {
        case WM_CHAR:
            switch (wParam)
            {
                case 'q':
                    fprintf(stderr, "Q IS PRESSED\n");
                    break;
            }
            break;

        case WM_CLOSE:
            if (MessageBox(hWnd, "Really quit?", "My application", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hWnd);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}