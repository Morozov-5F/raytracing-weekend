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

    size_t width, height;
};

static LRESULT rt_win32_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

rt_window_t *rt_window_init(const char *window_title, size_t width, size_t height)
{
    rt_window_t *new_window = calloc(1, sizeof(rt_window_t));
    if (NULL == new_window)
    {
        return NULL;
    }

    // First, create a window class
    new_window->cls.lpfnWndProc = (WNDPROC)rt_win32_wnd_proc;
    new_window->cls.hInstance = GetModuleHandle(0);
    new_window->cls.lpszClassName = "rt_wnd";
    new_window->width = width;
    new_window->height = height;

    // Then we should register it and get an atom which we can use to create a window later
    ATOM register_result = RegisterClassA(&new_window->cls);

    // Now we can create a windows with specified parameters
    new_window->window = CreateWindow(MAKEINTATOM(register_result), window_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
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

static RGBTRIPLE* convert_colour_to_bitmap(const colour_t *image, size_t width, size_t height, size_t samples_per_pixel, BITMAPINFO *bitmap_info)
{
    RGBTRIPLE *bitmap = calloc(width * height, sizeof(RGBTRIPLE));

    if (NULL == bitmap || NULL == bitmap_info)
    {
        return NULL;
    }

    for (size_t i = 0; i < width * height; i++)
    {
        int r, g, b;
        rt_convert_to_rgb(image[i], samples_per_pixel, &r, &g, &b);
        bitmap[i].rgbtRed = r;
        bitmap[i].rgbtGreen = g;
        bitmap[i].rgbtBlue = b;
    }

    bitmap_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info->bmiHeader.biWidth = (LONG)width;
    bitmap_info->bmiHeader.biHeight = (LONG)height;
    bitmap_info->bmiHeader.biPlanes = 1;
    bitmap_info->bmiHeader.biBitCount = 24;
    bitmap_info->bmiHeader.biCompression = BI_RGB;
    bitmap_info->bmiHeader.biSizeImage = 0;
    bitmap_info->bmiHeader.biXPelsPerMeter = 0;
    bitmap_info->bmiHeader.biYPelsPerMeter = 0;
    bitmap_info->bmiHeader.biClrUsed = 0;
    bitmap_info->bmiHeader.biClrImportant = 0;

    return bitmap;
}

bool rt_window_display_image(rt_window_t *window, colour_t *image, size_t width, size_t height, size_t samples_per_pixel)
{
    if (NULL == window || NULL == image)
    {
        return false;
    }

    HDC dc = GetDC(window->window);
    int rasterCaps = GetDeviceCaps(dc, RASTERCAPS);

    BITMAPINFO bitmap_info;
    RGBTRIPLE *pixel_data = convert_colour_to_bitmap(image, width, height, samples_per_pixel, &bitmap_info);
    if (NULL == pixel_data)
    {
        fprintf(stderr, "Could not convert to RGBTRIPLE\n");
    }

    int res = 0;
    if (rasterCaps & RC_STRETCHDIB)
    {
        res = StretchDIBits(dc, 0, 0, window->width, window->height, 0, 0, width, height, pixel_data, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);
    }
    else
    {
        res = SetDIBitsToDevice(dc, 0, 0, width, height, 0, 0, 0, height, pixel_data, &bitmap_info, DIB_RGB_COLORS);
    }

    if (!res)
    {
        fprintf(stderr, "SetDIBitsToDevice failed\n");
    }

    free(pixel_data);

    ReleaseDC(window->window, dc);

    return true;
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
            if (MessageBox(hWnd, "Really quit?", "RT Weekend", MB_OKCANCEL) == IDOK)
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