#pragma once

class Network {
public:
	Network(wchar_t* url);
	bool createSocket(HWND hwnd);
	bool connectServer(HWND hwnd);
};