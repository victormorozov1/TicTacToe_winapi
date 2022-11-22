#include <windows.h>

#pragma once

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

int** reverse(int** a, int sz) {
    int** ret = new int*[sz];

    for (int i = 0; i < sz; i++) {
        ret[i] = new int[sz];
        for (int j = 0; j < sz; j++) {
            ret[i][j] = a[j][i];
        }
    }

    return ret;
}