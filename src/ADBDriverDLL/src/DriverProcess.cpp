/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "DriverInternal.h"

namespace GameDev
{

std::wstring ADBDriver::Manager(const std::wstring & cmd)
{
    HANDLE hOutR, hOutW;
    STARTUPINFOW si;
    SECURITY_ATTRIBUTES sa;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&sa, sizeof(sa));

    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (
        (!CreatePipe(&hOutR, &hOutW, &sa, 0U)) ||
        (!SetHandleInformation(hOutR, HANDLE_FLAG_INHERIT, 0))
    )
        return std::wstring();

    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    si.cb = sizeof(si);
    si.hStdInput = NULL;
    si.hStdOutput = hOutW;
    si.hStdError = hOutW;
    si.dwFlags = STARTF_USESTDHANDLES;

    std::wstring adbcmd = m_exepath + L" " + cmd;

    BOOL ret = CreateProcessW(
                   m_exepath.c_str(),
                   const_cast<wchar_t*>(adbcmd.c_str()),
                   NULL,
                   NULL,
                   TRUE,
                   DETACHED_PROCESS | CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW,
                   NULL,
                   NULL,
                   &si,
                   &pi
               );

    CloseHandle(hOutW);
    std::wstring data;

    do
    {
        if (!ret)
            break;

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        DWORD br;
        auto buf = std::make_unique<char[]>(256);

        while (
               (ReadFile(hOutR, buf.get(), 256U, &br, NULL)) && (br > 0U)
              )
        {
            std::string _data(buf.get(), br);
            data.append(_data.begin(), _data.end());
        }
    }
    while (0);

    CloseHandle(hOutR);

    if (!ret)
        return std::wstring();

    return data;
}

}
