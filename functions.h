#include <windows.h>

int get_height(HWND hWnd) {
    RECT rc;
    GetClientRect(hWnd, &rc);
    return rc.bottom - rc.top;
}

int get_width(HWND hWnd) {
    RECT rc;
    GetClientRect(hWnd, &rc);
    return rc.right - rc.left;
}