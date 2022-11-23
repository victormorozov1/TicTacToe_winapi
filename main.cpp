#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <iostream>

#include "color.h"
#include "check_events.h"
#include "draw.h"
#include "game.h"
#include "config.h"

Game* game = nullptr;
int cells_num = 4, width = 320, height = 240;
Color background_color{117, 193, 255};

LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

WNDCLASSEX classRegister(HINSTANCE hInst) {
    WNDCLASSEX wcx = {0};
    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = WndProc;
    wcx.hInstance = hInst;
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcx.lpszClassName = TEXT("[Rossky.ru] Win32.");
    return wcx;
}

HWND winCreate(HINSTANCE hInst) {
    HWND hWnd = CreateWindowEx(
            0,
            TEXT("[Rossky.ru] Win32."),
            TEXT("[Rossky.ru] Win32. Первое приложение Win32."),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,0,
            width,height,
            0,
            0,
            hInst,
            0
    );
    return hWnd;
}

MSG startMessageCycle() {
    MSG msg = {0};
    while( GetMessage(&msg,
                      0,
                      0, 0) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nShowCmd) {
    read_config(cells_num, width, height, background_color);

    auto wcx = classRegister(hInst);  /// Может лучше указатель
    if ( !RegisterClassEx(&wcx) )
        return 1;// регистрируем ( не получилось - уходим по английски ) с кодом ошибки (1)

    auto hWnd = winCreate(hInst);
    if (!hWnd)
        return 2;

    ShowWindow(hWnd, nShowCmd);

    game = new Game(cells_num, hWnd, Painter(hWnd, background_color));
    game->draw();

    auto msg = startMessageCycle();

    write_config(cells_num, width, height, game->painter.backgroundColor);

    return( (int)msg.wParam );
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    check_events(hWnd, msg, wParam, lParam, game);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
