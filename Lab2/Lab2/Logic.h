#pragma once

class Logic {
public:
	Logic(HWND hwnd);
	bool checkUrl(HWND hwnd);
	wchar_t* getUrl();
};