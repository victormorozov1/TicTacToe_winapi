#include <windows.h>
#include "game.h"

void process_click(HWND hWnd, LPARAM lParam, Game* game, char symbol) {
    int x = GET_X_LPARAM(lParam);
    int y = GET_Y_LPARAM(lParam);
    int i, j;
    get_ij(hWnd, game->cells_num, x, y, i, j);
    game->set(i, j, symbol);
    game->draw();
}

void check_events(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Game* game) {
    int i, j;
    switch(msg) {
        case WM_DESTROY:// если этого не сделать, то все ваши жалкие попытки закрыть окно будут проигнорированы
            PostQuitMessage(0);// отправляет приложению сообщение WM_QUIT. Принимает код ошибки, который заносится в wParam сообщения WM_QUIT
            break;
        case WM_PAINT:
            game->draw();
        case WM_MOUSEMOVE:
            break;
        case WM_LBUTTONUP:
            process_click(hWnd, lParam, game, '0');
            break;
        case WM_RBUTTONUP:
            process_click(hWnd, lParam, game, 'x');
            break;
    }
}