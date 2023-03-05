#pragma once

#include <windows.h>
#include <iostream>

#include "painter.h"
#include "functions.h"

void draw_cross(HWND hWnd, Painter painter, int grid_n, int i, int j, int width) {
    
    int left_x, up_y, dx, dy;
    get_grid_rect(hWnd, grid_n, i, j, left_x, up_y, dx, dy, painter.padding);

    painter.draw_cross(left_x, up_y, dx, dy, width);

    /*RECT rc = RECT();
    rc.left = left_x;
    rc.right = left_x + dx;
    rc.top = up_y;
    rc.bottom = up_y + dy;
    InvalidateRect(hWnd, &rc, TRUE);*/
}

void draw_ellips(HWND hWnd, Painter painter, int grid_n, int i, int j, int width) {
    
    int left_x, up_y, dx, dy;
    get_grid_rect(hWnd, grid_n, i, j, left_x, up_y, dx, dy, painter.padding);
    painter.draw_ellips(left_x, up_y, dx, dy, width);
}