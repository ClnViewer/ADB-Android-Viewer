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
    return (WSAGetLastError());
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

    closesocket(client);
    return true;
}

int32_t DriverNet::Close(SOCKET client)
{
    if (client != INVALID_SOCKET)
        return closesocket(client);
    return -1;
}

SOCKET DriverNet::Connect()
{
    SOCKET client = INVALID_SOCKET;

    do
    {

        if ((client = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
            break;

        struct sockaddr_in addr{};
        Init(&addr);

        if (connect(client, (struct sockaddr*)&addr, sizeof(addr)) != 0)
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

bool    DriverNet::Check() {}
bool    DriverNet::GetNetError() { return false; }
void    DriverNet::Init(struct sockaddr_in*) {}
int32_t DriverNet::Connect() { return -1; }
int32_t DriverNet::Close(SOCKET) {  return -1; }

#endif

}
