// точка входа в программу
#include <windows.h>//добавляем к проекту заголовочный файл с основными функциями и макросами Windows API
#include <tchar.h>
#include <windowsx.h>
#include <cstdint>
#include <fstream>

#include "color.h"

// Тип TCHAR и макрос TEXT в зависимости от типа компиляции выбирают использовать юникод или обычный ANSI

const int winWidth = 320;
const int winHeight = 240;
const int N = 3;

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

Color backgroundColor{255, 0, 0};
Color gridColor{100, 200, 55};
LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

WNDCLASSEX classRegister(HINSTANCE hInst) {
    WNDCLASSEX wcx = {0};//обнуляем сразу все поля структуры, чтобы ничего не забыть, т.к. понадобятся нам пока не все// я же говорил что WNDCLASSEX можно не юзать, но MSDN ругается
    wcx.cbSize = sizeof(WNDCLASSEX); //по размеру структуры Windows определит, какая версия API была использована
    wcx.style = CS_HREDRAW | CS_VREDRAW;// говорим окну перерисовываться при изменении размеров окна
    wcx.lpfnWndProc = WndProc;// указываем функцию обработки сообщений
    wcx.hInstance = hInst;	// хендл на наше приложение
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // GetStockObject возвращает хендл на белую кисточку, для фона окна
    wcx.lpszClassName = TEXT("[Rossky.ru] Win32.");// имя данного класса. Должно быть уникальным, иначе, если класс с таким именем уже зарегестрирован, то в регистрации будет отказано
    return wcx;
}

HWND winCreate(HINSTANCE hInst) {
    HWND hWnd = CreateWindowEx(
            0,
            TEXT("[Rossky.ru] Win32."),//имя класса
            TEXT("[Rossky.ru] Win32. Первое приложение Win32."),//заголовок окна
            WS_OVERLAPPEDWINDOW, //тип окошка (включает отображение системного меню, кнопок в верхнем правом углу и т.п.)
            CW_USEDEFAULT,0,//место появления окна (координаты х и y). Здесь указано место “по умолчанию”, поэтому второй параметр игнорируется
            winWidth,winHeight,//ширина окна (определяется аналогично месту появления)
            0, //ссылка на родительское окно
            0,//хендл меню
            hInst,
            0
    );//код, передаваемый с сообщением WM_CREATE
    return hWnd;
}

MSG startMessageCycle() {
    MSG msg = {0};// создаем структуру сообщения, которую будем обрабатывать
    while( GetMessage(&msg,
                      0,//говорим получать сообщения от всех окон
                      0, 0) )//диапазон значений получаемых сообщений (сейчас получаем все)
    {	// ждем сообщение
        TranslateMessage(&msg);	// преобразуем виртуальную клавишу в ASCII-код и посылаем сообщение WM_CHAR (тут не нужно.Необходимо, если надо работать с текстом, вводимым с клавиатуры)
        DispatchMessage(&msg);	// передаем сообщения для обработки в "главную функцию обработки сообщений"
    }

    return msg;
}

//void setBackground(HWND hwnd, Color color) {
//    PAINTSTRUCT ps;
//    RECT rc;
//    HDC hdc = BeginPaint(hwnd, &ps);
//    GetClientRect(hwnd, &rc);
//    SetDCBrushColor(hdc, color.toRGB());
//    FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
//    //or use ps.rcPaint to repaint only the section which requires update
//    //FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(DC_BRUSH));
////    EndPaint(hwnd, &ps);
//}

int WINAPI WinMain(HINSTANCE hInst,	//хендл на это приложение
                   HINSTANCE hPrev,				//оставлен для совместимости с Win16, всегда = 0
                   LPSTR szCmdLine,				//командная строка этого приложения
                   int nShowCmd)					//параметры, указывающие, как надо запускать приложение
{
    // 1й этап
    // регистрируется класс

    auto wcx = classRegister(hInst);  /// Может лучше указатель

    if ( !RegisterClassEx(&wcx) )
        return 1;// регистрируем ( не получилось - уходим по английски ) с кодом ошибки (1)

    // 2й этап
    // создается окно

    auto hWnd = winCreate(hInst);

    if (!hWnd)    //проверяем успешность создания окна
        return 2;

    // теперь показываем окошко ( nShowCmd - как его показать? минимизированным, обычным или ... )
    ShowWindow(hWnd, nShowCmd);
    auto painter = Painter(hWnd);
//    setBackground(hWnd, backgroundColor);
    painter.draw_line(hWnd, 20, 10, 150, 150);
    painter.draw_line(hWnd, 30, 60, 100, 150);
//    UpdateWindow(hWnd);
//    E
    // говорим окну обновиться


    // 3й этап
    // запуск главного цикла обработки сообщений

    auto msg = startMessageCycle();

    return( (int)msg.wParam );	// т.к. это функция, то вернем параметр WM_QUIT сообщения (см. PostQuitMessage)
}

// главная функция обработки сообщений
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg) {
        case WM_DESTROY:// если этого не сделать, то все ваши жалкие попытки закрыть окно будут проигнорированы
            PostQuitMessage(0);// отправляет приложению сообщение WM_QUIT. Принимает код ошибки, который заносится в wParam сообщения WM_QUIT
            break;
        case WM_PAINT:
            break;
        case WM_MOUSEMOVE:
            break;
        default:
            return(DefWindowProc(hWnd, msg, wParam, lParam));//освобождаем очередь приложения от нераспознаных функций
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);//обрабатываем все остальные сообщения обработчиком "по умолчанию"
}