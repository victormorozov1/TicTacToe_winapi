#pragma once

#include <functions.h>

class Painter {
public:
    PAINTSTRUCT ps;
    HBRUSH hBrush;
    HDC hdc;
    HWND hWnd;
    RECT rc;
    Color backgroundColor{255, 0, 0};
    Color gridColor{100, 200, 55};
    Color crossColor{1, 2, 3};
    Color circleColor{200, 300, 0};

    Painter (HWND _hWnd) {
        hWnd = _hWnd;
        GetClientRect(hWnd, &rc);
        hdc=BeginPaint(hWnd, &ps);
        SelectObject(hdc, hBrush);
        SetBkMode(hdc, TRANSPARENT);
    }

    void set_background() {
        return;
        SetDCBrushColor(hdc, backgroundColor.toRGB());
        GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
    }

    void draw_grid(int cells_num) {
        double dx = (double)(get_width(hWnd)) / (double)cells_num;
        double dy = (double)(get_height(hWnd)) / (double)cells_num;
        
        GetClientRect(hWnd, &rc);

        for (double x = rc.left + dx; x < rc.right; x += dx) {
            draw_line((int)x, 0, int(x), rc.bottom, gridColor);
        }
        for (double y = rc.top + dy; y < rc.bottom; y += dy) {
            draw_line(0, (int)y, rc.right, int(y), gridColor);
        }
    }

    void draw_ellips(int left_x, int up_y, int dx, int dy) {
        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));  // Чтобы рисовать без заливки
        Ellipse(hdc, left_x, up_y, left_x + dx, up_y + dy);
    }

    void draw_cross(int left_x, int up_y, int dx, int dy) {
        draw_line(left_x, up_y, left_x + dx, up_y + dy, crossColor);
        draw_line(left_x + dx, up_y, left_x, up_y + dy, crossColor);
    }

private:
    void draw_line(int x, int y, int x2, int y2, Color color) { /// Не работает изменение цвета линии
        HPEN hPen = CreatePen(PS_SOLID, 1, color.toRGB());
        HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hPen));

        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x2, y2);

        SelectObject(hdc, hOldPen);
    }

    void end_paint() {
        EndPaint(hWnd, &ps);
    }
};