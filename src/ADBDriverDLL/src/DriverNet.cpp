
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
        //memset(&addr, 0, sizeof(addr));
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
