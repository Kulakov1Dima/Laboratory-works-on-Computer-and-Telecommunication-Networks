#pragma once
#include <set>
#include <string>

using namespace std;

class Network {
public:
	Network(wchar_t* url);
	bool createAgent(HWND hwnd);
	bool connectServer(HWND hwnd, wchar_t* path);
	void closeConnection(HWND hwnd);
	std::set<std::wstring> extractLinksFromHTML(const char* htmlContent, DWORD contentSize);
	bool isRelativeURL(const std::wstring& url);
	int countSlashes(const wchar_t* str);
};