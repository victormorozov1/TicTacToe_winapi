// точка входа в программу
#include <windows.h>//добавляем к проекту заголовочный файл с основными функциями и макросами Windows API

// Тип TCHAR и макрос TEXT в зависимости от типа компиляции выбирают использовать юникод или обычный ANSI

const int winWidth = 320;
const int winHeight = 240;

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
    // говорим окну обновиться
    UpdateWindow(hWnd);

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
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);//обрабатываем все остальные сообщения обработчиком "по умолчанию"
}