#include <windows.h>
#include <algorithm>
#include <string>

#include "painter.h"
#include "functions.h"
#include "draw.h"

bool equal_arr(int* a, int sz) {
    if (a[0] == 0) {
        return false;
    }
    for (int i = 0; i < sz; i++) {
        if (a[i] != a[0]) {
            return false;
        }
    }
    return true;
}

bool is_x(char c) {
    return c == 1 || std::tolower(c) == 'x';
}

bool is_o(char c) {
    return c == 2 || std::tolower(c) == 'o' || c == '0';
}

class Game{
public:
    int cells_num;
    HWND hWnd;
    Painter painter;
    int** field = nullptr; // 0 - empty; 1 - X; 2 - O
    bool game_finished;
    int steps_num, winner;

    Game(int _cells_num, HWND _hWnd, Painter painter) : painter(painter) {
        cells_num = _cells_num;
        hWnd = _hWnd;

        create_field();

        game_finished = false;
        steps_num = 0;
        winner = 0;
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
    }

    void set(int i, int j, char symbol) {
        if (equal_symbols(game_status()[0], symbol)) {
            if (is_empty(i, j)) {
                field[i][j] = symbol;
                steps_num++;            }
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

    void check_arr_on_end(int** a) {
        for (int i = 0; i < cells_num; i++) {
            if (equal_arr(a[i], cells_num)) {
                winner = a[i][0];
                game_finished = true;
                return;
            }
        }
    }

    bool is_empty(int i, int j) {
        return field[i][j] == 0;
    }

    bool equal_symbols(char c1, char c2) {
        return is_x(c1) && is_x(c2) || is_o(c1) && is_o(c2);
    }
};