#include <windows.h>

#include "game.h"
#include "config.h"
#include "windowsx.h"

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

LRESULT CALLBACK check_events(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Game* game, UINT synchMessage) {
    switch(msg) {
        case WM_SIZE: {
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        case WM_PAINT: {
            game->draw();
            game->count_sz();
            //game->painter.draw_line(0, 0, 200, 200, 4, Color{100, 100, 100});
            
            return 0;
        }
        case WM_MOUSEMOVE: {
            return 0;
        }
        case WM_LBUTTONUP: {
            process_click(hWnd, lParam, game, '0');
            InvalidateRect(hWnd, NULL, TRUE);
            PostMessage(HWND_BROADCAST, synchMessage, NULL, NULL);
            return 0;
        }
        case WM_RBUTTONUP: {
            process_click(hWnd, lParam, game, 'x');
            InvalidateRect(hWnd, NULL, TRUE);
            PostMessage(HWND_BROADCAST, synchMessage, NULL, NULL);
            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            if ((int16_t)HIWORD(wParam) > 0) {
                game->painter.gridColor += 1;
            }
            else {
                game->painter.gridColor -= 1;
            }
            game->draw();
            PostMessage(HWND_BROADCAST, synchMessage, NULL, NULL);
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }
        case WM_KEYDOWN: {
            if (((GetKeyState(VK_CONTROL) & KEY_SHIFTED) && (wParam == 81))) {
                DestroyWindow(hWnd);
            }
            else if ((GetKeyState(VK_SHIFT) & KEY_SHIFTED) && (wParam == 67)) {
                notepad();
            }

            switch (wParam) {
                case VK_RETURN: {
                    game->painter.backgroundColor.set_random();
                    game->draw();
                    PostMessage(HWND_BROADCAST, synchMessage, NULL, NULL);
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                case VK_ESCAPE: {
                    DestroyWindow(hWnd);
                }
            }

            return 0;
        }
        default: {
            if (msg == synchMessage) {
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;

        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}