//#include <windows.h>

class Painter {
public:
    PAINTSTRUCT ps;
    HBRUSH hBrush;
    HDC hdc;
    HWND hWnd;
    RECT rc;
    Color backgroundColor{255, 0, 0};
    Color gridColor{100, 200, 55};
    Color crossColor{1, 2, 3};
    Color circleColor{200, 300, 0};

    Painter (HWND _hWnd) {
        hWnd = _hWnd;
        GetClientRect(hWnd, &rc);
        hdc=BeginPaint(hWnd, &ps);
        SelectObject(hdc, hBrush);
        SetBkMode(hdc, TRANSPARENT);
    }

    void end_paint() {
        EndPaint(hWnd, &ps);
    }

    void draw_line(int x, int y, int x2, int y2, Color color) { /// Не работает изменение цвета линии
//        HBRUSH hBrush; //создаём объект-кисть
        CreateSolidBrush(RGB(255,0,67)); //задаём сплошную кисть, закрашенную цветом RGB
        SelectObject(hdc, hBrush); //делаем кисть активной
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x2, y2);
    }

    void set_background() {
        SetDCBrushColor(hdc, backgroundColor.toRGB());
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
    }

    void draw_grid(int cells_num) {
        double dx = (double)(rc.right - rc.left) / (double)cells_num;
        double dy = (double)(rc.bottom - rc.top) / (double)cells_num;

        for (double x = dx; x < rc.right; x += dx) {
            draw_line((int)x, 0, int(x), rc.bottom, gridColor);
        }
        for (double y = dy; y < rc.bottom; y += dy) {
            draw_line(0, (int)y, rc.right, int(y), gridColor);
        }
    }

    void draw_ellips(int left_x, int up_y, int dx, int dy) {
        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));  // Чтобы рисовать без заливки
        Ellipse(hdc, left_x, up_y, left_x + dx, up_y + dy);
    }

    void draw_cross(int left_x, int up_y, int dx, int dy) {
        draw_line(left_x, up_y, left_x + dx, up_y + dy, crossColor);
        draw_line(left_x + dx, up_y, left_x, up_y + dy, crossColor);
    }
};