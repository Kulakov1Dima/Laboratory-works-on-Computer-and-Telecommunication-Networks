#include <wchar.h>

#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <regex>

#pragma comment(lib, "wininet.lib")

#include "Net.h"


set<wstring> visitedPaths;

HINTERNET hConnect;
HINTERNET hInternet;
HINTERNET hRequest;
string addr;
int count1 = 0;


Network::Network(wchar_t* takeUrl) {
	wstring wide_url(takeUrl);
	addr = string(wide_url.begin(), wide_url.end());
}

bool Network::createAgent(HWND hwnd)
{

	if (addr.substr(0, 7) == "http://") {
		addr = addr.substr(7, addr.length());
	}
	if (addr.substr(0, 8) == "https://") {
		addr = addr.substr(8, addr.length());
	}

	int wstrSize = MultiByteToWideChar(CP_UTF8, 0, addr.c_str(), -1, nullptr, 0);
	wchar_t* wideUrl = new wchar_t[wstrSize];
	MultiByteToWideChar(CP_UTF8, 0, addr.c_str(), -1, wideUrl, wstrSize);

	hInternet = InternetOpen(L"AGENTLAB2", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet) {
		hConnect = InternetConnect(hInternet, wideUrl, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		return true;
	}
	else {
		SetWindowText(GetDlgItem(hwnd, 2), L"�� ������� ������� ������ WinINet");
		InternetCloseHandle(hInternet);
	}
	return false;
}

bool Network::connectServer(HWND hwnd, wchar_t* path){	
	if (visitedPaths.find(path) != visitedPaths.end()) {
		return true; // ���� ��� �������, ���������� true
	}

	count1++;
	if (count1 > 500) { return false; };

	SendMessage(GetDlgItem(hwnd, 101), LB_ADDSTRING, 0, (LPARAM)path);
	visitedPaths.insert(path);

	if (hConnect) {
		hRequest = HttpOpenRequest(hConnect, L"GET", path, L"HTTP/1.1", NULL, NULL, INTERNET_FLAG_RELOAD, 0);
		if (hRequest) {
			LPCWSTR additionalHeaders = L"User-Agent: Lab2\r\nAccept: */*\r\n";
			if (HttpSendRequest(hRequest, additionalHeaders, lstrlen(additionalHeaders), NULL, 0)) {
				// ������ ������
				char* buffer = nullptr;
				DWORD totalBytesRead = 0;
				DWORD bytesRead;
				DWORD bufferSize = 8192; // ��������� ������ ������

				do {
					buffer = (char*)realloc(buffer, totalBytesRead + bufferSize);
					if (!buffer) {
						SetWindowText(GetDlgItem(hwnd, 2), L"�� ������� �������� ������ ��� ������");
						return false;
					}

					if (InternetReadFile(hRequest, buffer + totalBytesRead, bufferSize, &bytesRead)) {
						totalBytesRead += bytesRead;
					}
					else {
						SetWindowText(GetDlgItem(hwnd, 2), L"�� ������� ��������� ������");
						free(buffer);
						return false;
					}
				} while (bytesRead > 0);

				buffer[totalBytesRead] = '\0';

				SetWindowText(GetDlgItem(hwnd, 2), path);

				std::set<std::wstring> extractedLinks = extractLinksFromHTML(buffer, totalBytesRead);

				for (const auto& link : extractedLinks) {
					// �������� connectServer ���������� ��� ������ ������
					if (!connectServer(hwnd, const_cast<wchar_t*>(link.c_str()))) {
						return false;
					}
				}
				free(buffer); // ������������ ������

				return true;
			}
			else {
				SetWindowText(GetDlgItem(hwnd, 2), L"�� ������� ��������� ������ ��� ��������� ������");
				return false;
			}
		}
		else {
			SetWindowText(GetDlgItem(hwnd, 2), L"�� ������� ������� ������");
			InternetCloseHandle(hRequest);
			return false;
		}
	}
	else{
		SetWindowText(GetDlgItem(hwnd, 2), L"�� ������� ������������ � �������");
		InternetCloseHandle(hConnect);
		return false;
	}
	return false;
}

void Network::closeConnection(HWND hwnd)
{
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
}

set<wstring> Network::extractLinksFromHTML(const char* htmlContent, DWORD contentSize)
{
	set<wstring> links;
	wstring htmlPattern = L"<a\\s+(?:[^>]*?\\s+)?href\\s*=\\s*\"([^\"]*)\"";
	wregex regexPattern(htmlPattern);

	wstring htmlStr(htmlContent, htmlContent + contentSize);
	wsregex_iterator regexIterator(htmlStr.begin(), htmlStr.end(), regexPattern);
	wsregex_iterator endIterator;

	// �������� �� ���� ��������� �����������
	while (regexIterator != endIterator) {
		std::wsmatch match = *regexIterator;
		if (match.size() > 1) {
			wstring url = match.str(1); // �������� URL �� ����������
			if (isRelativeURL(url)) {
				links.insert(url); // ��������� ������ ������������� URL � ��������� ������
			}
		}
		++regexIterator;
	}

	return links;
}

bool Network::isRelativeURL(const std::wstring& url)
{
	return url.find(L"http://") != 0 && url.find(L"https://") != 0;
}

