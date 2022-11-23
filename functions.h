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

void get_grid_rect(HWND hWnd, int grid_n, int i, int j, int& left_x, int& up_y, int& dx, int& dy, int padding) {
    double _dx = (double)get_width(hWnd) / (double)grid_n;
    double _dy = (double)get_height(hWnd) / (double)grid_n;

    left_x = (int)(j * _dx) + padding;
    up_y = (int)(i * _dy) + padding;
    dx = (int)_dx - 2 * padding;
    dy = (int)_dy - 2 * padding;
}

void get_ij(HWND hWnd, int grid_n, int x, int y, int& i, int& j) {
    int height = get_height(hWnd);
    int width = get_width(hWnd);
    int dx = width / grid_n; // Возможно лучше double
    int dy = height / grid_n;
    i = y / dy;
    j = x / dx;
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