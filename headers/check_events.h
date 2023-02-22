#include <windows.h>

#include "game.h"
#include "config.h"

#define KEY_SHIFTED 0x8000

void process_click(HWND hWnd, LPARAM lParam, Game* game, char symbol) {
    game->check_end();
    int x = GET_X_LPARAM(lParam);
    int y = GET_Y_LPARAM(lParam);
    int i, j;
    get_ij(hWnd, game->cells_num, x, y, i, j);
    game->set(i, j, symbol);
    game->draw();
}

void check_events(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Game* game) {
    switch(msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            game->draw();
            game->count_sz();
            break;
        case WM_MOUSEMOVE:
            break;
        case WM_LBUTTONUP:
            process_click(hWnd, lParam, game, '0');
            break;
        case WM_RBUTTONUP:
            process_click(hWnd, lParam, game, 'x');
            break;
        case WM_MOUSEWHEEL:
        {
            if ((int16_t)HIWORD(wParam) > 0) {
                game->painter.gridColor += 1;
            }
            else {
                game->painter.gridColor -= 1;
            }
            game->draw();
            return ;
        }
        case WM_KEYDOWN:
        {
            if (((GetKeyState(VK_CONTROL) & KEY_SHIFTED) && (wParam == 81))) {
                DestroyWindow(hWnd);
            }
            else if ((GetKeyState(VK_SHIFT) & KEY_SHIFTED) && (wParam == 67)) {
                notepad();
            }

            switch (wParam) {
                case VK_RETURN:
                {
                    game->painter.backgroundColor.set_random();
                    game->draw();

                    return;
                }
                case VK_ESCAPE:
                {
                    DestroyWindow(hWnd);
                }
            }
        }
        default {
            if (message == synchMessage) {
                game->draw();
            }
            break;
        }
    }
}