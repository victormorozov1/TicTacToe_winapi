#include <windows.h>
#include "game.h"

void check_events(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Game* game) {
    switch(msg) {
        case WM_DESTROY:// если этого не сделать, то все ваши жалкие попытки закрыть окно будут проигнорированы
            PostQuitMessage(0);// отправляет приложению сообщение WM_QUIT. Принимает код ошибки, который заносится в wParam сообщения WM_QUIT
            break;
        case WM_PAINT:
            game->draw();
        case WM_MOUSEMOVE:
            break;
    }
}