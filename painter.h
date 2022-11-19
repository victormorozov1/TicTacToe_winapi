#include <windows.h>//добавляем к проекту заголовочный файл с основными функциями и макросами Windows API
#include <tchar.h>
#include <windowsx.h>
#include <cstdint>
#include <fstream>

class Painter {
public:
    PAINTSTRUCT ps;
    HBRUSH hBrush;
    HDC hdc;

    Painter (HWND hWnd) {
        hdc=BeginPaint(hWnd, &ps);
        SelectObject(hdc, hBrush);//выбор кисти
        SetBkMode(hdc, TRANSPARENT);//Устанавливает режим, определяющий,должен ли интерфейс GDI удалять существующие цвета фона
    }

    void end_paint(HWND hWnd) {
        EndPaint(hWnd, &ps);
    }

    void draw_line(HWND hWnd, int x, int y, int x2, int y2) {
        //перед рисованием текста, использованием шриховочных кистей и стилей пера при рисовании несплошных линий.
        MoveToEx(hdc, x, y, NULL); //сделать текущими координаты x1, y1
        LineTo(hdc, x2, y2);
    }
};