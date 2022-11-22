#include <windows.h>
#include <iostream>

#include "painter.h"
#include "functions.h"

#pragma once

void get_grid_rect(HWND hWnd, int grid_n, int i, int j, int& left_x, int& up_y, int& dx, int& dy) {
    double _dx = (double)get_width(hWnd) / (double)grid_n;
    double _dy = (double)get_height(hWnd) / (double)grid_n;

    left_x = (int)(j * _dx);
    up_y = (int)(i * _dy);
    dx = (int)_dx;
    dy = (int)_dy;
}

void draw_cross(HWND hWnd, Painter painter, int grid_n, int i, int j) {
    int left_x, up_y, dx, dy;
    get_grid_rect(hWnd, grid_n, i, j, left_x, up_y, dx, dy);
    std::cout << left_x << " " << up_y << " " << dx << " " << dy << std::endl;
    painter.draw_cross(left_x, up_y, dx, dy);
}

void draw_ellips(HWND hWnd, Painter painter, int grid_n, int i, int j) {
    int left_x, up_y, dx, dy;
    get_grid_rect(hWnd, grid_n, i, j, left_x, up_y, dx, dy);
    painter.draw_ellips(left_x, up_y, dx, dy);
}