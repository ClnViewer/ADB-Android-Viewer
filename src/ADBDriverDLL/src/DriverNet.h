
#pragma once


#if (!defined(__WIN32__) && !defined(__WIN64__))
   typedef SOCKET int32_t;
#  define INVALID_SOCKET -1
#endif

namespace GameDev
{

class DriverNet
{
private:
    std::atomic<bool> IsError;

public:

    typedef struct __attribute__((__packed__))
    {
        uint32_t id;
        uint32_t usz;
        uint32_t csz;
        uint32_t w;
        uint32_t h;
    } STREAMHEADER;

    typedef struct __attribute__((__packed__))
    {
        uint8_t id[4];
        uint32_t sz;
    } ADBSYNCDATA;

    DriverNet();
    ~DriverNet();

    SOCKET      Connect();
    int32_t     Close(SOCKET);

    bool        Check();
    void        Init(struct sockaddr_in*);
    bool        GetInitError();
    static bool GetNetError();
};

}
