#include <wchar.h>
#include <Windows.h>

#include "eventHandler.h"
#include "Logic.h"
#include "Net.h"

//http://imc.ssau.ru

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND: {
		if (LOWORD(wParam) == 1) {
			Logic backWindow(hwnd);
			if (backWindow.checkUrl(hwnd)) {
				wchar_t*  url = backWindow.getUrl();

				wchar_t messageBuffer[512];
				swprintf(messageBuffer, sizeof(messageBuffer) / sizeof(messageBuffer[0]), L"Проверка завершена: %s", url);
				SetWindowText(GetDlgItem(hwnd, 2), messageBuffer);

				Network getter(url);
				if (getter.createSocket(hwnd)) {
					if (getter.connectServer(hwnd)) {

					}
				}

			}
			//wchar_t messageBuffer[256];
			//wchar_t buffer[16];
			//static int count = 0;
			//swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"Элемент %d", ++count);
			//SendMessage(GetDlgItem(hwnd, 101), LB_ADDSTRING, 0, (LPARAM)buffer);
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