#pragma once

#include "functions.h"
#include "color.h"
#include "draw.h"

#include <iostream>

class Painter {
public:
    PAINTSTRUCT ps;
    HBRUSH hBrush;
    HDC hdc;
    HWND hWnd;
    RECT rc;

    Color backgroundColor{ 117, 193, 255 };
    Color gridColor{ 255, 36, 0 };
    Color crossColor{ 43, 181, 43 };
    Color circleColor{ 255, 104, 0 };

    int grid_width = 4;
    int cross_width = 10;
    int ellips_width = cross_width;

    int padding = 11;

    int cells_num;

    Painter(HWND _hWnd, Color _background_color, int _cells_num) {
        set_default(_hWnd);
        backgroundColor = _background_color;
        cells_num = _cells_num;
        std::cout << "create painter cells_num=" << cells_num << std::endl;
    }

    void set_background() {
        set_background_color(backgroundColor.toRGB(), hWnd, hdc);
    }

    void draw_grid() {
        double dx = (double)(get_width(hWnd)) / (double)cells_num;
        double dy = (double)(get_height(hWnd)) / (double)cells_num;

        GetClientRect(hWnd, &rc);

        for (double x = rc.left + dx; x < rc.right; x += dx) {
            draw_line((int)x, 0, int(x), rc.bottom, grid_width, gridColor.toRGB(), hdc);
        }
        for (double y = rc.top + dy; y < rc.bottom; y += dy) {
            draw_line(0, (int)y, rc.right, int(y), grid_width, gridColor.toRGB(), hdc);
        }
    }

    void draw_cross_on_field(int i, int j, int width) {
        int left_x, up_y, dx, dy;
        get_grid_rect(hWnd, cells_num, i, j, left_x, up_y, dx, dy, padding);
        draw_cross(left_x, up_y, dx, dy, width, crossColor.toRGB(), hdc);
    }

    void draw_ellips_on_field(int i, int j, int width) {
        int left_x, up_y, dx, dy;
        get_grid_rect(hWnd, cells_num, i, j, left_x, up_y, dx, dy, padding);
        draw_ellips(left_x, up_y, dx, dy, width, circleColor.toRGB(), hdc);
    }


private:
    void set_default(HWND _hWnd) {
        hWnd = _hWnd;
        GetClientRect(hWnd, &rc);
        hdc = BeginPaint(hWnd, &ps);
        SelectObject(hdc, hBrush);
        SetBkMode(hdc, TRANSPARENT);
    }
};