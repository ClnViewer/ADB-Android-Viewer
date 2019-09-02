
#pragma once

#include "DriverConst.h"
#include "DriverExcept.h"
#include "DriverAsync.h"
#include "DriverNet.h"
#include "Utils/extMultiString.h"

namespace GameDev
{

typedef std::map<std::wstring, std::wstring> SelectedList;

class DLL_EXPORT ADBDriver
{
private:
    uint32_t             m_display[4];
    std::string          m_device_abi;
    std::string          m_device_sdk;
    std::string          m_device_rel;

    GameDev::DriverAsync m_cmdasync;
    GameDev::DriverNet   m_net;

public:

    struct DLL_EXPORT Tap_s
    {
        int32_t x;
        int32_t y;
    };

    struct DLL_EXPORT Swipe_s
    {
        int32_t x0;
        int32_t y0;
        int32_t x1;
        int32_t y1;
        int32_t t;
    };
    //
    struct DLL_EXPORT DirItemColor
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
    //
    struct DLL_EXPORT DirItem
    {
        std::string         s;
        std::string         cmds;
        std::string         desc;
        DriverConst::FileType type;
        DirItemColor ctxt,  cbg;
        uint8_t             permission[3];
        uint32_t            pmode;
        //
        DirItem();
        DirItem(
            void*,
            void*);
        DirItem(
            std::string const&,
            void*,
            void*);
        DirItem(
            std::string const&,
            std::string const&,
            std::string const&,
            DriverConst::FileType,
            ADBDriver::DirItemColor const&);
        //
        void        Permission(uint32_t);
        std::string Permission();
        //
        void                  Type(uint32_t);
        DriverConst::FileType Type();
        //
        void Description(
            std::string  const&,
            std::string  const&,
            std::string  const&,
            DirItemColor const&);
        //
        template<typename T>
        const T * ColorText() const
        {
            return reinterpret_cast<const T*>(&ctxt);
        }
        template<typename T>
        const T * ColorBg() const
        {
            return reinterpret_cast<const T*>(&cbg);
        }
    };

    typedef bool (video_cb)(std::vector<uint8_t>&, uint32_t, uint32_t);

    /// Driver
    ADBDriver();
    ADBDriver(std::wstring&);

    stdext::MultiString AdbBin;
    stdext::MultiString DeviceId;

    bool        AdbCheck();
    bool        AdbStart(std::wstring&);
    bool        AdbStop (std::wstring&);
    bool        InitRemote();

    /// Driver (Process)
    template<typename T>
        bool    ManagerT(T const&, T&);

    /// Driver(Socket)
    template <typename T>
        bool    AdbRawT(T, T const&, T&, DriverConst::ClearType = DriverConst::ClearType::CLEARTYPE_NONE);

    /// Driver(Socket) Input
    void        Click(ADBDriver::Tap_s*);
    void        Click(ADBDriver::Swipe_s*);
    void        ClickDbl(ADBDriver::Tap_s*);
    void        SendSpecialKey(DriverConst::KeysType, int32_t);
    bool        SendToShell(std::string const&, std::string&, bool = false, DriverConst::ClearType = DriverConst::ClearType::CLEARTYPE_NONE);
    template <typename T>
        bool    SendTextT(T const&, bool = false);

    /// Driver(Socket) FS Directory
    bool         DirList(std::string const&, std::vector<ADBDriver::DirItem>&, std::string &);

    /// Driver(Socket) FS File
    bool         FileSend(std::string const&, std::string const&, DriverConst::FilePermissionType);
    bool         FileReceive(std::string const&, std::string const&, bool = false);
    bool         FileReceive(std::string const&, std::string const&, std::string&, bool = false);
    bool         FileDelete(std::string const&, std::string&);
    bool         FileChmod(std::string const&, DriverConst::FilePermissionType, std::string&);
    std::string  FilePermission(DriverConst::FilePermissionType);
    uint32_t     FilePermission(std::string const&);
    std::string  FileSize(uintmax_t);
    std::string  FileSize(std::string const&);

    /// Driver(Socket) Device
    bool         InstallApk(std::string const&);
    bool         UnInstallApk(std::string const&, std::string&);

    SelectedList GetDeviceList();
    bool         GetDeviceListUI();
    bool         GetDeviceSetupUI();
    std::string  GetDeviceInfo();
    std::string  GetDriverInfo();
    std::string  GetProperties(DriverConst::DeviceInfoType);

    /// Driver(Socket) Capture
    uint32_t     GetDisplayWidth()  const;
    uint32_t     GetDisplayHeight() const;
    uint32_t     GetDisplayType()   const;
    uint32_t     GetDisplaySize()   const;
    uint32_t     GetDisplayMagic()  const;

    std::vector<uint8_t> Capture();
    std::vector<uint8_t> Capture(DriverConst::CapType, uint32_t, uint32_t);
    bool                 Video(uint32_t, uint32_t, std::function<video_cb>);
    bool                 Video(uint32_t, uint32_t, uint32_t, std::function<video_cb>);

private:

    void deviceinfo();
    //
#   if defined(_BUILD_DIRLIST_BY_SHELL)
    bool dirlistitem(std::string const&, std::string const&, ADBDriver::DirItem&);
#   endif
};

extern template bool ADBDriver::ManagerT<std::wstring>(std::wstring const&, std::wstring&);
extern template bool ADBDriver::ManagerT<std::string> (std::string  const&, std::string&);

extern template bool ADBDriver::AdbRawT<std::wstring>(std::wstring, std::wstring const&, std::wstring&, DriverConst::ClearType);
extern template bool ADBDriver::AdbRawT<std::string> (std::string,  std::string const&,  std::string&,  DriverConst::ClearType);

extern template bool ADBDriver::SendTextT<std::wstring>(std::wstring const&, bool);
extern template bool ADBDriver::SendTextT<std::string> (std::string const&,  bool);

}
