#include <wchar.h>
#include <Windows.h>
#include <time.h> 

#include "eventHandler.h"
#include "Logic.h"
#include "Net.h"
#include <thread>


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

				thread([hwnd, url]() {
					clock_t start = clock();
					Network getter(url);
					if (getter.createAgent(hwnd)) {
						getter.connectServer(hwnd, (wchar_t*)"/");
						clock_t end = clock();
						double seconds = (double)(end - start) / CLOCKS_PER_SEC;
						wstring infotime = to_wstring(seconds) + L" c";
						LPCWSTR infoT = infotime.c_str();
						SetWindowText(GetDlgItem(hwnd, 7), infoT);
					}

					getter.closeConnection(hwnd);
				}).detach();
			}
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