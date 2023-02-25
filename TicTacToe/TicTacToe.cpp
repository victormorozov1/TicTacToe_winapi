﻿#include <stdio.h>
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
	MSG message;            /* Here message to the application are saved */
	WNDCLASS wincl = { 0 };         /* Data structure for the windowclass */

	/* Harcode show command num when use non-winapi entrypoint */
	int nCmdShow = SW_SHOW;
	/* Get handle */
	HINSTANCE hThisInstance = GetModuleHandle(NULL);

	/* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szWinClass;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by Windows */

	/* Use custom brush to paint the background of the window */

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClass(&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	hwnd = CreateWindow(
		szWinClass,          /* Classname */
		szWinName,       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		width,                 /* The programs width */
		height,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);

	HANDLE hFileMapping = OpenFileMapping(PAGE_READWRITE, FALSE, szSharedMemName);
	hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, cells_num * cells_num * sizeof(TCHAR), szSharedMemName);
	buffer = (LPTSTR)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, (cells_num + 10) * cells_num * sizeof(TCHAR));
	std::cout << buffer[0];
	game = new Game(cells_num, hwnd, Painter(hwnd, background_color), buffer); // maybe link

	synchMessage = RegisterWindowMessage((LPCTSTR)_T("sdfn"));

	/* Make the window visible on the screen */
	ShowWindow(hwnd, nCmdShow);

	/* Run the message loop. It will run until GetMessage() returns 0 */
	//while ((bMessageOk = GetMessage(&message, NULL, 0, 0)) != 0)
	//{
	//	/* Yep, fuck logic: BOOL mb not only 1 or 0.
	//	 * See msdn at https://msdn.microsoft.com/en-us/library/windows/desktop/ms644936(v=vs.85).aspx
	//	 */
	//	if (bMessageOk == -1)
	//	{
	//		puts("Suddenly, GetMessage failed! You can call GetLastError() to see what happend");
	//		break;
	//	}
	//	/* Translate virtual-key message into character message */
	//	TranslateMessage(&message);
	//	/* Send message to WindowProcedure */
	//	DispatchMessage(&message);
	//}

	startMessageCycle();

	/* Cleanup stuff */

	UnmapViewOfFile(buffer);
	CloseHandle(hFileMapping);
	DestroyWindow(hwnd);
	UnregisterClass(szWinClass, hThisInstance);
	DeleteObject(hBrush);


	return 0;
}