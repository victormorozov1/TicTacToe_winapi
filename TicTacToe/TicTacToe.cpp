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

#define KEY_SHIFTED     0x8000
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

struct color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

int n = 5;

LPTSTR buffer;
UINT synchMessage; 

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return check_events(hwnd, message, wParam, lParam, game, synchMessage);
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

int main(int argc, char** argv)
{
	

	if (argc > 1) {
		n = atoi(argv[1]);
	}

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

	startMessageCycle();

	UnmapViewOfFile(buffer);
	CloseHandle(hFileMapping);
	DestroyWindow(hwnd);
	UnregisterClass(szWinClass, hThisInstance);
	DeleteObject(hBrush);

	return 0;
}