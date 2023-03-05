#pragma once

#include <windows.h>
#include <iostream>

#include "functions.h"

void draw_line(int x, int y, int x2, int y2, int width, unsigned long color, HDC hdc) { /// Не работает изменение цвета линии
    HPEN hPen = CreatePen(PS_SOLID, width, color);
    HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hPen));

    MoveToEx(hdc, x, y, NULL);
    LineTo(hdc, x2, y2);

    SelectObject(hdc, hOldPen);
    DeleteObject(hOldPen);
    DeleteObject(hPen);
}

void set_background_color(unsigned long background_color, HWND& hWnd, HDC& hdc) {
    PAINTSTRUCT ps;
    RECT rc;
    hdc = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rc);
    SetDCBrushColor(hdc, background_color);
    GetClientRect(hWnd, &rc);
    FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
}

void draw_cross(int left_x, int up_y, int dx, int dy, int width, unsigned long crossColor, HDC& hdc) {
    draw_line(left_x, up_y, left_x + dx, up_y + dy, width, crossColor, hdc);
    draw_line(left_x + dx, up_y, left_x, up_y + dy, width, crossColor, hdc);
}

void draw_ellips(int left_x, int up_y, int dx, int dy, int width, unsigned long ellips_color, HDC& hdc) {
    HPEN hPen = CreatePen(PS_SOLID, width, ellips_color);
    HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hPen));

    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));  // Чтобы рисовать без заливки
    Ellipse(hdc, left_x, up_y, left_x + dx, up_y + dy);

    SelectObject(hdc, hOldPen);

    DeleteObject(hPen);
    DeleteObject(hOldPen);
}