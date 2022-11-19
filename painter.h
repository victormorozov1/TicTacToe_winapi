#include <windows.h>

class Painter {
public:
    PAINTSTRUCT ps;
    HBRUSH hBrush;
    HDC hdc;

    Painter (HWND hWnd) {
        hdc=BeginPaint(hWnd, &ps);
        SelectObject(hdc, hBrush);
        SetBkMode(hdc, TRANSPARENT);
    }

    void end_paint(HWND hWnd) {
        EndPaint(hWnd, &ps);
    }

    void draw_line(HWND hWnd, int x, int y, int x2, int y2) {
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x2, y2);
    }
};