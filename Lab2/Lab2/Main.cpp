#include <Windows.h>
#include "WindowLogic.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSW wc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"SECONDLABORATORYWORKCLASS";

	RegisterClassW(&wc);

	HWND hwnd = CreateWindowEx(
		0,
		wc.lpszClassName,
		L"Лабораторная работа №2 Кулаков Дмитрий 6303",
		WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX,

		CW_USEDEFAULT, CW_USEDEFAULT, 450, 300,

		NULL,    
		NULL,
		hInstance,
		NULL
	);

	HWND hEdit = CreateWindowEx(
		0,
		L"EDIT",
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 10, 300, 20,
		hwnd,
		(HMENU)10,
		hInstance,
		NULL
	);

	HWND hButton = CreateWindow(
		L"BUTTON",
		L"Запуск",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		320, 10, 100, 20,
		hwnd,
		(HMENU)1,
		hInstance,
		NULL
	);

	HWND hListBox = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"LISTBOX",
		L"",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY,
		10, 50, 410, 190,
		hwnd,
		(HMENU)101,
		hInstance,
		NULL
	);

	HWND hStatic = CreateWindowEx(
		0,
		L"STATIC",
		L"",
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		0, 241, 448, 18,
		hwnd,
		(HMENU)2,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}