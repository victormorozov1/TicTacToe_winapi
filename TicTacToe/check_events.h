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
}

LRESULT CALLBACK check_events(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Game* game, HANDLE &drawing_thread, bool &pause, UINT synchMessage) {
    if (msg ==  WM_SIZE) {
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    else if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    else if (msg == WM_PAINT) {
        game->count_sz();
        return 0;
    }
    else if (msg == WM_MOUSEMOVE) {
        return 0;
    }
    else if (msg == WM_LBUTTONUP) {
        process_click(hWnd, lParam, game, '0');
        InvalidateRect(hWnd, NULL, TRUE);
        PostMessage(HWND_BROADCAST, synchMessage, NULL, NULL);
        return 0;
    }
    else if (msg == WM_RBUTTONUP) {
        process_click(hWnd, lParam, game, 'x');
        InvalidateRect(hWnd, NULL, TRUE);
        PostMessage(HWND_BROADCAST, synchMessage, NULL, NULL);
        return 0;
    }
    else if (msg == WM_MOUSEWHEEL)
    {
        if ((int16_t)HIWORD(wParam) > 0) {
            game->painter.gridColor += 1;
        }
        else {
            game->painter.gridColor -= 1;
        }
        PostMessage(HWND_BROADCAST, synchMessage, NULL, NULL);
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    else if (msg == WM_KEYDOWN) {
        if (((GetKeyState(VK_CONTROL) & KEY_SHIFTED) && (wParam == 81))) {
            DestroyWindow(hWnd);
        }
        else if ((GetKeyState(VK_SHIFT) & KEY_SHIFTED) && (wParam == 67)) {
            notepad();
        }

        if (wParam == VK_RETURN) {
            game->painter.backgroundColor.set_random();
            PostMessage(HWND_BROADCAST, synchMessage, NULL, NULL);
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }
        else if (wParam == VK_SPACE) {
            if (pause) {
                pause = false;
                ResumeThread(drawing_thread);
                std::cerr << "Continue\n";
            }
            else {
                pause = true;
                SuspendThread(drawing_thread);
                std::cerr << "pause\n";
            }
        }
        else if (wParam == VK_ESCAPE) {
               DestroyWindow(hWnd);
        }
        else if (48 <= wParam && wParam <= 57) {
            int priority;
            if (wParam == 48) {
                priority =  THREAD_PRIORITY_IDLE;
            }
            else if (wParam == 57) {
                priority = THREAD_PRIORITY_TIME_CRITICAL;
            }
            else {
                priority =  wParam - 51; 
            }
            SetThreadPriority(drawing_thread, priority);
            std::cerr << "Set thread priority " << priority << std::endl;
        }

        return 0;
    }
    else if (msg == synchMessage) {
        InvalidateRect(hWnd, NULL, TRUE);
    }
    else {
        //std::cerr << "unhandled message " << msg << std::endl;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}