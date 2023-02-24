#pragma once

#include "functions.h"
#include "color.h"
#include <iostream>

class Painter {
public:
    PAINTSTRUCT ps;
    HBRUSH hBrush;
    HDC hdc;
    HWND hWnd;
    RECT rc;

    Color backgroundColor{117, 193, 255};
    Color gridColor{255, 36, 0};
    Color crossColor{43, 181, 43};
    Color circleColor{255, 104, 0};

    int grid_width = 4;
    int cross_width = 10;
    int ellips_width = cross_width;

    int padding = 11;

    Painter (HWND _hWnd) {
        set_default(_hWnd);
    }

    Painter(HWND _hWnd, Color _background_color) {
        set_default(_hWnd);
        backgroundColor = _background_color;
    }

    void set_background() {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rc);
        SetDCBrushColor(hdc, backgroundColor.toRGB());
        GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
        //EndPaint(hWnd, &ps);
    }

    void draw_grid(int cells_num) {
        double dx = (double)(get_width(hWnd)) / (double)cells_num;
        double dy = (double)(get_height(hWnd)) / (double)cells_num;
        
        GetClientRect(hWnd, &rc);

        for (double x = rc.left + dx; x < rc.right; x += dx) {
            draw_line((int)x, 0, int(x), rc.bottom, grid_width, gridColor);
        }
        for (double y = rc.top + dy; y < rc.bottom; y += dy) {
            draw_line(0, (int)y, rc.right, int(y), grid_width, gridColor);
        }
    }

    void draw_ellips(int left_x, int up_y, int dx, int dy) {
        HPEN hPen = CreatePen(PS_SOLID, ellips_width, circleColor.toRGB());
        HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hPen));

        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));  // Чтобы рисовать без заливки
        Ellipse(hdc, left_x, up_y, left_x + dx, up_y + dy);

        SelectObject(hdc, hOldPen);

        DeleteObject(hPen);
        DeleteObject(hOldPen);
    }

    void draw_cross(int left_x, int up_y, int dx, int dy) {
        draw_line(left_x, up_y, left_x + dx, up_y + dy, cross_width, crossColor);
        draw_line(left_x + dx, up_y, left_x, up_y + dy, cross_width, crossColor);
    }

    void draw_line(int x, int y, int x2, int y2, int width, Color color) { /// Не работает изменение цвета линии
        HPEN hPen = CreatePen(PS_SOLID, width, color.toRGB());
        HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hPen));
        
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x2, y2);

        SelectObject(hdc, hOldPen);
        DeleteObject(hOldPen);
        DeleteObject(hPen);

    }

private:
    void set_default(HWND _hWnd) {
        hWnd = _hWnd;
        GetClientRect(hWnd, &rc);
        hdc=BeginPaint(hWnd, &ps);
        SelectObject(hdc, hBrush);
        SetBkMode(hdc, TRANSPARENT);
    }
};