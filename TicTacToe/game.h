#pragma once

#include <windows.h>
#include <algorithm>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <chrono>
#include <ctime>
#include "painter.h"
#include "functions.h"
#include "draw.h"


namespace sc = std::chrono;

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
    bool game_finished;
    int steps_num, winner;
    int height, width;
    LPTSTR buffer;
    

    Game(int _cells_num, HWND _hWnd, Painter painter, LPTSTR _buffer) : painter(painter) {
        cells_num = _cells_num;
        hWnd = _hWnd;

        game_finished = false;
        steps_num = 0;
        winner = 0;

        buffer = _buffer;
        
       // std::chrono::system_clock::time_point start_time = 
        
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

        int steps = 0;
        for (int i = 0; i < cells_num; i++) {
            for (int j = 0; j < cells_num; j++) {
                if (!is_empty(i, j)) {
                    steps++;
                }
            }
        }

        if (steps % 2 == 0) {
            return "X";
        }
        return "O";
    }

    void check_end() {
        if (game_finished) {
            return;
        }

        for (int i = 0; i < cells_num; i++) {
            int sum = 0;
            for (int j = 0; j < cells_num; j++) {
                if (is_x(get(i, j))) {
                    sum++;
                }
                else if (is_o(get(i, j) )) {
                    sum--;
                }
            }
            if (abs(sum) == cells_num) {
                game_finished = true;
                return;
            }
        }

        for (int j = 0; j < cells_num; j++) {
            int sum = 0;
            for (int i = 0; i < cells_num; i++) {
                if (is_x(get(i, j))) {
                    sum++;
                }
                else if (is_o(get(i, j))) {
                    sum--;
                }
            }
            if (abs(sum) == cells_num) {
                game_finished = true;
                return;
            }
        }

        int sum1 = 0, sum2 = 0;
        for (int i = 0; i < cells_num; i++) {
            if (is_x(get(i, i))) {
                sum1++;
            }
            else if (is_o(get(i, i))) {
                sum1--;
            }
            if (is_x(get(i, cells_num - i - 1))) {
                sum2++;
            }
            else if (is_o(get(i, cells_num - i - 1))) {
                sum2--;
            }
        }

        game_finished = abs(sum1) == cells_num || abs(sum2) == cells_num;
    }

    void set(int i, int j, char symbol) {
        if (equal_symbols(game_status()[0], symbol)) {
            if (is_empty(i, j)) {
                buffer[i * cells_num + j] = symbol;
                steps_num++;
            }
        }
    }

    int get(int i, int j) {
        return buffer[i * cells_num + j];
    }

    void draw(double passed_time) {

        painter.set_background();

        int min_width = 5;
        int max_width = 15;
        int width_interval = max_width - min_width;
        passed_time *= 10;
       // std::cerr << "passed time = " << passed_time << std::endl;
        double half_animation_interval = 5;
        double animation_interval = half_animation_interval * 2;
        double current_condition = passed_time;
        while (current_condition > animation_interval) {
            current_condition -= animation_interval;
        }
        //std::cout << "current condition = " << current_condition << std::endl;
        int width;
        if (current_condition < half_animation_interval) {
            double p = current_condition / half_animation_interval;
            width = min_width + p * width_interval;
        }
        else {
            current_condition -= half_animation_interval;
            double p = current_condition / half_animation_interval;
            width = max_width - p * width_interval;
        }

        std::cout << "width = " << width << std::endl;

        painter.draw_grid(cells_num);
        for (int i = 0; i < cells_num; i++) {
            for (int j = 0; j < cells_num; j++) {
                if (is_o(get(i, j))) {
                    draw_ellips(hWnd, painter, cells_num, i, j, width);
                } else if (is_x(get(i, j))) {
                    draw_cross(hWnd, painter, cells_num, i, j, width);
                }
            }
        }
    }

private:
    void check_string_on_end(int* a) {
        if (equal_arr(a, cells_num)) {
            winner = a[0];
            game_finished = true;
        }
    }

    void check_arr_on_end() {
        /*for (int i = 0; i < cells_num; i++) {
            check_string_on_end(a[i]);
        }*/
        // Переделать
    }

    bool is_empty(int i, int j) {
        return get(i, j) == 0;
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
            ret[sz] = get(i, j);
            i += di;
            j += dj;
            sz++;
        }
        return ret;
    }
};