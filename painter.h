#include <windows.h>

class Painter {
public:
    PAINTSTRUCT ps;
    HBRUSH hBrush;
    HDC hdc;
    HWND hWnd;
    Color backgroundColor{255, 0, 0};
    Color gridColor{100, 200, 55};

    Painter (HWND _hWnd) {
        hWnd = _hWnd;
        hdc=BeginPaint(hWnd, &ps);
        SelectObject(hdc, hBrush);
        SetBkMode(hdc, TRANSPARENT);
    }

    void end_paint() {
        EndPaint(hWnd, &ps);
    }

    void draw_line(int x, int y, int x2, int y2) {
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x2, y2);
    }

    void set_background() {
        RECT rc;
        GetClientRect(hWnd, &rc);
        SetDCBrushColor(hdc, backgroundColor.toRGB());
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
    }
};