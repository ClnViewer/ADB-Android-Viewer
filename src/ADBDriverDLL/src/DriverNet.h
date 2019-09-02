
#pragma once

#if !defined(OS_WIN)
   typedef SOCKET int32_t;
#  define INVALID_SOCKET -1
#  define SOCKET_ERROR -1

#  if defined(EAGAIN)
#     define SOCKET_BLOCK EAGAIN
#  elif defined(EWOULDBLOCK)
#     define SOCKET_BLOCK EWOULDBLOCK
#  endif

#else
#  define SOCKET_BLOCK WSAEWOULDBLOCK
#endif

#include "Utils/extEnumPrintable.h"

namespace GameDev
{

class DLL_EXPORT DriverNet
{
private:
    std::atomic<bool> IsError;
    bool              IsDesposed;

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
        int32_t                rsz, asz;
        char                  *buff;
    } ADBSYNCPKT;

    typedef struct __attribute__((__packed__))
    {
        uint8_t  id[4];
        uint32_t sz;
    } ADBSYNCDATA;

    typedef struct __attribute__((__packed__))
    {
        uint8_t  id[4];
        uint32_t mode;
        uint32_t bsize;
        uint32_t date;
        uint32_t esize;
    } ADBSYNCDENT;

    ENUM_PRINTABLE(SyncTagType, int32_t,
        SYNC_TAG_OKAY,  // 0
        SYNC_TAG_FAIL,
        SYNC_TAG_DONE,
        SYNC_TAG_SEND,
        SYNC_TAG_RECV,
        SYNC_TAG_DATA,
        SYNC_TAG_LIST,
        SYNC_TAG_DENT,
        SYNC_TAG_NONE,    // 8
        SYNC_TAG_BEGIN,   // 9
        SYNC_TAG_END,     // 10
        SYNC_TAG_WRITE,   // 11
        SYNC_TAG_ERROR,   // 12
        SYNC_TAG_CONTINUE // 13
    );

    typedef bool (dir_list_cb)(GameDev::DriverNet::ADBSYNCDENT const&, std::string const&, void*);

    DriverNet();
    ~DriverNet();

    SOCKET             Connect();
    int32_t            Close(SOCKET);

    bool               Check();
    void               Init(struct sockaddr_in*);
    bool               GetInitError();
    static bool        GetNetError();
    static std::string GetNetError(int32_t);
    static bool        GetNetBlock();
    static bool        GetNetTimeOut();
    static bool        SetNetTimeOut(SOCKET, int32_t = 0);

    /// Template
    template <typename T> DriverNet::SyncTagType SyncDataType(T const&);
    template <typename T> bool SyncCmdSend(SOCKET, T const&, DriverNet::SyncTagType = DriverNet::SyncTagType::SYNC_TAG_OKAY);
    template <typename T> bool SyncStringReceive(SOCKET, T const&, T &);
    template <typename T> bool SyncVectorReceive(SOCKET, T const&, std::vector<uint8_t>&);

    /// Sync protocol
    DriverNet::ADBSYNCDATA SyncDataFromBuff(char*);
    DriverNet::ADBSYNCDATA SyncDataFromBuff(std::string const&);
    DriverNet::ADBSYNCDATA SyncDataFromBuff(std::wstring const&);
    //
    DriverNet::ADBSYNCDATA SyncDataMake(DriverNet::SyncTagType, uint32_t);
    //
    bool                   SyncDataSend(SOCKET, std::string const&, DriverNet::SyncTagType);
    bool                   SyncDataSend(SOCKET, const char*, int32_t, DriverNet::SyncTagType);
    //
    bool                   SyncDataResponse(SOCKET, DriverNet::SyncTagType);
    bool                   SyncDataResponse(SOCKET, DriverNet::ADBSYNCDATA&, DriverNet::SyncTagType);
    //
    bool                   SyncTargetSend(SOCKET, std::wstring const&);
    bool                   SyncFileSend(SOCKET, std::string const&, std::string const&);
    bool                   SyncFileReceive(SOCKET, std::string const&, std::string const&, std::string&);
    bool                   SyncDirList(SOCKET, std::string const&, void*, std::function<dir_list_cb>);

private:
    DriverNet::SyncTagType receiveData(SOCKET, std::unique_ptr<char[]>&, DriverNet::ADBSYNCPKT&);
    DriverNet::SyncTagType parseData(SOCKET, FILE*, DriverNet::ADBSYNCPKT&, bool = false);
    DriverNet::SyncTagType parseDataTag(DriverNet::ADBSYNCPKT&, DriverNet::ADBSYNCDATA const&);
    bool                   receivedDataWrite(FILE*, DriverNet::ADBSYNCPKT&);
    bool                   receiveErrors(DriverNet::ADBSYNCPKT&, std::string&);
    bool                   sendOkay(SOCKET);

#if (defined(_BUILD_TRACE) && (_BUILD_TRACE == 1))
public:
    /// location: DriverNetHelper.cpp
    /// ostream << helper operator to -> class TraceFunction::instance().args(...)
    friend std::ostream & operator << (std::ostream&, const DriverNet::ADBSYNCPKT&);
    friend std::ostream & operator << (std::ostream&, const DriverNet::ADBSYNCDATA&);
#endif

};

extern template DriverNet::SyncTagType DriverNet::SyncDataType<DriverNet::ADBSYNCDATA>(DriverNet::ADBSYNCDATA const&);
extern template DriverNet::SyncTagType DriverNet::SyncDataType<DriverNet::ADBSYNCDENT>(DriverNet::ADBSYNCDENT const&);
//
extern template bool DriverNet::SyncCmdSend<std::wstring>(SOCKET, std::wstring const&, DriverNet::SyncTagType);
extern template bool DriverNet::SyncCmdSend<std::string> (SOCKET, std::string const&,  DriverNet::SyncTagType);
//
extern template bool DriverNet::SyncVectorReceive<std::wstring>(SOCKET, std::wstring const&, std::vector<uint8_t>&);
extern template bool DriverNet::SyncVectorReceive<std::string> (SOCKET, std::string const&,  std::vector<uint8_t>&);
//
extern template bool DriverNet::SyncStringReceive<std::wstring>(SOCKET, std::wstring const&, std::wstring&);
extern template bool DriverNet::SyncStringReceive<std::string> (SOCKET, std::string const&,  std::string&);

}
