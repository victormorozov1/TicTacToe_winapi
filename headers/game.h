#pragma once

#include <windows.h>
#include <algorithm>
#include <string>
#include <vector>

#include "painter.h"
#include "functions.h"
#include "draw.h"

bool is_x(char c) {
    return c == 1 || std::tolower(c) == 'x';
}

bool is_o(char c) {
    return c == 2 || std::tolower(c) == 'o' || c == '0';
}

bool equal_symbols(char c1, char c2) {
    return is_x(c1) && is_x(c2) || is_o(c1) && is_o(c2);
}

bool equal_arr(int* a, int sz) {
    if (a[0] == 0) {
        return false;
    }
    for (int i = 0; i < sz; i++) {
        if (!equal_symbols(a[i], a[0])) {
            return false;
        }
    }
    return true;
}

class Game{
public:
    int cells_num;
    HWND hWnd;
    Painter painter;
    int** field = nullptr; // 0 - empty; 1 - X; 2 - O
    bool game_finished;
    int steps_num, winner;
    int height, width;

    Game(int _cells_num, HWND _hWnd, Painter painter) : painter(painter) {
        cells_num = _cells_num;
        hWnd = _hWnd;

        create_field();

        game_finished = false;
        steps_num = 0;
        winner = 0;

        count_sz();
    }

    void count_sz() {
        height = get_full_height(hWnd);
        width = get_full_width(hWnd);
    }

    std::string game_status() {
        if (game_finished) {
            return "end";
        }
        if (steps_num % 2 == 0) {
            return "X";
        }
        return "O";
    }

    void check_end() {
        check_arr_on_end(field);
        auto r_field = reverse(field, cells_num);
        check_arr_on_end(r_field);
        check_string_on_end(get_main_diag(0, 0, 1, 1));
        check_string_on_end(get_main_diag(0, cells_num - 1, 1, -1));
    }

    void set(int i, int j, char symbol) {
        if (equal_symbols(game_status()[0], symbol)) {
            if (is_empty(i, j)) {
                field[i][j] = symbol;
                steps_num++;
            }
        }
    }

    void draw() {
        painter.set_background();

        painter.draw_grid(cells_num);
        for (int i = 0; i < cells_num; i++) {
            for (int j = 0; j < cells_num; j++) {
                if (is_o(field[i][j])) {
                    draw_ellips(hWnd, painter, cells_num, i, j);
                } else if (is_x(field[i][j])) {
                    draw_cross(hWnd, painter, cells_num, i, j);
                }
            }
        }
    }



private:
    void create_field() {
        field = new int*[cells_num];
        for (int i = 0; i < cells_num; i++) {
            field[i] = new int[cells_num];
            std::fill(field[i], field[i] + cells_num, 0);
        }
    }

    void check_string_on_end(int* a) {
        if (equal_arr(a, cells_num)) {
            winner = a[0];
            game_finished = true;
        }
    }

    void check_arr_on_end(int** a) {
        for (int i = 0; i < cells_num; i++) {
            check_string_on_end(a[i]);
        }
    }

    bool is_empty(int i, int j) {
        return field[i][j] == 0;
    }

    bool one_coord_on_field(int x) {
        return x >= 0 && x < cells_num;
    }

    bool on_field(int i, int j) {
        return one_coord_on_field(i) && one_coord_on_field(j);
    }

    int* get_main_diag(int start_i, int start_j, int di, int dj) {
        auto ret = new int(cells_num);
        int i = start_i, j = start_j, sz = 0;
        while (on_field(i, j)) {
            ret[sz] = field[i][j];
            i += di;
            j += dj;
            sz++;
        }
        return ret;
    }
};