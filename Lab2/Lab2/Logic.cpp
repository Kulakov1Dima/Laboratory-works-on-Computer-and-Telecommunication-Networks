#include <Windows.h>
#include <string>

#include "Logic.h"


using namespace std;

string url;

Logic::Logic(HWND hwnd) {
    wchar_t lurl[256];
    GetWindowText(GetDlgItem(hwnd, 10), lurl, sizeof(lurl));
    wstring wide_url(lurl);
    url = string(wide_url.begin(), wide_url.end());
}

bool Logic::checkUrl(HWND hwnd) {
    if (url.length()<9) {
        SetWindowText(GetDlgItem(hwnd, 2), L"Неверная длина адресной строки");
        return false;
    }
    else {
        if (url.substr(0, 7) == "http://" || url.substr(0, 8) == "https://") {
            return true;
        }
        else {
            SetWindowText(GetDlgItem(hwnd, 2), L"Проверьте правильность протокола");
        }
    }
    return false;
}

wchar_t* Logic::getUrl()
{
    int wstrSize = MultiByteToWideChar(CP_UTF8, 0, url.c_str(), -1, nullptr, 0);
    wchar_t* wideUrl = new wchar_t[wstrSize];
    MultiByteToWideChar(CP_UTF8, 0, url.c_str(), -1, wideUrl, wstrSize);

    return wideUrl;
}


