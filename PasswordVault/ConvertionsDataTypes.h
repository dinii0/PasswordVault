#ifndef CONVERTIONSDATATYPES_H
#define CONVERTIONSDATATYPES_H

#include <Windows.h>
#include "framework.h"
#include <iostream>
#include <string>


PTSTR convert_string_to_ptstr(std::string s) {
    int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<int>(s.length()), nullptr, 0);

    PTSTR buffer = new TCHAR[len + 1];
    int result = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<int>(s.length()), buffer, len);

    buffer[len] = _T('\0');
    return buffer;
}
std::string convert_ptstr_to_string(PTSTR pszText) {
    int len = WideCharToMultiByte(CP_UTF8, 0, pszText, -1, nullptr, 0, nullptr, nullptr);

    std::string buffer(len - 1, '\0');
    int result = WideCharToMultiByte(CP_UTF8, 0, pszText, -1, &buffer[0], len, nullptr, nullptr);

    return buffer;
}

#endif
