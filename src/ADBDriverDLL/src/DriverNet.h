
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

    enum SyncTagType
    {
        SYNC_TAG_OKAY = 0,
        SYNC_TAG_FAIL,
        SYNC_TAG_DONE,
        SYNC_TAG_SEND,
        SYNC_TAG_RECV,
        SYNC_TAG_DATA,
        SYNC_TAG_LIST,
        SYNC_TAG_DENT,
        SYNC_TAG_NONE
    };

    typedef bool (dir_list_cb)(GameDev::DriverNet::ADBSYNCDENT const&, std::string const&, void*);

    DriverNet();
    ~DriverNet();

    SOCKET      Connect();
    int32_t     Close(SOCKET);

    bool        Check();
    void        Init(struct sockaddr_in*);
    bool        GetInitError();
    static bool GetNetError();

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
    bool                   SyncFileReceive(SOCKET, std::string const&, std::string const&);
    bool                   SyncDirList(SOCKET, std::string const&, void*, std::function<dir_list_cb>);
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
