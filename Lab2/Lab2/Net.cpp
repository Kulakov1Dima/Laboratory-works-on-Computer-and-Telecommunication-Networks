#include <WinSock2.h>
#include <ws2tcpip.h>
#include <wchar.h>
#include <string>

#include "Net.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

SOCKET sock;
string addr;


Network::Network(wchar_t* takeUrl) {
	wstring wide_url(takeUrl);
	addr = string(wide_url.begin(), wide_url.end());
}

bool Network::createSocket(HWND hwnd)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		SetWindowText(GetDlgItem(hwnd, 2), L"Не удалось инициализировать Winsock");
		return false;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		SetWindowText(GetDlgItem(hwnd, 2), L"Не удалось создать сокет");
		WSACleanup();
		return false;
	}

	return true;
}

bool Network::connectServer(HWND hwnd)
{

	if (addr.substr(0, 7) == "http://") {
		addr = addr.substr(7, addr.length());
	}
	if (addr.substr(0, 8) == "http://") {
		addr = addr.substr(8, addr.length());
	}
		
	return false;
}


