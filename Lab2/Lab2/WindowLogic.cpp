#include <wchar.h>
#include <Windows.h>
#include "WindowLogic.h"


wchar_t g_szTextBuffer[256];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND: {
		if (LOWORD(wParam) == 1) {
			wchar_t messageBuffer[256];
			wchar_t buffer[16];
			static int count = 0;
			swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"Элемент %d", ++count);
			SendMessage(GetDlgItem(hwnd, 101), LB_ADDSTRING, 0, (LPARAM)buffer);

			GetWindowText(GetDlgItem(hwnd, 10), g_szTextBuffer, sizeof(g_szTextBuffer));
			swprintf(messageBuffer, sizeof(messageBuffer) / sizeof(messageBuffer[0]), L"Добавлен элемент: %s", g_szTextBuffer);
			SetWindowText(GetDlgItem(hwnd, 2), messageBuffer);
		}
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}