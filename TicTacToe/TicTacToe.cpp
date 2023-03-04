#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <cstdint>
#include <fstream>

#include "game.h"
#include "color.h"
#include "painter.h"
#include "check_events.h"
#include "config.h"

#define KEY_SHIFTED     0x8000 //
#define KEY_TOGGLED     0x0001

const TCHAR szWinClass[] = _T("Win32SampleApp");
const TCHAR szWinName[] = _T("Win32SampleWindow");
const TCHAR szSharedMemName[] = _T("MEM NAME");

HWND hwnd;
HBRUSH hBrush;
POINT cursor;

Game* game = nullptr;
int cells_num = 4, width = 320, height = 240;
Color background_color{ 255, 255, 255 };

HANDLE drawing_thread;

LPTSTR buffer;
UINT synchMessage;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return check_events(hwnd, message, wParam, lParam, game, drawing_thread, synchMessage);
}

MSG startMessageCycle() {
	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg;
}

DWORD WINAPI drawing_func(LPVOID) {
	PAINTSTRUCT ps;
	RECT rect;

	while (true) {
		game->draw();
		GetClientRect(hwnd, &rect);
		RedrawWindow(hwnd, &rect, NULL, RDW_INVALIDATE); 
		Sleep(140); // тут бы аналог clock.tick()
	}
}

int main(int argc, char** argv)
{
	read_config(cells_num, width, height, background_color);

	BOOL bMessageOk;
	MSG message;
	WNDCLASS wincl = { 0 };

	int nCmdShow = SW_SHOW;
	HINSTANCE hThisInstance = GetModuleHandle(NULL);

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szWinClass;
	wincl.lpfnWndProc = WindowProcedure;

	if (!RegisterClass(&wincl))
		return 0;

	hwnd = CreateWindow(
		szWinClass,
		szWinName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		HWND_DESKTOP,
		NULL,
		hThisInstance,
		NULL
	);

	HANDLE hFileMapping = OpenFileMapping(PAGE_READWRITE, FALSE, szSharedMemName);
	hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, cells_num * cells_num * sizeof(TCHAR), szSharedMemName);
	buffer = (LPTSTR)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, (cells_num + 10) * cells_num * sizeof(TCHAR));
	std::cout << buffer[0];
	game = new Game(cells_num, hwnd, Painter(hwnd, background_color), buffer);

	synchMessage = RegisterWindowMessage((LPCTSTR)_T("BRAWL STARS"));

	ShowWindow(hwnd, nCmdShow);

	drawing_thread = CreateThread(NULL, 0, drawing_func, NULL, 0, 0);

	startMessageCycle();

	write_config(game->cells_num, game->width, game->height, game->painter.backgroundColor);

	UnmapViewOfFile(buffer);
	CloseHandle(hFileMapping);
	DestroyWindow(hwnd);
	UnregisterClass(szWinClass, hThisInstance);
	DeleteObject(hBrush);

	return 0;
}