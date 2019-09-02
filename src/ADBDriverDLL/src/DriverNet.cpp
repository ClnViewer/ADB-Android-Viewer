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
#include "DriverConst.h"
#include "Utils/extTraceFunction.h"
#include <errno.h>

#if !defined(SOCKET_READ_TIMEOUT_SEC)
#  define SOCKET_READ_TIMEOUT_SEC 1000
#endif

#if !defined(SOCKET_READ_ATTEMPT)
#  define SOCKET_READ_ATTEMPT 5
#endif

namespace GameDev
{

#if defined(OS_WIN)

DriverNet::DriverNet()
    : IsError(false), IsDesposed(false)
{
    WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (ret != 0)
        IsError = true;
#   if (defined(_BUILD_TRACE) && (_BUILD_TRACE == 1))
    trace_start();
#   endif

}

DriverNet::~DriverNet()
{
    if (IsDesposed)
        return;
    IsDesposed = true;
    WSACleanup();
}

bool DriverNet::GetInitError()
{
    return IsError;
}

bool DriverNet::GetNetError()
{
    return (::WSAGetLastError());
}

std::string DriverNet::GetNetError(int32_t line)
{
    LPWSTR pbuf = nullptr;
    int32_t err = ::WSAGetLastError();

    if (!err)
    {
#       if defined(_BUILD_WARNING_0)
        return std::string(DriverConst::ls_errorDN0);
#       else
        return std::string();
#       endif
    }

    ::FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        nullptr,
        err,
        MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        (LPWSTR)&pbuf,
        0,
        nullptr
    );

    std::stringstream ss;
    ss << "Error (" << line << "): ";

    if (!pbuf)
#       if defined(_BUILD_WARNING_0)
        ss << DriverConst::ls_errorDN1;
#       endif
    else
    {
        ss << string_to_utf8(pbuf).c_str();
        ::LocalFree(pbuf);
    }
    return ss.str();
}

bool DriverNet::GetNetBlock()
{
    return (::WSAGetLastError() == WSAEWOULDBLOCK);
}

bool DriverNet::GetNetTimeOut()
{
    return (::WSAGetLastError() == WSAETIMEDOUT);
}

bool DriverNet::SetNetTimeOut(SOCKET client, int32_t sec)
{
    if (!sec)
        sec = SOCKET_READ_ATTEMPT;
    uint32_t stm = (SOCKET_READ_TIMEOUT_SEC * sec);
    return !(::setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char*)&stm, sizeof(stm)));
}

void DriverNet::Init(struct sockaddr_in *addr)
{
    addr->sin_family = AF_INET;
    addr->sin_port = htons(5037);
    addr->sin_addr.s_addr = inet_addr("127.0.0.1");
}

bool DriverNet::Check()
{
    SOCKET client;
    if ((client = Connect()) == INVALID_SOCKET)
        return false;

    ::closesocket(client);
    return true;
}

int32_t DriverNet::Close(SOCKET client)
{
    if (client != INVALID_SOCKET)
    {
        ::shutdown(client, SD_BOTH);
        return ::closesocket(client);
    }
    return -1;
}

SOCKET DriverNet::Connect()
{
    SOCKET client = INVALID_SOCKET;

    do
    {

        if ((client = ::socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
            break;

        struct sockaddr_in addr{};
        Init(&addr);

        if (::connect(client, (struct sockaddr*)&addr, sizeof(addr)) != 0)
            break;

        return client;
    }
    while (0);

    Close(client);
    return INVALID_SOCKET;
}

#else

/// TODO:

DriverNet::DriverNet()
    : IsError(false), IsDesposed(false) {}
DriverNet::~DriverNet() {}

std::string DriverNet::GetNetError(int32_t) { return std::string(); }
bool        DriverNet::GetNetError() { return false; }
bool        DriverNet::GetNetTimeOut() { return false; }
bool        DriverNet::Check() {}
void        DriverNet::Init(struct sockaddr_in*) {}
int32_t     DriverNet::Connect() { return -1; }
int32_t     DriverNet::Close(SOCKET) {  return -1; }

bool        DriverNet::GetNetBlock()
{
    return ((errno == EAGAIN) || (errno == EWOULDBLOCK));
}

#endif

}
